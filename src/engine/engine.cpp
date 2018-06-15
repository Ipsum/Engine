#include "engine.h"
#include "components/components.h"
#include "components/components.cpp"
#include "component_systems/component_systems.h"
#include "component_systems/component_systems.cpp"
#include "asset_types/assets.h"
#include "asset_types/assets.cpp"
#include "renderer.h"

Engine* Engine::instance = NULL;

Engine* Engine::get_instance()
{
    if ( instance == NULL ) {
        instance = new Engine;
    }
    return instance;
}

void Engine::init()
{
    //load opengl extentions (important this must be done before creating window)
    load_opengl_extensions();

    //create the game window
    this->window = new core::Window(1000, 800, "Launcher");

    //setup component manager
    component_manager = core::Component_Manager::get_instance();

    //setup entity manager
    enitiy_manager = core::Entity_Manager::get_instance();

    //setup system manager
    system_manager = core::System_Manager::get_instance();

    //setup asset manager
    asset_manager = core::Asset_Manager::get_instance();

    //setup event manager
    //uh::CEMS::get_instance();

    //register all components NOTE: this will need to be generated maybe
    component_manager->register_component<Position_Component>();
    component_manager->register_component<Mesh_Component>();
    component_manager->register_component<Shader_Component>();
    //TODO: function that will call users reg components

    //register all systems NOTE: this will need to be generated maybe
    system_manager->register_system<Mesh_Render_System>();
    //TODO: function that will call users reg systems


    //TODO: add function that will call users init code

    system_manager->init_systems();
}

void Engine::update()
{
    //update the time
    this->frame_time.update();

    //update the window
    this->window->update();

    //process an event from the event queue
    core::IEMS::process_event();

    //run all system updates
    system_manager->update_systems();
}

void Engine::shutdown()
{
    system_manager->shutdown_systems();
}