#include "entity_manager.h"
#include "archetype_manager.h"

namespace Engine {
namespace Entity_Manager {

#define ENTITY_RESIZE_SIZE 1000

typedef struct {
    std::string archetype_name;
    uint32_t    add_count;
} entity_add_node_t;

typedef struct {
    uint32_t                                    next_entity_idx;
    std::vector<Entity>                         entity_ids;       // Vector of all Entity ids
    std::vector<internal_entity_id>             internal_entity_ids;
    std::vector<uint32_t>                       free_entity_idx;
    std::vector<Entity>                         entities_to_free;
} entity_manager_t;

entity_manager_t entity_manager;

void _increase_entity_vec( void )
{
    size_t current_size = entity_manager.entity_ids.size();

    CHECK( (current_size + ENTITY_RESIZE_SIZE) < U32_MAX );

    entity_manager.entity_ids.resize( current_size + ENTITY_RESIZE_SIZE );
    entity_manager.internal_entity_ids.resize( current_size + ENTITY_RESIZE_SIZE );

    // loop through new nodes and set them up
    for (; current_size < entity_manager.entity_ids.size(); current_size++ ) {
        entity_manager.entity_ids[current_size].active = 0;
        entity_manager.entity_ids[current_size].index = current_size;
    }
}

void _reduce_entity_vec( void )
{
    //TODO(JOSH): need to implement.
    CHECK_INFO( 0, "_reduce_entity_vec not implemented" );
}

Rc_t init( void )
{
    entity_manager.next_entity_idx = 0;

    _increase_entity_vec();

    return SUCCESS;
}

void update()
{
    // check to see if entities need to be cleaned up
    if ( !entity_manager.entities_to_free.empty() ) {
        for ( uint32_t ii = 0; ii < entity_manager.entities_to_free.size(); ++ii ) {
            Archetype_Manager::remove_entity(entity_manager.internal_entity_ids[entity_manager.entities_to_free[ii].index]);
            entity_manager.entity_ids[entity_manager.entities_to_free[ii].index].phase++;
            entity_manager.entity_ids[entity_manager.entities_to_free[ii].index].active = HANDLE_SET_NON_ACTIVE;
            entity_manager.free_entity_idx.push_back(entity_manager.entities_to_free[ii].index);
        }

        entity_manager.entities_to_free.clear();
    }

    Archetype_Manager::update_entity_counts();
}

bool is_valid_entity( Entity entity )
{
    return (entity.id == entity_manager.entity_ids[entity.index].id);
}

internal_entity_id get_internal_id( Entity entity )
{
    return entity_manager.internal_entity_ids[entity.index];
}

internal_entity_id* get_internal_id_pointer( Entity entity )
{
    return &entity_manager.internal_entity_ids[entity.index];
}

} // end namespace Entity_Manager

Entity create_entity( std::string archetype_name )
{
    Entity return_entity = {0};
    uint64_t internal_idx;

    // check to see if there is a node in the free vector
    if ( !Entity_Manager::entity_manager.free_entity_idx.empty() ) { //TODO(JOSH): should be able to get ride of this branch
        uint32_t free_idx = Entity_Manager::entity_manager.free_entity_idx.back();
        Entity_Manager::entity_manager.free_entity_idx.pop_back();

        Entity_Manager::entity_manager.entity_ids[free_idx].active = HANDLE_SET_ACTIVE;
        internal_idx = free_idx;
        return_entity = Entity_Manager::entity_manager.entity_ids[free_idx];
    } else {
        // check to see if we have run out of entities in the vector
        if ( Entity_Manager::entity_manager.entity_ids.size() == Entity_Manager::entity_manager.next_entity_idx ) {
            Entity_Manager::_increase_entity_vec();
        }
        internal_idx = Entity_Manager::entity_manager.next_entity_idx;
        Entity_Manager::entity_manager.entity_ids[Entity_Manager::entity_manager.next_entity_idx].active = HANDLE_SET_ACTIVE;
        return_entity.id = Entity_Manager::entity_manager.entity_ids[Entity_Manager::entity_manager.next_entity_idx].id;
        Entity_Manager::entity_manager.next_entity_idx++;
    }

    Archetype_Manager::register_entity(return_entity, &Entity_Manager::entity_manager.internal_entity_ids[internal_idx], archetype_name);

    return return_entity;
}

Rc_t delete_entity( Entity entity )
{
    // check to see if the Entity is valid
    if ( entity.id == Entity_Manager::entity_manager.entity_ids[entity.index].id ) {
        // we defer the free of an entity until the end of the frame
        Entity_Manager::entity_manager.entities_to_free.push_back(entity);

        return SUCCESS;
    }

    return ERROR_INVALID_ENTITY;
}

bool is_valid_entity( Entity entity )
{
    return (entity.id == Entity_Manager::entity_manager.entity_ids[entity.index].id);
}

} // end namespace Engine
