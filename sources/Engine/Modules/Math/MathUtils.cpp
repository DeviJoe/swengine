#include "MathUtils.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

glm::mat4 MathUtils::getTranslationMatrix(const glm::vec3& translation)
{
  return glm::translate(IDENTITY_MATRIX4, translation);
}

glm::mat4 MathUtils::getRotationMatrix(const glm::vec3& axis, float angle)
{
  return glm::rotate(IDENTITY_MATRIX4, glm::radians(angle), axis);
}

glm::mat4 MathUtils::getRotationMatrixFromYawPitchRoll(float yaw, float pitch, float roll)
{
  return glm::yawPitchRoll(glm::radians(yaw), glm::radians(pitch), glm::radians(roll));
}

glm::mat4 MathUtils::getScaleMatrix(const glm::vec3& scale)
{
  return glm::scale(IDENTITY_MATRIX4, scale);
}

glm::mat4 MathUtils::getYawMatrix(float angle)
{
  return getRotationMatrix(AXIS_Y, angle);
}

glm::mat4 MathUtils::getPitchMatrix(float angle)
{
  return getRotationMatrix(AXIS_X, angle);
}

glm::mat4 MathUtils::getRollMatrix(float angle)
{
  return getRotationMatrix(AXIS_Z, angle);
}

bool MathUtils::isEqual(const glm::mat4& a, const glm::mat4& b, float eps)
{
  using matrix_length_type = glm::mat4::length_type;

  for (matrix_length_type c = 0; c < 4; c++) {
    for (matrix_length_type r = 0; r < 4; r++) {
      if (glm::abs(a[c][r] - b[c][r]) > eps) {
        return false;
      }
    }
  }

  return true;
}

bool MathUtils::isEqual(const glm::quat& a, const glm::quat& b, float eps)
{
  return glm::abs(glm::dot(a, b)) > 1 - eps;
}

bool MathUtils::isEqual(const glm::vec3& a, const glm::vec3& b, float eps)
{
  return isVectorsEqual(a, b, eps);
}

bool MathUtils::isEqual(float a, float b, float eps)
{
  return glm::abs(a - b) <= eps;
}

bool MathUtils::isMatrixIdentity(const glm::mat4& matrix, const float eps)
{
  return isEqual(matrix, glm::identity<glm::mat4>(), eps);
}
