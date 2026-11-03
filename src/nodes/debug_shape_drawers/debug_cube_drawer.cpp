#include "nodes/debug_shape_drawers/debug_cube_drawer.h"

#include <godot_cpp/variant/basis.hpp>

void DebugCubeDrawer::_enter_tree() {
	DebugShapeDrawer::_enter_tree();

	if (lines.multimesh.is_null()) {
		lines = _create_batch(_create_array_mesh(_create_lines_cube(), Mesh::PRIMITIVE_LINES, _create_lines_material()));
	}

	if (fill.multimesh.is_null()) {
		fill = _create_batch(_create_array_mesh(_create_fill_cube(), Mesh::PRIMITIVE_TRIANGLES, _create_fill_material()));
	}
}

void DebugCubeDrawer::draw(const Vector3& p_center, const Vector3& p_size, const Quaternion& p_rotation, const Color& p_line_color, const Color& p_fill_color) {
	if (p_size == Vector3()) {
		return;
	}

	const Transform3D instance_transform(Basis(p_rotation).scaled_local(p_size), p_center);

	if (p_line_color.a > 0.0f) {
		_push_instance(lines, instance_transform, p_line_color);
	}

	if (p_fill_color.a > 0.0f) {
		_push_instance(fill, instance_transform, p_fill_color);
	}
}

void DebugCubeDrawer::finalize() {
	_finalize_batch(lines);
	_finalize_batch(fill);
}

void DebugCubeDrawer::reset() {
	lines.count = 0;
	fill.count = 0;
}

static void set_cube_vertices(MeshData& r_mesh_data) {
	r_mesh_data.resize_for_vertices(8);
	r_mesh_data.set_vertex(0, Vector3(-0.5, -0.5, -0.5));
	r_mesh_data.set_vertex(1, Vector3(0.5, -0.5, -0.5));
	r_mesh_data.set_vertex(2, Vector3(-0.5, 0.5, -0.5));
	r_mesh_data.set_vertex(3, Vector3(0.5, 0.5, -0.5));
	r_mesh_data.set_vertex(4, Vector3(-0.5, -0.5, 0.5));
	r_mesh_data.set_vertex(5, Vector3(0.5, -0.5, 0.5));
	r_mesh_data.set_vertex(6, Vector3(-0.5, 0.5, 0.5));
	r_mesh_data.set_vertex(7, Vector3(0.5, 0.5, 0.5));
}

MeshData DebugCubeDrawer::_create_lines_cube() {
	MeshData mesh_data;
	set_cube_vertices(mesh_data);
	mesh_data.resize_for_lines(12);

	mesh_data.set_line_indices(0, 0, 1);
	mesh_data.set_line_indices(1, 0, 2);
	mesh_data.set_line_indices(2, 1, 3);
	mesh_data.set_line_indices(3, 2, 3);
	mesh_data.set_line_indices(4, 4, 5);
	mesh_data.set_line_indices(5, 4, 6);
	mesh_data.set_line_indices(6, 5, 7);
	mesh_data.set_line_indices(7, 6, 7);
	mesh_data.set_line_indices(8, 0, 4);
	mesh_data.set_line_indices(9, 1, 5);
	mesh_data.set_line_indices(10, 2, 6);
	mesh_data.set_line_indices(11, 3, 7);

	return mesh_data;
}

MeshData DebugCubeDrawer::_create_fill_cube() {
	MeshData mesh_data;
	set_cube_vertices(mesh_data);
	mesh_data.resize_for_triangles(6 * 2);

	_create_fill_face(mesh_data, 0, 0, 1, 2, 3);
	_create_fill_face(mesh_data, 1, 5, 4, 7, 6);
	_create_fill_face(mesh_data, 2, 4, 0, 6, 2);
	_create_fill_face(mesh_data, 3, 1, 5, 3, 7);
	_create_fill_face(mesh_data, 4, 2, 3, 6, 7);
	_create_fill_face(mesh_data, 5, 1, 0, 5, 4);

	return mesh_data;
}

void DebugCubeDrawer::_create_fill_face(MeshData& r_mesh_data, int p_face_index, int32_t p_bottom_left, int32_t p_bottom_right, int32_t p_top_left, int32_t p_top_right) {
	r_mesh_data.set_triangle_indices(p_face_index * 2 + 0, p_bottom_left, p_bottom_right, p_top_right);
	r_mesh_data.set_triangle_indices(p_face_index * 2 + 1, p_bottom_left, p_top_right, p_top_left);
}
