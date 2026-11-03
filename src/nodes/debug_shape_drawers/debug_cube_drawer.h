#pragma once

#include "nodes/debug_shape_drawers/debug_shape_drawer.h"

#include <godot_cpp/variant/quaternion.hpp>

class DebugCubeDrawer : public DebugShapeDrawer {
	GDCLASS(DebugCubeDrawer, DebugShapeDrawer)

	MultiMeshBatch lines;
	MultiMeshBatch fill;

	static MeshData _create_lines_cube();
	static MeshData _create_fill_cube();
	static void _create_fill_face(MeshData& r_mesh_data, int p_face_index, int32_t p_bottom_left, int32_t p_bottom_right, int32_t p_top_left, int32_t p_top_right);

protected:
	static void _bind_methods() {}

public:
	void _enter_tree() override;

	void draw(const Vector3& p_center, const Vector3& p_size, const Quaternion& p_rotation, const Color& p_line_color, const Color& p_fill_color);
	void finalize();
	void reset();
};
