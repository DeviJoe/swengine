#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "GL.h"
#include "GLTexture.h"

class GLShadersPipeline;

class GLShader {
 public:
  GLShader(GLenum type, const std::string& source);
  ~GLShader();

  [[nodiscard]] GLenum getType() const;

  void setParameter(const std::string& name, bool value);
  void setParameter(const std::string& name, int value);
  void setParameter(const std::string& name, float value);
  void setParameter(const std::string& name, const glm::vec2& value);
  void setParameter(const std::string& name, const glm::vec3& value);
  void setParameter(const std::string& name, const glm::vec4& value);
  void setParameter(const std::string& name, const glm::mat4x4& value);
  void setParameter(const std::string& name, const GLTexture& texture, size_t unitIndex);

  [[nodiscard]] size_t getArraySize(const std::string& name) const;

  void setArrayParameter(const std::string& name, const std::vector<glm::mat4x4>& array);
  void setArrayParameter(const std::string& name, size_t valueIndex, const glm::mat4x4& value);

  [[nodiscard]] bool hasParameter(const std::string& name) const;
 private:
  struct UniformInfo {
    GLint location;
    GLsizei size;
  };

 private:
  void cacheUniformsLocations();

 private:
  GLuint m_shaderProgram;
  GLenum m_type;

  std::unordered_map<std::string, UniformInfo> m_uniformsCache;

 private:
  friend class GLShadersPipeline;
};

