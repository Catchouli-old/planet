#ifndef PLANET_MESH_H
#define PLANET_MESH_H

struct GpuMesh;

/// Create a new indexed gpu mesh
struct GpuMesh* gpu_mesh_new_indexed(float* pos, int pos_stride,
  float* nrm, int nrm_stride, int vertex_count, int* idx, int idx_count);

/// Load gpu mesh from an obj file
struct GpuMesh* gpu_mesh_load_obj(const char* filename);

#endif

