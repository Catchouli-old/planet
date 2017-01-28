#include "maths.h"
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdio.h>

int aseq(float x, float y) {
  return fabs(x-y) < FLT_EPSILON;
}

int veq(const vec4f* a, const vec4f* b) {
  return a->x == b->x
      && a->y == b->y
      && a->z == b->z
      && a->w == b->w;
}

int aveq(const vec4f* a, const vec4f* b) {
  return aseq(a->x, b->x)
      && aseq(a->y, b->y)
      && aseq(a->z, b->z)
      && aseq(a->w, b->w);
}

vec4f vadd(const vec4f* a, const vec4f* b) {
  vec4f res = {a->x+b->x,a->y+b->y,a->z+b->z,a->w+b->w};
  return res;
}

vec4f vneg(const vec4f* a) {
  vec4f res = {-a->x,-a->y,-a->z,-a->w};
  return res;
}

vec4f vrecp(const vec4f* a) {
  vec4f res = {1.0f/a->x,1.0f/a->y,1.0f/a->z,1.0f/a->w};
  return res;
}

vec4f vmuls(const vec4f* a, float s) {
  vec4f res = {s*a->x,s*a->y,s*a->z,s*a->w};
  return res;
}

vec4f vmulv(const vec4f* a, const vec4f* b) {
  vec4f res = {a->x*b->x,a->y*b->y,a->z*b->z,a->w*b->w};
  return res;
}

int ameq(const mat4f* a, const mat4f* b) {
  return aseq(a->m[0], b->m[0])
      && aseq(a->m[1], b->m[1])
      && aseq(a->m[2], b->m[2])
      && aseq(a->m[3], b->m[3])
      && aseq(a->m[4], b->m[4])
      && aseq(a->m[5], b->m[5])
      && aseq(a->m[6], b->m[6])
      && aseq(a->m[7], b->m[7])
      && aseq(a->m[8], b->m[8])
      && aseq(a->m[9], b->m[9])
      && aseq(a->m[10], b->m[10])
      && aseq(a->m[11], b->m[11])
      && aseq(a->m[12], b->m[12])
      && aseq(a->m[13], b->m[13])
      && aseq(a->m[14], b->m[14])
      && aseq(a->m[15], b->m[15]);
}

vec4f mmulv(const mat4f* a, const vec4f* b) {
  vec4f res = {0};
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      res.m[i] += a->j[j].i[i] * b->m[j];
    }
  }
  return res;
}

mat4f mmulm(const mat4f* a, const mat4f* b) {
  mat4f res = {0};
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < 4; ++k) {
        res.j[j].i[i] += a->j[k].i[i] * b->j[j].i[k];
      }
    }
  }
  return res;
}

void matrix_print(const mat4f* a) {
  printf("Matrix:\n");
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      printf("%f ", a->j[j].i[i]);
    }
    printf("\n");
  }
  printf("In memory: ");
  for (int i = 0; i < 16; ++i)
    printf("%f ", a->m[i]);
  printf("\n");
  printf("Translation: %f %f %f\n", a->j[3].i[0], a->j[3].i[1], a->j[3].i[2]);
  printf("Translation: %f %f %f\n", a->m[12], a->m[13], a->m[14]);
}
