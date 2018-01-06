#pragma once

#include <Engine\types.h>
#include <Engine\Components\Graphics\OpenGL3\OpenGL3.h>

#include <string>

struct WindowCursorState {
	real64 x;
	real64 y;
};

enum class CursorType {
	Default, Hidden, Manual
};

class Window {
public:

public:
	Window();
	~Window();

	void initialize(const std::string&, int, int);
	void shutdown();
	void update();
	void render();

	int16 getWidth() const;
	int16 getHeight() const;

	void setCursorType(CursorType);
	CursorType getCursorType() const;
	
	void setCursorPosition(real64, real64);
	void resetCursorPosition();
	WindowCursorState getCursorPosition() const;
	void getCursorPosition(real64*, real64*) const;

	void close();
	bool shouldClose() const;

	GLFWwindow* getWindowPointer() const;
private:
	GLFWwindow* m_windowPointer;
	CursorType m_cursorType;

	int16 m_width;
	int16 m_height;
};