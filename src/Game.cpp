#include "Game.h"

#include <GLEW/glew.h>
#include <GLFW/glfw.h>
#include <GLM/glm.hpp>
#include <cassert>
#include <iostream>

#include "Resources/resource_factory.h"
#include "Resources/program_resource_loader.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Resources/DirectoryManifest.h"
#include "resource_dir.h"

#include "Components/SpatialComponent.h"
#include "Components/ModelComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ControlComponent.h"
#include "Components/SpatialHierarchyComponent.h"
#include "Components/HUDTextComponent.h"
#include "Components/TimeComponent.h"
#include "Components/TTLComponent.h"

//HACK
#include "EntityParser/EntityParser.h"

Game::Game() : _input_system(&_world), _render_system(&_world), _movement_control_system(&_world), _spatial_hierarchy_system(&_world), _time_left_system(&_world), _ttl_system(&_world), _script_sytem(&_world) {
}

void Game::init() {
    std::cout << "--- Baldur Game engine" << std::endl;
    std::cout << "------ Copyright Tim Peters 2013" << std::endl;
    std::cout << "--- Initializing GLFW" << std::endl;
    initGLFW();
    std::cout << "--- Initializing GLEW" << std::endl;
    initGLEW();
    
    std::cout << "--- Initializing Resource Factory" << std::endl;
    
    std::cout << "------ Manifest" << std::endl;
    auto manifest = std::make_shared<Ymir::DirectoryManifest>(ResourceDirectory() + "/Resources");
    resource_factory::instance().add_manifest(manifest);
    
    std::cout << "------ Program Loader" << std::endl;
    resource_factory::instance().add_loader("program", std::make_shared<program_resource_loader>());
    
    std::cout << "------ Mesh Loader" << std::endl;
    resource_factory::instance().add_loader("mesh", std::make_shared<default_resource_loader<Ymir::Mesh>>());
    
    std::cout << "------ Texture Loader" << std::endl;
    resource_factory::instance().add_loader("texture", std::make_shared<default_resource_loader<Ymir::Texture>>());
    
    std::cout << "--- Initializing InputSystem" << std::endl;
    _input_system.init();
    
    std::cout << "--- Initializing RenderSystem" << std::endl;
    _render_system.init();
    
    std::cout << "--- Initializing MovementControlSystem" << std::endl;
    _movement_control_system.init();
    
    std::cout << "--- Initializing SpatialHierarchySystem" << std::endl;
    _spatial_hierarchy_system.init();
    
    std::cout << "--- Initializing TimeLeftSystem" << std::endl;
    _time_left_system.init();
    
    std::cout << "--- Initializing TTLSystem" << std::endl;
    _ttl_system.init();
    
    std::cout << "--- Initializing ScriptSystem" << std::endl;
    _script_sytem.init();
}

void Game::shutdown() {
    std::cout << "--- Shutting down ScriptSystem" << std::endl;
    _script_sytem.shutdown();
    
    std::cout << "--- Shutting down TTLSystem" << std::endl;
    _ttl_system.shutdown();
    
    std::cout << "--- Shutting down TimeLeftSystem" << std::endl;
    _time_left_system.shutdown();
    
    std::cout << "--- Shutting down SpatialHierarchySystem" << std::endl;
    _spatial_hierarchy_system.shutdown();
    
    std::cout << "--- Shutting down MovementControlSystem" << std::endl;
    _movement_control_system.shutdown();
    
    std::cout << "--- Shutting down RenderSystem" << std::endl;
    _render_system.shutdown();
    
    std::cout << "--- Shutting down InputSystem" << std::endl;
    _input_system.shutdown();
    
    std::cout << "--- Shutting down GLFW" << std::endl;
    glfwTerminate();
}

int frames = 0;
double t0 = 0.0;
#include <sstream>
void showFPS(const char* title) {

    double t, fps;

    // Get current time
    t = glfwGetTime();  // Get number of seconds since glfwInit()
    // Calculate and display FPS (frames per second) in title bar.
    if( (t-t0) > 1.0 || frames == 0 )
    {
        fps = (double)frames / (t-t0);
        std::stringstream ss;
        ss << title << " | FPS: " << fps;
        glfwSetWindowTitle(ss.str().c_str());
        t0 = t;
        frames = 0;
    }
    frames ++;
}

void Game::build() {
    //SHIP
    Entity* spaceship = _world.loadEntity("spaceship.entity", "Ship");
    auto ship_spatial = _world.component<SpatialComponent>(spaceship);
    //END SHIP
    
    //BOX
    EntityHandle box = _world.loadEntity("box.entity", "Box");
    auto box_spatial = _world.component<SpatialComponent>(box);
    box_spatial->spatial.translate(glm::vec3(3,0,0));
    //END BOX
    
    //BOX
    _world.loadEntity("box.entity", "SomeTestBox");
    //END BOX
    
    //CAMERA
    Entity* camera = _world.loadEntity("camera.entity", "Camera");
    
    auto camera_hier = _world.createComponent<SpatialHierarchyComponent>(camera);
    camera_hier->local.translate(glm::vec3(0, 0, 2));
    camera_hier->local.look_at(ship_spatial->spatial);
    camera_hier->owner = spaceship;
    //END CAMERA
    
    systems::build();
}

void Game::run() {
    
    bool run = true;
    double last_time = glfwGetTime();
    while(run) {
        double this_time = glfwGetTime();
        double delta = this_time - last_time;
        if(delta > 2) delta = 1/60;
        last_time = this_time;
        
        for(int i = 0; i < 32; ++i) {
            events::runEvents();
            if(systems::step(i, delta) == false) run = false;
        }
        showFPS("Baldur");
        glfwSwapBuffers();
    }
}

void Game::initGLFW() {
    assert(glfwInit());

    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

    assert(glfwOpenWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, 8, 8, 8, 8, 16, 0, GLFW_WINDOW));

    // GLFW settings
    glfwSwapInterval(0); //uncomment for no v-sync
}

void Game::initGLEW() {
    glewExperimental = GL_TRUE; //stops glew crashing on OSX
    assert(glewInit() == GLEW_OK);
    glGetError(); //ignore any errors coming from glewInit, because they can be safely ignored.

    std::cout << "------ OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "------ GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "------ Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "------ Renderer: " << glGetString(GL_RENDERER) << std::endl;

    assert(GLEW_VERSION_3_2);
}
