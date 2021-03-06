#pragma once

#include <catch2/catch.hpp>

#include <Engine/Modules/Graphics/GraphicsSystem/Animation/Skeleton.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/AnimationClip.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/AnimationStatesMachine.h>

#include <Engine/Modules/Math/MathUtils.h>

Skeleton generateTestSkeleton();
AnimationClip generateTestAnimationClip();
AnimationClipInstance generateTestAnimationClipInstance();
