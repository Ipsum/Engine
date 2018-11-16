#include "archetype.h"

namespace Engine {

Archetype::Archetype()
{

}

void Archetype::add_component( uint32_t component_id )
{
    std::unordered_map<uint32_t, uint64_t>::const_iterator ele = component_map.find(component_id);
    if ( ele == component_map.end() ) {
        used_component_ids.push_back(component_id);
        component_map.insert({ component_id, used_component_ids.size() - 1 });
    }
}

bool Archetype::has_component( uint32_t component_id )
{
    std::unordered_map<uint32_t, uint64_t>::const_iterator ele = component_map.find(component_id);
    if ( ele == component_map.end() ) {
        return false;
    }

    return true;
}

} // end namespace Engine
