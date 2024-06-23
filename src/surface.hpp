#ifndef SURFACE_HPP_INCLUDED
#define SURFACE_HPP_INCLUDED

#include "formatting.hpp"
#include "vmath.hpp"

#include <stdexcept>
#include <string>

namespace cpu {
  struct Surface {
    Vec3s_x3 verts;
    Vec3f normal;
    float originOffset;

    int32_t platIndex;
    int32_t index;

    Surface(int32_t index) {
      this->index = index;

      reset_pos();

      normal = Vec3f(0.0f, 1.0f, 0.0f);

      originOffset = -verts[0].y();
    }

    void reset_pos() {
      using namespace std::literals;
      if (index == 0) {
        verts = Vec3s_x3 {
            Vec3s(307, 307, -306),
            Vec3s(-306, 307, -306),
            Vec3s(-306, 307, 307),
        };
      }
      else if (index == 1) {
        verts = Vec3s_x3 {
            Vec3s(307, 307, -306),
            Vec3s(-306, 307, 307),
            Vec3s(307, 307, 307),
        };
      }
      else if (index == 2) {
        verts = Vec3s_x3 {
            Vec3s(-306, 307, 307),
            Vec3s(-306, 307, -306),
            Vec3s(0, 0, 0),
        };
      }
      else if (index == 3) {
        verts = Vec3s_x3 {
            Vec3s(0, 0, 0),
            Vec3s(307, 307, 307),
            Vec3s(-306, 307, 307),
        };
      }
      else if (index == 4) {
        verts = Vec3s_x3 {
            Vec3s(307, 307, -306),
            Vec3s(0, 0, 0),
            Vec3s(-306, 307, -306),
        };
      }
      else if (index == 5) {
        verts = Vec3s_x3 {
            Vec3s(0, 0, 0),
            Vec3s(307, 307, -306),
            Vec3s(307, 307, 307),
        };
      }
      else {
        throw std::out_of_range(
            "Invalid surface index "s + std::to_string(index));
      }
    }

    void rotate(const Mat4& matrix) {
      reset_pos();

      tform_surface(verts, matrix, verts);

      normal = vec3f_normalize(vec3f_cross(
          (verts[1] - verts[0]).convert<float>(),
          (verts[2] - verts[1]).convert<float>()));

      originOffset = -vec3f_dot(normal, verts[0].convert<float>());
    }

    void repr() {
      // hacky C++20 substitute for std::print.
      std::cout << std::format(
          "vert1: {}\n"
          "vert2: {}\n"
          "vert3: {}\n",
          verts[0], verts[1], verts[2]);
    }
  };

  template <std::ranges::forward_range T> requires (std::is_same_v<std::ranges::range_value_t<T>, Surface>)
  std::pair<std::ranges::iterator_t<T>, float> find_floor(Vec3f pos, T&& surfaces) {
    Vec3s ipos = pos.convert<int32_t>().convert<int16_t>();

    int16_t x = ipos.x(), y = ipos.y(), z = ipos.z();

    auto end = std::ranges::end(surfaces);

    for (auto it = std::ranges::begin(surfaces); it != end; it++) {
      const Surface& surf = *it;

      // check if Mario is inside the horizontal projection of the triangle

      int16_t x1 = surf.verts[0].x(), z1 = surf.verts[0].z();
      int16_t x2 = surf.verts[1].x(), z2 = surf.verts[1].z();

      if ((z1 - z) * (x2 - x1) - (x1 - x) * (z2 - z1) < 0) {
        continue;
      }

      int16_t x3 = surf.verts[2].x(), z3 = surf.verts[2].z();

      if ((z2 - z) * (x3 - x2) - (x2 - x) * (z3 - z2) < 0) {
        continue;
      }
      if ((z3 - z) * (x1 - x3) - (x3 - x) * (z1 - z3) < 0) {
        continue;
      }

      // compute the floor height (I have no clue how this works)
      float oo = vec3f_dot(surf.normal, surf.verts[0].convert<float>());
      float height = -(x * surf.normal.x() + z * surf.normal.z() + oo) / surf.normal.y();
      // If Mario is under the floor, this isn't the right floor
      if (y - (height - 78.0f) < 0.0f) {
        continue;
      }

      return std::pair(it, height);
    }
    return std::pair(end, NAN);
  }
}  // namespace cpu

#endif