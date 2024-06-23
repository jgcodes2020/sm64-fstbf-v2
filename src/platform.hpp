#ifndef PLATFORM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED

#include "surface.hpp"
#include "vmath.hpp"

namespace cpu {
  struct Platform {
    Vec3f pos = {0.0f, 0.0f, 0.0f};
    Vec3f normal = {0.0f, 1.0f, 0.0f};
    Mat4 transform = {{}};

    bool oTiltingPyramidMarioOnPlatform = false;
    bool onPlatform = false;
    std::array<Surface, 2> floors = {Surface(0), Surface(1)};
    std::array<Surface, 4> ceilings = {Surface(2), Surface(3), Surface(4), Surface(5)};

    Platform() {}

    Platform(Vec3s pos, Vec3f normal) {
      this->pos = pos.convert<float>();
      this->normal = normal;

      create_transform_from_normals();

      for (Surface& surf : floors)
        surf.rotate(transform);
      for (Surface& surf : ceilings)
        surf.rotate(transform);
    }

    void create_transform_from_normals();
    void platform_logic(Vec3f& marioPos);
  };
}  // namespace cpu

#endif