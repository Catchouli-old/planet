#ifndef PLANET_MATH_H
#define PLANET_MATH_H

/// Matrix type
/// Matrices are stored in opengl order (translation component in m[12],m[13],m[14])
/// i is rows and j is columns (ordered as below to preserve previously stated order)
typedef union mat4f {
  struct {
    struct {
      float i[4];
    } j[4];
  };
  float m[16];
} mat4f;

typedef union vec4f {
  struct {
    float x, y, z, w;
  };
  float m[16];
} vec4f;

/// Scalar approx equals
int aseq(float x, float y);

/// Vector equals
int veq(const vec4f* a, const vec4f* b);

/// Vector approx equals
int aveq(const vec4f* a, const vec4f* b);

/// Vector addition
vec4f vadd(const vec4f* a, const vec4f* b);

/// Vector negation
vec4f vneg(const vec4f* a);

/// Vector reciprocal
vec4f vrecp(const vec4f* a);

/// Vector multiplication by a scalar
vec4f vmuls(const vec4f* a, float s);

/// Vector multiplication by a vector
vec4f vmulv(const vec4f* a, const vec4f* b);

/// Matrix equality
int ameq(const mat4f* a, const mat4f* b);

// Multiplication of a column-major matrix and a vector
vec4f mmulv(const mat4f* a, const vec4f* b);

/// Multiplication of two column-major matrices
mat4f mmulm(const mat4f* a, const mat4f* b);

/// Print a matrix
void matrix_print(const mat4f* a);

#endif
