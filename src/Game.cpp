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

static const glm::vec2 SCREEN_SIZE (800, 600);

Game::Game() : _input_system(&_world) {
}

void Game::init() {
    std::cout << "--- Baldur Game engine" << std::endl;
    std::cout << "------ Copyright Tim Peters 2013" << std::endl;
    std::cout << "--- Initializing GLFW" << std::endl;
    initGLFW();
    std::cout << "--- Initializing GLEW" << std::endl;
    initGLEW();
    
    std::cout << "--- Initializing Resource Loaders" << std::endl;
    
    std::cout << "------ Program Loader" << std::endl;
    resource_factory::instance().add_loader("program", std::make_shared<program_resource_loader>());
    
    std::cout << "------ Mesh Loader" << std::endl;
    resource_factory::instance().add_loader("mesh", std::make_shared<default_resource_loader<Ymir::Mesh>>());
    
    std::cout << "------ Texture Loader" << std::endl;
    resource_factory::instance().add_loader("texture", std::make_shared<default_resource_loader<Ymir::Texture>>());
    
    std::cout << "--- Initializing InputSystem" << std::endl;
    _input_system.init();
}

void Game::shutdown() {
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

void Game::run() {
    
    bool run = true;
    double last_time = glfwGetTime();
    while(run) {
        double this_time = glfwGetTime();
        double delta = this_time - last_time;
        last_time = this_time;
        
        events::runEvents();
        for(int i = 0; i < 32; ++i)
            if(systems::step(i, delta) == false) run = false;
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
