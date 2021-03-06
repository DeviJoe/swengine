#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


class MathUtils {
 public:
  static bool isEqual(const glm::mat4& a, const glm::mat4& b, float eps = 1e-6f);
  static bool isEqual(const glm::quat& a, const glm::quat& b, float eps = 1e-6f);
  static bool isEqual(const glm::vec3& a, const glm::vec3& b, float eps = 1e-6f);
  static bool isEqual(float a, float b, float eps = 1e-6f);

  static bool isMatrixIdentity(const glm::mat4& matrix, const float eps = 1e-6f);

  static glm::mat4 getTranslationMatrix(const glm::vec3& translation);
  static glm::mat4 getRotationMatrix(const glm::vec3& axis, float angle);
  static glm::mat4 getRotationMatrixFromYawPitchRoll(float yaw, float pitch, float roll);
  static glm::mat4 getScaleMatrix(const glm::vec3& scale);

  static glm::mat4 getYawMatrix(float angle);
  static glm::mat4 getPitchMatrix(float angle);
  static glm::mat4 getRollMatrix(float angle);
  
 public:
  static constexpr glm::vec3 AXIS_X = {1.0f, 0.0f, 0.0f};
  static constexpr glm::vec3 AXIS_Y = {0.0f, 1.0f, 0.0f};
  static constexpr glm::vec3 AXIS_Z = {0.0f, 0.0f, 1.0f};

  static constexpr glm::mat4 IDENTITY_MATRIX4 = glm::identity<glm::mat4>();
  static constexpr glm::quat IDENTITY_QUAT = glm::identity<glm::quat>();

  static constexpr glm::vec3 ZERO_VEC3 = {0.0f, 0.0f, 0.0f};
};

template<class T>
bool isVectorsEqual(const T& a, const T& b, float eps = 1e-6f)
{
  using vector_length_type = typename T::length_type;

  for (vector_length_type coordinateIndex = 0; coordinateIndex < a.length(); coordinateIndex++) {
    if (glm::abs(a[coordinateIndex] - b[coordinateIndex]) > eps) {
      return false;
    }
  }

  return true;
}
