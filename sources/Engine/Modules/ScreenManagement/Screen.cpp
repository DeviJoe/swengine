#include "Screen.h"
#include "ScreenManager.h"

Screen::Screen(const std::string& name)
    : m_name(name),
      m_guiLayout(std::make_shared<GUILayout>())
{

}

Screen::~Screen()
{

}

std::string Screen::getName() const
{
    return m_name;
}

void Screen::load()
{

}

void Screen::unload()
{

}

void Screen::update(float delta)
{
    ARG_UNUSED(delta);
}

void Screen::render()
{

}

std::shared_ptr<GUILayout> Screen::getGUILayout() const
{
    return m_guiLayout;
}

void Screen::activate()
{

}

void Screen::deactivate()
{

}

void Screen::activateNextScreen(const std::string& name)
{
    SW_ASSERT(m_screenManager != nullptr);

    m_screenManager->changeScreen(name);
}

void Screen::performInternalInitialization(std::shared_ptr<ScreenManager> screenManager,
                                      std::shared_ptr<GameWorld> gameWorld,
                                      std::shared_ptr<GraphicsModule> graphicsModule,
                                      std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
                                      std::shared_ptr<ResourceManager> resourceManager)
{
    m_screenManager = screenManager;
    m_gameWorld = gameWorld;
    m_graphicsModule = graphicsModule;
    m_sharedGraphicsState = sharedGraphicsState;
    m_resourceManager = resourceManager;

    m_guiLayout->setSize(screenManager->getCommontGUILayout()->getSize());
}
