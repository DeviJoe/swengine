#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Skeleton.h"

struct BoneAnimationPositionFrame {
    float time;
    glm::vec3 position;
};

struct BoneAnimationOrientationFrame {
    float time;
    glm::quat orientation;
};

struct SkeletalAnimationMatrixPalette {
    SkeletalAnimationMatrixPalette(const std::vector<glm::mat4>& bonesTransforms);

    std::vector<glm::mat4> bonesTransforms;
};

class BoneAnimationChannel {
public:
    BoneAnimationChannel() = default;
    BoneAnimationChannel(const std::vector<BoneAnimationPositionFrame>& positionFrames,
                         const std::vector<BoneAnimationOrientationFrame>& orientationFrames);

    ~BoneAnimationChannel() = default;

    std::vector<BoneAnimationPositionFrame>& getPositionFrames();
    const std::vector<BoneAnimationPositionFrame>& getPositionFrames() const;

    const std::vector<BoneAnimationOrientationFrame>& getOrientationFrames() const;
    std::vector<BoneAnimationOrientationFrame>& getOrientationFrames();

private:
    std::vector<BoneAnimationPositionFrame> m_positionFrames;
    std::vector<BoneAnimationOrientationFrame> m_orientationFrames;
};

class SkeletalAnimationClip
{
public:
    SkeletalAnimationClip(const std::string& name,
                          float duration,
                          float rate,
                          const std::vector<BoneAnimationChannel>& bonesAnimationChannels);

    SkeletalAnimationClip(const SkeletalAnimationClip& clip);

    ~SkeletalAnimationClip() = default;

    const std::string& getName() const;

    void setDuration(float duration);
    float getDuration() const;

    void setRate(float rate);
    float getRate() const;

    BonePose getBoneRelativePose(uint8_t boneIndex, float time) const;

private:
    template<class T, class S>
    T getMixedAdjacentFrames(const std::vector<S>& frames, float time) const;

    template<class T>
    T getIdentity() const;

    template<class T, class S>
    T getFrameValue(const S& frame) const;

    template<class T>
    T getMixedValue(const T& first, const T& second, float delta) const;

private:
    std::string m_name;
    std::vector<BoneAnimationChannel> m_bonesAnimationChannels;

    float m_duration = 0.0f;
    float m_rate = 0.0f;
};

class SkeletalAnimationClipInstance;

class SkeletalAnimationPose {
public:
    SkeletalAnimationPose(std::shared_ptr<Skeleton> skeleton,
            const std::vector<BonePose>& bonesPoses);

    void setBoneLocalPose(uint8_t boneIndex, const BonePose& pose);
    const BonePose& getBoneLocalPose(uint8_t boneIndex) const;

    const SkeletalAnimationMatrixPalette& getMatrixPalette() const;

private:
    const BonePose& getBoneGlobalPose(uint8_t boneIndex) const;
    void updateBoneGlobalPose(uint8_t boneIndex, const BonePose& pose);

private:
    std::shared_ptr<Skeleton> m_skeleton;

    std::vector<BonePose> m_bonesLocalPoses;
    std::vector<BonePose> m_bonesGlobalPoses;

    mutable SkeletalAnimationMatrixPalette m_matrixPalette;
    mutable bool m_isMatrixPaletteOutdated = true;

private:
    friend class SkeletalAnimationClipInstance;
};

class SkeletalAnimationClipInstance {
public:
    SkeletalAnimationClipInstance(std::shared_ptr<Skeleton> skeleton,
                                  std::shared_ptr<SkeletalAnimationClip> animationClip);

    const SkeletalAnimationClip& getAnimationClip() const;
    SkeletalAnimationClip& getAnimationClip();

    void setScale(float scale);
    float getScale() const;

    void setLooped(bool looped);
    bool isLooped() const;

    const SkeletalAnimationPose& getAnimationPose() const;

    const Skeleton& getSkeleton() const;

    void increaseCurrentTime(float delta);
    float getCurrentTime() const;

private:
    std::shared_ptr<Skeleton> m_skeleton;

    std::shared_ptr<SkeletalAnimationClip> m_animationClip;
    mutable SkeletalAnimationPose m_animationPose;
    mutable bool m_isAnimationPoseOutdated = true;

    float m_scale = 1.0f;
    bool m_isLooped = false;
    float m_currentTime = 0.0f;
};
