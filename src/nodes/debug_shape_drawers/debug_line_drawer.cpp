#include "nodes/debug_shape_drawers/debug_line_drawer.h"

#include <godot_cpp/variant/basis.hpp>

void DebugLineDrawer::_enter_tree() {
	DebugShapeDrawer::_enter_tree();

	if (lines.multimesh.is_null()) {
		lines = _create_batch(_create_array_mesh(_create_lines_line(), Mesh::PRIMITIVE_LINES, _create_lines_material()));
	}
}

void DebugLineDrawer::draw(const Vector3& p_start, const Vector3& p_end, const Color& p_line_color) {
	if (p_line_color.a <= 0.0f || p_start == p_end) {
		return;
	}

	const Vector3 offset = p_end - p_start;
	const Transform3D instance_transform(Basis(offset, Vector3(), Vector3()), p_start);

	_push_instance(lines, instance_transform, p_line_color);
}

void DebugLineDrawer::finalize() {
	_finalize_batch(lines);
}

void DebugLineDrawer::reset() {
	lines.count = 0;
}

MeshData DebugLineDrawer::_create_lines_line() {
	MeshData mesh_data;
	mesh_data.resize_for_vertices(2);
	mesh_data.resize_for_lines(1);

	mesh_data.set_vertex(0, Vector3(0.0, 0.0, 0.0));
	mesh_data.set_vertex(1, Vector3(1.0, 0.0, 0.0));
	mesh_data.set_line_indices(0, 0, 1);

	return mesh_data;
}
