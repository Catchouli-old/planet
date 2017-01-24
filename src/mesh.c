#include "mesh.h"
#include "stretchy_buffer.h"
#include <stdio.h>
#include <assert.h>

struct GpuMesh* gpu_mesh_new_indexed(float* pos, int pos_stride,
  float* nrm, int nrm_stride, int vertex_count, int* idx, int idx_count)
{
  return NULL;
}

struct GpuMesh* gpu_mesh_load_obj(const char* filename) {
  FILE* fd = fopen(filename, "rb");
  if (fd == NULL) {
    fprintf(stderr, "Failed to open file %s\n", filename);
    return NULL;
  }

  float* positions = NULL;
  float* normals = NULL;
  int* indices = NULL;

  const size_t len = 300;
  char buf[len];
  while (fgets(buf, len, fd) != NULL) {
    float x, y, z;
    float vi, ni, ti, vj, nj, tj, vk, nk, tk;
    // TODO: add more formats (especially for faces)
    if (sscanf(buf, "v %f %f %f", &x, &y, &z) == 3) {
      sb_push(positions, x);
      sb_push(positions, y);
      sb_push(positions, z);
    }
    else if (sscanf(buf, "vn %f %f %f", &x, &y, &z) == 3) {
      sb_push(normals, x);
      sb_push(normals, y);
      sb_push(normals, z);
    }
    else if (sscanf(buf, "f %f//%f %f//%f %f//%f", &vi, &ni, &vj, &nj, &vk, &nk) == 6) {
      assert(vi == ni && vj == nj && vk == nk);
      sb_push(indices, vi);
      sb_push(indices, vj);
      sb_push(indices, vk);
    }
    else {
      printf("Unrecognised obj entry: %s", buf);
    }
  }

  assert(sb_count(positions) % 3 == 0);
  assert(sb_count(normals) % 3 == 0);
  assert(sb_count(indices) % 3 == 0);
  assert(sb_count(normals) == 0 || sb_count(normals) == sb_count(positions));

  printf("Finished parsing obj file:\n");
  printf("Positions: %d\n", sb_count(positions)/3);
  printf("Normals: %d\n", sb_count(normals)/3);
  printf("Indices: %d\n", sb_count(indices));

  struct GpuMesh* mesh = gpu_mesh_new_indexed(positions, 3 * sizeof(float),
                           normals, 3 * sizeof(float), indices, sb_count(indices));

  sb_free(positions);
  sb_free(normals);
  sb_free(indices);

  fclose(fd);
  return mesh;
}
