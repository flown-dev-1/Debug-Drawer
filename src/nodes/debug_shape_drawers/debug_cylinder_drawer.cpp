#include "nodes/debug_shape_drawers/debug_cylinder_drawer.h"
#include "classes/debug_drawer_resources.h"

#include "shaders/cylinder_fill.h"
#include "shaders/cylinder_lines.h"

#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/basis.hpp>

#include <algorithm>

void DebugCylinderDrawer::draw(const Vector3& p_center, real_t p_height, real_t p_top_radius, real_t p_bottom_radius, const Quaternion& p_rotation, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color) {
	if (p_height == 0.0f || (p_top_radius == 0.0f && p_bottom_radius == 0.0f)) {
		return;
	}

	const int32_t resolution = std::max(p_resolution, MIN_RESOLUTION);

	const Transform3D instance_transform(Basis(p_rotation).scaled_local(Vector3(1.0, p_height, 1.0)), p_center);
	const Color instance_custom_data(p_top_radius, p_bottom_radius, 0.0f, 0.0f);

	if (p_line_color.a > 0.0f) {
		push_instance(get_lines_batch(resolution), instance_transform, p_line_color, instance_custom_data);
	}

	if (p_fill_color.a > 0.0f) {
		push_instance(get_fill_batch(resolution), instance_transform, p_fill_color, instance_custom_data);
	}
}

void DebugCylinderDrawer::finalize() {
	finalize_batches(lines);
	finalize_batches(fill);
}

void DebugCylinderDrawer::reset() {
	reset_batches(lines);
	reset_batches(fill);
}

MultiMeshBatch& DebugCylinderDrawer::get_lines_batch(int32_t p_resolution) {
	auto it = lines.find(p_resolution);
	if (it != lines.end()) {
		return it->second;
	}

	if (lines_material.is_null()) {
		lines_material = DebugDrawerResources::create_shader_material(DebugDrawerShaders::CYLINDER_LINES);
	}

	Ref<ArrayMesh> mesh = DebugDrawerResources::create_array_mesh(create_lines_cylinder(p_resolution), Mesh::PRIMITIVE_LINES, lines_material);
	return lines.emplace(p_resolution, create_batch(mesh, true)).first->second;
}

MultiMeshBatch& DebugCylinderDrawer::get_fill_batch(int32_t p_resolution) {
	auto it = fill.find(p_resolution);
	if (it != fill.end()) {
		return it->second;
	}

	if (fill_material.is_null()) {
		fill_material = DebugDrawerResources::create_shader_material(DebugDrawerShaders::CYLINDER_FILL);
	}

	Ref<ArrayMesh> mesh = DebugDrawerResources::create_array_mesh(create_fill_cylinder(p_resolution), Mesh::PRIMITIVE_TRIANGLES, fill_material);
	return fill.emplace(p_resolution, create_batch(mesh, true)).first->second;
}

MeshData DebugCylinderDrawer::create_lines_cylinder(int32_t p_resolution) {
	MeshData mesh_data;
	mesh_data.resize_for_vertices(p_resolution * 2);
	mesh_data.resize_for_lines(p_resolution * 3);

	set_circle_vertices(mesh_data, 0, p_resolution, Vector3(0.0, -0.5, 0.0), 1.0);
	set_circle_vertices(mesh_data, p_resolution, p_resolution, Vector3(0.0, 0.5, 0.0), 1.0);

	for (int32_t segment = 0; segment < p_resolution; segment++) {
		const int32_t top_index = get_top_vertex_index(p_resolution, segment);
		const int32_t next_top_index = get_top_vertex_index(p_resolution, segment + 1);
		const int32_t bottom_index = get_bottom_vertex_index(p_resolution, segment);
		const int32_t next_bottom_index = get_bottom_vertex_index(p_resolution, segment + 1);

		const int32_t line_index = segment * 3;

		mesh_data.set_line_indices(line_index + 0, top_index, next_top_index);
		mesh_data.set_line_indices(line_index + 1, bottom_index, next_bottom_index);
		mesh_data.set_line_indices(line_index + 2, top_index, bottom_index);
	}

	return mesh_data;
}

MeshData DebugCylinderDrawer::create_fill_cylinder(int32_t p_resolution) {
	MeshData mesh_data;
	mesh_data.resize_for_vertices(p_resolution * 2 + 2);
	mesh_data.resize_for_triangles(p_resolution * 4);

	set_circle_vertices(mesh_data, 0, p_resolution, Vector3(0.0, -0.5, 0.0), 1.0);
	set_circle_vertices(mesh_data, p_resolution, p_resolution, Vector3(0.0, 0.5, 0.0), 1.0);

	const int32_t top_center_index = p_resolution * 2;
	const int32_t bottom_center_index = p_resolution * 2 + 1;

	mesh_data.set_vertex(top_center_index, Vector3(0.0, 0.5, 0.0));
	mesh_data.set_vertex(bottom_center_index, Vector3(0.0, -0.5, 0.0));

	for (int32_t segment = 0; segment < p_resolution; segment++) {
		const int32_t top_index = get_top_vertex_index(p_resolution, segment);
		const int32_t next_top_index = get_top_vertex_index(p_resolution, segment + 1);
		const int32_t bottom_index = get_bottom_vertex_index(p_resolution, segment);
		const int32_t next_bottom_index = get_bottom_vertex_index(p_resolution, segment + 1);

		const int32_t triangle_index = segment * 4;

		mesh_data.set_triangle_indices(triangle_index + 0, top_center_index, next_top_index, top_index);
		mesh_data.set_triangle_indices(triangle_index + 1, bottom_center_index, bottom_index, next_bottom_index);
		mesh_data.set_triangle_indices(triangle_index + 2, top_index, next_top_index, next_bottom_index);
		mesh_data.set_triangle_indices(triangle_index + 3, top_index, next_bottom_index, bottom_index);
	}

	return mesh_data;
}

void DebugCylinderDrawer::set_circle_vertices(MeshData& r_mesh_data, int32_t p_index_offset, int32_t p_resolution, const Vector3& p_offset, real_t p_radius) {
	const double step = Math::TAU / p_resolution;

	for (int32_t segment = 0; segment < p_resolution; segment++) {
		const double angle = segment * step;
		const real_t x = static_cast<real_t>(Math::cos(angle));
		const real_t z = static_cast<real_t>(Math::sin(angle));

		r_mesh_data.set_vertex(p_index_offset + segment, p_offset + Vector3(x, 0.0, z) * p_radius);
	}
}

int32_t DebugCylinderDrawer::get_top_vertex_index(int32_t p_resolution, int32_t p_segment) {
	return p_resolution + static_cast<int32_t>(Math::posmod(p_segment, p_resolution));
}

int32_t DebugCylinderDrawer::get_bottom_vertex_index(int32_t p_resolution, int32_t p_segment) {
	return static_cast<int32_t>(Math::posmod(p_segment, p_resolution));
}
