#pragma once

#include <Engine/Modules/Graphics/GraphicsSystem/Camera.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Mesh.h>

class PlayerComponent {
 public:
  PlayerComponent();

  void setMovementSpeed(float speed);
  [[nodiscard]] float getMovementSpeed() const;

  void setDistanceToPlayer(float distance);
  void increaseDistanceToPlayer(float delta);
  [[nodiscard]] float getDistanceToPlayer() const;

  void setThirdPersonViewPitch(float pitch);
  void increaseThirdPersonViewPitch(float delta);
  [[nodiscard]] float getThirdPersonViewPitch() const;

  void setThirdPersonViewYaw(float yaw);
  void increaseThirdPersonViewYaw(float delta);
  [[nodiscard]] float getThirdPersonViewYaw() const;

 private:
  float m_movementSpeed = 2.15f;

  float m_distanceToPlayer = 3.0f;
  float m_thirdPersonViewPitch = 45.0f;
  float m_thirdPersonViewYaw = 0.0f;
};

