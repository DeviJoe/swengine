#pragma once

#include <string>

#include <Engine\Engine.h>


class BaseApplication {
public:
	BaseApplication(const std::string& windowName, unsigned int width, unsigned int height);
	virtual ~BaseApplication();

	virtual void run();
	virtual void update();
	virtual void render();

protected:
	Window* m_window;
	Renderer* m_renderer;

	InputManager* m_inputMgr;
	ResourceManager* m_resMgr;
	SceneManager* m_sceneMgr;
};