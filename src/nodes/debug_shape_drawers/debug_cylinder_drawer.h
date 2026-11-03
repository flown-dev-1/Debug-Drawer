#pragma once

#include "classes/mesh_data.h"
#include "nodes/debug_shape_drawers/debug_shape_drawer.h"

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/variant/quaternion.hpp>

class DebugCylinderDrawer : public DebugShapeDrawer {
	GDCLASS(DebugCylinderDrawer, DebugShapeDrawer)

	static constexpr int32_t MIN_RESOLUTION = 3;

	Ref<ShaderMaterial> lines_material;
	ResolutionBatches lines;

	Ref<ShaderMaterial> fill_material;
	ResolutionBatches fill;

	MultiMeshBatch& get_lines_batch(int32_t p_resolution);
	MultiMeshBatch& get_fill_batch(int32_t p_resolution);

	static MeshData create_lines_cylinder(int32_t p_resolution);
	static MeshData create_fill_cylinder(int32_t p_resolution);
	static void set_circle_vertices(MeshData& r_mesh_data, int32_t p_index_offset, int32_t p_resolution, const Vector3& p_offset, real_t p_radius);
	static int32_t get_top_vertex_index(int32_t p_resolution, int32_t p_segment);
	static int32_t get_bottom_vertex_index(int32_t p_resolution, int32_t p_segment);

protected:
	static void _bind_methods() {}

public:
	void draw(const Vector3& p_center, real_t p_height, real_t p_top_radius, real_t p_bottom_radius, const Quaternion& p_rotation, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color);
	void finalize();
	void reset();
};
