#include "platform.hpp"

static float approach_by_increment(float goal, float src, float inc) {
  float newVal;

  if (src <= goal) {
    if (goal - src < inc) {
      newVal = goal;
    }
    else {
      newVal = src + inc;
    }
  }
  else if (goal - src > -inc) {
    newVal = goal;
  }
  else {
    newVal = src - inc;
  }

  return newVal;
}

void cpu::Platform::create_transform_from_normals() {
  mtxf_align_terrain_normal(transform, normal, pos, 0);
}

void cpu::Platform::platform_logic(Vec3f& marioPos) {
  Vec3f dr;
  float d;

  Vec3f dist;
  Vec3f posBeforeRotation;
  Vec3f posAfterRotation;

  int marioOnPlatform = 0;

  create_transform_from_normals();

  if (onPlatform) {
    dist = marioPos - pos.convert<float>();

    posBeforeRotation = linear_mtxf_mul_vec3f(transform, dist);

    dr = Vec3f(dist.x(), 500.0f, dist.z());
    d  = vec3f_magnitude(dr);

    d = 1.0 / d;
    dr *= d;

    if (oTiltingPyramidMarioOnPlatform)
      marioOnPlatform++;

    oTiltingPyramidMarioOnPlatform = true;
  }
  else {
    dr                             = Vec3f(0.0f, 1.0f, 0.0f);
    oTiltingPyramidMarioOnPlatform = false;
  }

  normal.x() = approach_by_increment(dr.x(), normal.x(), 0.01f);
  normal.y() = approach_by_increment(dr.y(), normal.y(), 0.01f);
  normal.z() = approach_by_increment(dr.z(), normal.z(), 0.01f);
  create_transform_from_normals();

  for (Surface& surf : floors)
    surf.rotate(transform);
  for (Surface& surf : ceilings)
    surf.rotate(transform);

  if (marioOnPlatform) {
    posAfterRotation = linear_mtxf_mul_vec3f(transform, dist);
    marioPos += posAfterRotation - posBeforeRotation;
  }

  const auto [floor, floor_height] = find_floor(marioPos, floors);
  onPlatform = floor != floors.end() && fabsf(marioPos[1] - floor_height) <= 4.0;
}