#include "mesh.h"
#include "stretchy_buffer.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <GL/glew.h>
#include <stdint.h>

struct GpuMesh {
  // Vertex buffer object
  GLuint vbuf;
  // Stride for vertex buffer
  int stride;
  // Offset of position start in vertex buffer in bytes (-1 if not available in buffer)
  uintptr_t pos_offset;
  // Offset of normal start in vertex buffer in bytes (-1 if not available in buffer)
  uintptr_t nrm_offset;
  // Index buffer object
  GLuint ibuf;
  // Index count
  int idx_count;
};

struct GpuMesh* gpu_mesh_load_obj(const char* filename) {
  FILE* fd = fopen(filename, "rb");
  if (fd == NULL) {
    fprintf(stderr, "Failed to open file %s\n", filename);
    return NULL;
  }

  // Create stretchy buffers
  float* positions = NULL;
  float* normals = NULL;
  int* indices = NULL;

  // A very basic obj parser that is probably not correct for many reasons
  // Needs to suport more formats for faces
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
      // Convert to 0-indexed
      sb_push(indices, vi-1);
      sb_push(indices, vj-1);
      sb_push(indices, vk-1);
    }
    else {
      printf("Unrecognised obj entry: %s", buf);
    }
  }
  fclose(fd);

  const int pos_count = sb_count(positions);
  const int nrm_count = sb_count(normals);
  const int idx_count = sb_count(indices);

  // Validate input to the best of my understanding / requirements
  // It will probably crash if these assertions fail :)
  assert(pos_count % 3 == 0);
  assert(nrm_count % 3 == 0);
  assert(idx_count % 3 == 0);
  assert(nrm_count == 0 || nrm_count == pos_count);

  // Some reassuring output
  printf("Finished parsing obj file:\n");
  printf("Positions: %d\n", pos_count/3);
  printf("Normals: %d\n", nrm_count/3);
  printf("Indices: %d\n", idx_count);

  // Build vertex buffer
  const int buf_size = sizeof(float) * (pos_count + nrm_count);
  const int buf_stride = (pos_count > 0 ? 3 : 0) + (nrm_count > 0 ? 3 : 0);
  float* const vertex_buffer = malloc(buf_size);
  float* ptr = vertex_buffer;
  for (int i = 0; i < pos_count; ++i) {
    if (pos_count > 0)
      *(ptr++) = positions[i];
    if (nrm_count > 0)
      *(ptr++) = normals[i];
  }

  // Construct gpu mesh
  struct GpuMesh* mesh = malloc(sizeof(struct GpuMesh));
  memset(mesh, 0, sizeof(struct GpuMesh));

  // Set data
  mesh->stride = buf_stride;
  mesh->idx_count = idx_count;
  mesh->pos_offset = (pos_count > 0 ? 0 : -1);
  mesh->nrm_offset = (nrm_count > 0 ? mesh->pos_offset + 3*sizeof(float) : -1);

  // Allocate opengl buffers
  glGenBuffers(1, &mesh->vbuf);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbuf);
  glBufferData(GL_ARRAY_BUFFER, buf_size, vertex_buffer, GL_STATIC_DRAW);

  glGenBuffers(1, &mesh->ibuf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibuf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx_count * sizeof(float), indices, GL_STATIC_DRAW);

  // Free buffers
  free(vertex_buffer);
  sb_free(positions);
  sb_free(normals);
  sb_free(indices);

  return mesh;
}

void gpu_mesh_delete(struct GpuMesh* mesh) {
  glDeleteBuffers(1, &mesh->vbuf);
  glDeleteBuffers(1, &mesh->ibuf);
  free(mesh);
}

void gpu_mesh_draw(struct GpuMesh* mesh) {
  // todo: convert to use shader locations
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbuf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibuf);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->stride, (void*)mesh->pos_offset);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh->stride, (void*)mesh->nrm_offset);
  glDrawElements(GL_TRIANGLES, mesh->idx_count/3, GL_UNSIGNED_INT, 0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
}
