#pragma once

#include "classes/mesh_data.h"
#include "nodes/debug_shape_drawers/debug_shape_drawer.h"

#include <godot_cpp/classes/standard_material3d.hpp>

#include <unordered_map>

class DebugSphereDrawer : public DebugShapeDrawer {
	GDCLASS(DebugSphereDrawer, DebugShapeDrawer)

	Ref<StandardMaterial3D> lines_material;
	ResolutionBatches lines;

	Ref<StandardMaterial3D> fill_material;
	ResolutionBatches fill;

	MultiMeshBatch& _get_lines_batch(int32_t p_resolution);
	MultiMeshBatch& _get_fill_batch(int32_t p_resolution);

	static MeshData _create_ico_sphere(int32_t p_resolution);
	static MeshData _create_icosahedron();
	static MeshData _subdivide(MeshData& r_mesh_data);
	static int32_t _get_midpoint_index(MeshData& r_mesh_data, std::unordered_map<int64_t, int32_t>& r_added_midpoints, int32_t p_a, int32_t p_b);
	static int64_t _get_edge_key(int32_t p_a, int32_t p_b);

protected:
	static void _bind_methods() {}

public:
	void draw(const Vector3& p_center, real_t p_radius, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color);
	void finalize();
	void reset();
};
