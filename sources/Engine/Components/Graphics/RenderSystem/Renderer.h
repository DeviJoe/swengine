#pragma once

#include <Engine\Components\GUI\Window.h>
#include <Engine\Components\Math\Math.h>

#include "Camera.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Color.h"
#include "Material.h"
#include "GpuProgram.h"
#include "Texture.h"
#include "Light.h"
#include "Model.h"
#include "Framebuffer.h"
#include "HardwareBuffer.h"

enum class ClearRenderTargetMode {
	Color, ColorDepth, Depth
};

enum class DrawPrimitivesMode {
	Triangles, TrianglesStrip
};

class Renderer {
public:
	enum class Option {
		DepthTest, DepthFunction, 
		FaceCulling, FaceCullingMode, FrontFace,
		MultiSample
	};

	enum class OptionValue {
		Enabled, Disabled,
		Less, LessEqual, // DepthFunction
		Front, Back, FrontAndBack,  // Face culling
		Clockwise, CounterClockwise
	};

	virtual Window* getWindow() const = 0;

	virtual void startFrame() = 0;
	virtual void endFrame() = 0;

	virtual void setCurrentCamera(Camera* camera) = 0;
	virtual Camera* getCurrentCamera() const = 0;

	virtual void drawPrimitives(DrawPrimitivesMode mode, size_t start, size_t count) = 0;
	virtual void drawIndexedPrimitives(DrawPrimitivesMode mode, size_t count) = 0;

	virtual void drawNDCQuad(GpuProgram* program) = 0;
	virtual void drawSprite(Sprite*, const glm::vec2&, const glm::vec2&, float) = 0;
	virtual void drawMesh(const Mesh* mesh, const matrix4& transform, const Material* material) = 0;
	virtual void drawModel(const Model*) = 0;

	virtual void swapBuffers() = 0;

	virtual void clearCurrentRenderTarget(const Color& color, ClearRenderTargetMode mode) = 0;
	virtual void setCurrentRenderTarget(Framebuffer* framebuffer) = 0;
	virtual Framebuffer* getCurrentRenderTarget() const = 0;
	virtual void resetRenderTarget() = 0;

	virtual void copyFramebufferDataToDefaultBuffer(Framebuffer* framebuffer) = 0;

	virtual void addLight(Light*) = 0;

	virtual void bindTextureUnit(const Texture* texture, size_t unit) = 0;
	virtual void bindShader(GpuProgram* gpuProgram) = 0;
	virtual void bindShaderParameters(GpuProgram* program) {};
	virtual void bindShaderData(GpuProgram* program) = 0;

	virtual void bindMaterial(const Material* material) = 0;

	virtual void bindGeometryBuffer(const HardwareBuffer* buffer) = 0;
	virtual void unbindGeometryBuffer() = 0;

	virtual void setShaderAutobindingData(const std::string& name, const GpuProgram::Parameter& value) = 0;

	virtual void setViewport(int width, int height) = 0;
	virtual void setOption(Option option, OptionValue value) = 0;
};