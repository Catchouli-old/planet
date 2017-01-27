#ifndef PLANET_MESH_H
#define PLANET_MESH_H

struct GpuMesh;

/// Load gpu mesh from an obj file
struct GpuMesh* gpu_mesh_load_obj(const char* filename);

/// Delete gpu mesh
void gpu_mesh_delete(struct GpuMesh* mesh);

/// Draw gpu mesh
void gpu_mesh_draw(struct GpuMesh* mesh);

#endif

