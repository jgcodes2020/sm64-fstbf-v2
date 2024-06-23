#ifndef TRIG_HPP
#define TRIG_HPP

#include <cstdint>

extern float gSineTable[4096];
extern float gCosineTable[4096];
extern int gArctanTable[8192];

namespace cpu {
  // Computes the sine of the provided angle.
  inline float sins(int16_t yaw) {
    return gSineTable[uint16_t(yaw) >> 4];
  }
  // Computes the cosine of the provided angle.
  inline float coss(int16_t yaw) {
    return gCosineTable[uint16_t(yaw) >> 4];
  }

  inline int16_t atan2_lookup(float z, float x) {
    return (x == 0) ? gArctanTable[0] :
                      gArctanTable[uint16_t(float((z / x) * 1024.0 + 0.5))];
  }
  inline int16_t atan2s(float z, float x) {
    int16_t angle = 0;

    if (x >= 0) {
      if (z >= 0) {
        if (z >= x) {
          angle = atan2_lookup(x, z);
        }
        else {
          angle = 0x4000 - atan2_lookup(z, x);
        }
      }
      else {
        z = -z;

        if (z < x) {
          angle = 0x4000 + atan2_lookup(z, x);
        }
        else {
          angle = 0x8000 - atan2_lookup(x, z);
        }
      }
    }
    else {
      x = -x;

      if (z < 0) {
        z = -z;

        if (z >= x) {
          angle = 0x8000 + atan2_lookup(x, z);
        }
        else {
          angle = 0xC000 - atan2_lookup(z, x);
        }
      }
      else {
        if (z < x) {
          angle = 0xC000 + atan2_lookup(z, x);
        }
        else {
          angle = -atan2_lookup(x, z);
        }
      }
    }

    return angle;
  }
}  // namespace cpu
#endif