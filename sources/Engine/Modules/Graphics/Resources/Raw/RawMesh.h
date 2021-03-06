#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <string>

#include "Modules/ResourceManagement/RawDataStructures.h"
#include "Modules/Math/geometry.h"

constexpr uint16_t MESH_FORMAT_VERSION = 112;

enum class RawMeshAttributes {
  Empty = 0,
  Positions = 1,
  Normals = 2,
  UV = 4,
  Tangents = 8,
  BonesIDs = 16,
  BonesWeights = 32,
};

inline RawMeshAttributes operator|(RawMeshAttributes a, RawMeshAttributes b)
{
  return static_cast<RawMeshAttributes>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

inline RawMeshAttributes operator&(RawMeshAttributes a, RawMeshAttributes b)
{
  return static_cast<RawMeshAttributes>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

struct RawMeshHeader {
  uint16_t formatVersion;
  uint16_t verticesCount;
  uint16_t indicesCount;

  uint16_t subMeshesIndicesOffsetsCount;
  bitmask64 storedAttributesMask;
};

struct RawMesh {
  RawMeshHeader header;

  std::vector<RawVector3> positions;
  std::vector<RawVector3> normals;
  std::vector<RawVector3> tangents;
  std::vector<RawVector2> uv;
  std::vector<RawU8Vector4> bonesIds;
  std::vector<RawU8Vector4> bonesWeights;

  std::vector<uint16_t> indices;
  std::vector<uint16_t> subMeshesIndicesOffsets;

  AABB aabb;

  static RawMesh readFromFile(const std::string& path);
  static void writeToFile(const std::string& path, const RawMesh& rawMesh);
};
