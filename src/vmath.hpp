#ifndef VMATH_HPP_INCLUDED
#define VMATH_HPP_INCLUDED

// File defining game-accurate math functionality.

#include <sycl/sycl.hpp>
#include <array>

#include "trig.hpp"

using Vec2f = sycl::vec<float, 2>;
using Vec3f = sycl::vec<float, 3>;
using Vec4f = sycl::vec<float, 4>;

using Vec3s = sycl::vec<int16_t, 3>;
using Vec3i = sycl::vec<int32_t, 3>;
using Vec4s = sycl::vec<int16_t, 4>;

using Vec3f_x3 = std::array<Vec3f, 3>;
using Vec3s_x3 = std::array<Vec3s, 3>;
using Mat4 = std::array<Vec4f, 4>;

// Computes cross product without using FMA. This should be fully compatible with SM64's implementation.
// algorithm from here: https://geometrian.com/programming/tutorials/cross-product/index.php.
// all orders should be equivalent.
inline Vec3f vec3f_cross(Vec3f a, Vec3f b) {
  using namespace sycl;

  Vec3f tmp0 = a.swizzle<elem::y, elem::z, elem::x>();
  Vec3f tmp1 = b.swizzle<elem::z, elem::x, elem::y>();

  Vec3f tmp2 = tmp0 * b;
  Vec3f tmp3 = tmp0 * tmp1;
  Vec3f tmp4 = tmp3.swizzle<elem::y, elem::z, elem::x>();

  return tmp3 - tmp4;
}

inline float vec3f_dot(Vec3f a, Vec3f b) {
  Vec3f parts = a * b;
  return parts.x() + parts.y() + parts.z();
}

// Normalizes a vector without using FMA.
inline Vec3f vec3f_normalize(Vec3f a) {
  using namespace sycl;
  Vec3f a2 = a * a;
  float ax = a2.x(), ay = a2.y(), az = a2.z();

  float inv_mag = 1.0f / sycl::sqrt(ax + ay + az);

  return a * inv_mag;
}

inline float vec3f_magnitude(Vec3f a) {
  Vec3f a2 = a * a;
  float ax = a2.x(), ay = a2.y(), az = a2.z();

  return sycl::sqrt(ax + ay + az);
}

inline float euclidean_distance_squared(Vec3f a, Vec3f b) {
  using namespace sycl;

  // = a.xz - b.xz
  Vec2f diff = Vec2f(
    a.swizzle<elem::x, elem::z>() - b.swizzle<elem::x, elem::z>()
  );
  // dot itself
  diff = diff * diff;
  return diff.x() + diff.y();
}

inline float euclidean_distance(Vec3f a, Vec3f b) {
  return sqrtf(euclidean_distance_squared(a, b));
}

// Sets `dest` to a transformation matrix that aligns an object with the terrain based on the normal.
inline void mtxf_align_terrain_normal(Mat4& dest, Vec3f upDir, Vec3f pos, int16_t yaw) {
  upDir = vec3f_normalize(upDir);
  Vec3f facingDir = Vec3f(cpu::sins(yaw), 0.0f, cpu::coss(yaw));

  Vec3f leftDir = vec3f_normalize(vec3f_cross(upDir, facingDir));
  Vec3f forwardDir = vec3f_normalize(vec3f_cross(leftDir, upDir));
  
  dest[0] = Vec4f(leftDir, 0.0f);
  dest[1] = Vec4f(upDir, 0.0f);
  dest[2] = Vec4f(forwardDir, 0.0f);
  dest[3] = Vec4f(pos, 1.0f);
}
inline void create_transform_from_normal(Vec3f normal, Vec3f position, Mat4& mat) {
  mtxf_align_terrain_normal(mat, normal, position, 0);
}

// Transforms a point according to an affine transformation matrix, truncating its 
// coordinates to the nearest `short` afterwards.
inline Vec3s tform_point(Vec3s point, const Mat4& transform) {
  using namespace sycl;
  Vec3f fp_point = point.convert<float>();
  Vec3f fp_res = 
    transform[0].swizzle<elem::x, elem::y, elem::z>() * fp_point.x() + 
    transform[1].swizzle<elem::x, elem::y, elem::z>() * fp_point.y() +
    transform[2].swizzle<elem::x, elem::y, elem::z>() * fp_point.z() +
    transform[3].swizzle<elem::x, elem::y, elem::z>();

  return fp_res.convert<int32_t>().convert<int16_t>();
}

// Multiplies a point by a transformation matrix, assuming that it contains
// a linear transformation in R^3; i.e. the matrix looks like
// | a b c 0 |
// | d e f 0 |
// | p q r 0 |
// | 0 0 0 1 |
inline Vec3f linear_mtxf_mul_vec3f(const Mat4& transform, Vec3f point) {
  using namespace sycl;
  Vec3f result = 
    transform[0].swizzle<elem::x, elem::y, elem::z>() * point.x() + 
    transform[1].swizzle<elem::x, elem::y, elem::z>() * point.y() +
    transform[2].swizzle<elem::x, elem::y, elem::z>() * point.z();
  
  return result;
}

// Transforms a set of 3 vertices according to SM64's geometry rules.
// See `tform_point` for details.
inline void tform_surface(Vec3s_x3& dest, const Mat4& transform, const Vec3s_x3& src) {
  for (int i = 0; i < 3; i++) {
    dest[i] = tform_point(src[i], transform);
  }
}

#endif