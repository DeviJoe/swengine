#include "BaseGameApplication.h"

#include <Exceptions/EngineRuntimeException.h>
#include <spdlog/spdlog.h>

BaseGameApplication::BaseGameApplication(int argc, char* argv[], const std::string& windowTitle, int windowWidth, int windowHeight)
    : m_mainWindow(nullptr)
{
    spdlog::info("Application start...");

    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    int initStatus = SDL_Init(SDL_INIT_EVERYTHING);

    if (initStatus != 0) {
        ENGINE_RUNTIME_ERROR(std::string(SDL_GetError()));
    }

    spdlog::info("SDL is initialized");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    spdlog::info("Create main window...");

    m_mainWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight, SDL_WINDOW_OPENGL);

    if (m_mainWindow == nullptr) {
        ENGINE_RUNTIME_ERROR(std::string(SDL_GetError()));
    }

    spdlog::info("Window is created");

    spdlog::info("Initialize engine modules...");

    m_graphicsModule = std::make_unique<GraphicsModule>(m_mainWindow);
    m_gameWorld = std::make_unique<GameWorld>();

    spdlog::info("Engine modules are initialized");
}

BaseGameApplication::~BaseGameApplication()
{
    SDL_DestroyWindow(m_mainWindow);
}

void BaseGameApplication::load()
{

}

void BaseGameApplication::unload()
{

}

void BaseGameApplication::update(float delta)
{
    ARG_UNUSED(delta);
}

void BaseGameApplication::render()
{

}

int BaseGameApplication::execute()
{
    spdlog::info("Perform game application loading...");
    performLoad();
    spdlog::info("Game application is loaded and ready...");

    SDL_ShowWindow(m_mainWindow);

    const int FRAMES_PER_SECOND = 30;
    const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

    unsigned long nextTick = GetTickCount();

    long sleepTime = 0;

    SDL_Event event;
    bool isRun = true;

    spdlog::info("Starting main loop...");

    while (isRun) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                isRun = false;
                break;
            }
        }

        if (!isRun)
            break;

        performUpdate(1.0f / FRAMES_PER_SECOND);
        performRender();

        nextTick += SKIP_TICKS;
        sleepTime = static_cast<long>(nextTick) - static_cast<long>(GetTickCount());

        if (sleepTime >= 0) {
            SDL_Delay(static_cast<unsigned long>(sleepTime));
        }
    }

    spdlog::info("Perform game application unloading...");
    performUnload();
    spdlog::info("Game application is unloaded...");

    return 0;
}

void BaseGameApplication::performLoad()
{
    load();
}

void BaseGameApplication::performUnload()
{
    unload();
}

void BaseGameApplication::performUpdate(float delta)
{
    update(delta);
}

void BaseGameApplication::performRender()
{
    render();
    m_graphicsModule->getGraphicsContext()->swapBuffers();
}