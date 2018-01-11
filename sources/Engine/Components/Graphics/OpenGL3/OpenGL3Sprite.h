#pragma once

#include <Engine\Components\Graphics\RenderSystem\Sprite.h>
#include "OpenGL3Texture.h"
#include "OpenGL3GpuProgram.h"

class OpenGL3Sprite : public Sprite {
public:
	OpenGL3Sprite(Texture* texture, GpuProgram* gpuProgram);
	~OpenGL3Sprite();

	void setTexture(Texture* texture) override;
	const Texture* getTexture() const override;

	void setShader(GpuProgram*) override;
	GpuProgram* getShader() const override;

	GLuint getVertexArrayPointer() const;
private:
	OpenGL3Texture* m_texture;
	OpenGL3GpuProgram* m_shaderProgram;

	GLuint m_VertexArrayObject;
	GLuint m_VertexBufferObject;
};