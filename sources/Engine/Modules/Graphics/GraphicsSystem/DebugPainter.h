#pragma once

#include <vector>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/GraphicsSystem/Mesh.h"
#include "Modules/Graphics/GraphicsSystem/Camera.h"

#include "Modules/Graphics/Resources/MeshResource.h"
#include "Modules/Graphics/Resources/ShaderResource.h"

#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "Modules/Graphics/OpenGL/GLShadersPipeline.h"

#include "Modules/Math/geometry.h"

struct DebugRenderQueueItem {
  GLGeometryStore* geometry;
  glm::mat4x4 transformationMatrix;
  glm::vec4 color = glm::vec4(0.0f);
  bool isWireframe = false;
  GLenum primitivesType = GL_TRIANGLES;
};

class DebugPainter {
 public:
  DebugPainter() = delete;

  static void initialize(std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<SharedGraphicsState> sharedGraphicsState);

  static void renderSegment(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
  static void renderVector(const glm::vec3& origin, const glm::vec3& direction, const glm::vec4& color);
  static void renderBasis(const glm::vec3& origin, const glm::vec3& x, const glm::vec3& y, const glm::vec3& z);

  static void renderSphere(const glm::vec3& centerPosition,
    float radius,
    const glm::vec4& color = {},
    bool wireframe = true);
  static void renderSphere(const Sphere& sphere, const glm::vec4& color = {}, bool wireframe = true);

  static void renderBox(const glm::vec3& centerPosition, const glm::vec3& halfSize,
    const glm::quat& orientation, const glm::vec4& color = {}, bool wireframe = true);

  static void renderFrustum(const glm::mat4x4& view, const glm::mat4x4& projection,
    const glm::vec4& color = {}, bool wireframe = true);

  static void renderFrustum(const Frustum& frustum,
    const glm::vec4& color = {});

  static void renderAABB(const glm::vec3& min,
    const glm::vec3& max,
    const glm::vec4& color = {},
    bool wireframe = true);
  static void renderAABB(const AABB& aabb, const glm::vec4& color = {}, bool wireframe = true);

  static void flushRenderQueue(GLGraphicsContext* graphicsContext);

 private:
  static GLGeometryStore* createGeometryStore(const std::vector<glm::vec3>& points);

 private:
  static std::shared_ptr<Mesh> s_sphere;
  static std::shared_ptr<Mesh> s_box;

  static std::shared_ptr<GLShadersPipeline> s_debugShaderPipeline;
  static std::unique_ptr<GLMaterial> s_debugMaterial;

  static std::shared_ptr<SharedGraphicsState> s_sharedGraphicsState;

  static std::vector<std::unique_ptr<GLGeometryStore>> s_primitivesGeometry;
  static std::vector<DebugRenderQueueItem> s_debugRenderQueue;
};

