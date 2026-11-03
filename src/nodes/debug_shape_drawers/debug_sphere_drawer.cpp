#include "nodes/debug_shape_drawers/debug_sphere_drawer.h"
#include "classes/debug_drawer_resources.h"
#include "nodes/debug_drawer.h"

#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/basis.hpp>

#include <algorithm>
#include <unordered_set>

void DebugSphereDrawer::draw(const Vector3& p_center, real_t p_radius, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color) {
	if (p_radius == 0.0f) {
		return;
	}

	const int32_t resolution = std::clamp<int32_t>(p_resolution, DebugDrawer::LOWEST, DebugDrawer::HIGHEST);

	const Transform3D instance_transform(Basis::from_scale(Vector3(1.0, 1.0, 1.0) * p_radius), p_center);

	if (p_line_color.a > 0.0f) {
		push_instance(get_lines_batch(resolution), instance_transform, p_line_color);
	}

	if (p_fill_color.a > 0.0f) {
		push_instance(get_fill_batch(resolution), instance_transform, p_fill_color);
	}
}

void DebugSphereDrawer::finalize() {
	finalize_batches(lines);
	finalize_batches(fill);
}

void DebugSphereDrawer::reset() {
	reset_batches(lines);
	reset_batches(fill);
}

MultiMeshBatch& DebugSphereDrawer::get_lines_batch(int32_t p_resolution) {
	auto it = lines.find(p_resolution);
	if (it != lines.end()) {
		return it->second;
	}

	MeshData sphere = create_ico_sphere(p_resolution);
	MeshData line_mesh_data;
	line_mesh_data.vertices = sphere.vertices;

	std::unordered_set<int64_t> added_edges;

	for (size_t index = 0; index + 2 < sphere.indices.size(); index += 3) {
		const int32_t a = sphere.indices[index + 0];
		const int32_t b = sphere.indices[index + 1];
		const int32_t c = sphere.indices[index + 2];

		add_line_edge(line_mesh_data, added_edges, a, b);
		add_line_edge(line_mesh_data, added_edges, b, c);
		add_line_edge(line_mesh_data, added_edges, c, a);
	}

	if (lines_material.is_null()) {
		lines_material = DebugDrawerResources::create_lines_material();
	}

	Ref<ArrayMesh> mesh = DebugDrawerResources::create_array_mesh(line_mesh_data, Mesh::PRIMITIVE_LINES, lines_material);
	return lines.emplace(p_resolution, create_batch(mesh)).first->second;
}

MultiMeshBatch& DebugSphereDrawer::get_fill_batch(int32_t p_resolution) {
	auto it = fill.find(p_resolution);
	if (it != fill.end()) {
		return it->second;
	}

	if (fill_material.is_null()) {
		fill_material = DebugDrawerResources::create_fill_material();
	}

	Ref<ArrayMesh> mesh = DebugDrawerResources::create_array_mesh(create_ico_sphere(p_resolution), Mesh::PRIMITIVE_TRIANGLES, fill_material);
	return fill.emplace(p_resolution, create_batch(mesh)).first->second;
}

MeshData DebugSphereDrawer::create_ico_sphere(int32_t p_resolution) {
	MeshData mesh_data = create_icosahedron();

	for (int32_t subdivision = 0; subdivision < p_resolution; subdivision++) {
		mesh_data = subdivide(mesh_data);
	}

	return mesh_data;
}

MeshData DebugSphereDrawer::create_icosahedron() {
	MeshData mesh_data;
	mesh_data.resize_for_vertices(12);
	mesh_data.resize_for_triangles(20);

	const real_t phi = static_cast<real_t>((1.0 + Math::sqrt(5.0)) / 2.0);

	mesh_data.set_vertex(0, Vector3(-1.0, phi, 0.0).normalized());
	mesh_data.set_vertex(1, Vector3(1.0, phi, 0.0).normalized());
	mesh_data.set_vertex(2, Vector3(-1.0, -phi, 0.0).normalized());
	mesh_data.set_vertex(3, Vector3(1.0, -phi, 0.0).normalized());
	mesh_data.set_vertex(4, Vector3(0.0, -1.0, phi).normalized());
	mesh_data.set_vertex(5, Vector3(0.0, 1.0, phi).normalized());
	mesh_data.set_vertex(6, Vector3(0.0, -1.0, -phi).normalized());
	mesh_data.set_vertex(7, Vector3(0.0, 1.0, -phi).normalized());
	mesh_data.set_vertex(8, Vector3(phi, 0.0, -1.0).normalized());
	mesh_data.set_vertex(9, Vector3(phi, 0.0, 1.0).normalized());
	mesh_data.set_vertex(10, Vector3(-phi, 0.0, -1.0).normalized());
	mesh_data.set_vertex(11, Vector3(-phi, 0.0, 1.0).normalized());

	mesh_data.set_triangle_indices(0, 0, 11, 5);
	mesh_data.set_triangle_indices(1, 0, 5, 1);
	mesh_data.set_triangle_indices(2, 0, 1, 7);
	mesh_data.set_triangle_indices(3, 0, 7, 10);
	mesh_data.set_triangle_indices(4, 0, 10, 11);
	mesh_data.set_triangle_indices(5, 1, 5, 9);
	mesh_data.set_triangle_indices(6, 5, 11, 4);
	mesh_data.set_triangle_indices(7, 11, 10, 2);
	mesh_data.set_triangle_indices(8, 10, 7, 6);
	mesh_data.set_triangle_indices(9, 7, 1, 8);
	mesh_data.set_triangle_indices(10, 3, 9, 4);
	mesh_data.set_triangle_indices(11, 3, 4, 2);
	mesh_data.set_triangle_indices(12, 3, 2, 6);
	mesh_data.set_triangle_indices(13, 3, 6, 8);
	mesh_data.set_triangle_indices(14, 3, 8, 9);
	mesh_data.set_triangle_indices(15, 4, 9, 5);
	mesh_data.set_triangle_indices(16, 2, 4, 11);
	mesh_data.set_triangle_indices(17, 6, 2, 10);
	mesh_data.set_triangle_indices(18, 8, 6, 7);
	mesh_data.set_triangle_indices(19, 9, 8, 1);

	return mesh_data;
}

MeshData DebugSphereDrawer::subdivide(MeshData& r_mesh_data) {
	std::unordered_map<int64_t, int32_t> added_midpoints;
	std::vector<int32_t> new_indices;
	new_indices.reserve(r_mesh_data.indices.size() * 4);

	for (size_t index = 0; index + 2 < r_mesh_data.indices.size(); index += 3) {
		const int32_t a = r_mesh_data.indices[index + 0];
		const int32_t b = r_mesh_data.indices[index + 1];
		const int32_t c = r_mesh_data.indices[index + 2];

		const int32_t ab = get_midpoint_index(r_mesh_data, added_midpoints, a, b);
		const int32_t bc = get_midpoint_index(r_mesh_data, added_midpoints, b, c);
		const int32_t ca = get_midpoint_index(r_mesh_data, added_midpoints, c, a);

		new_indices.insert(new_indices.end(), { a, ab, ca });
		new_indices.insert(new_indices.end(), { b, bc, ab });
		new_indices.insert(new_indices.end(), { c, ca, bc });
		new_indices.insert(new_indices.end(), { ab, bc, ca });
	}

	MeshData new_mesh_data;
	new_mesh_data.vertices = r_mesh_data.vertices;
	new_mesh_data.indices = std::move(new_indices);
	return new_mesh_data;
}

int32_t DebugSphereDrawer::get_midpoint_index(MeshData& r_mesh_data, std::unordered_map<int64_t, int32_t>& r_added_midpoints, int32_t p_a, int32_t p_b) {
	const int64_t key = get_edge_key(p_a, p_b);

	auto it = r_added_midpoints.find(key);
	if (it != r_added_midpoints.end()) {
		return it->second;
	}

	const Vector3 midpoint_position = ((r_mesh_data.vertices[p_a] + r_mesh_data.vertices[p_b]) * 0.5).normalized();
	const int32_t midpoint_index = r_mesh_data.add_vertex(midpoint_position);
	r_added_midpoints.emplace(key, midpoint_index);

	return midpoint_index;
}

void DebugSphereDrawer::add_line_edge(MeshData& r_mesh_data, std::unordered_set<int64_t>& r_added_edges, int32_t p_a, int32_t p_b) {
	if (r_added_edges.insert(get_edge_key(p_a, p_b)).second) {
		r_mesh_data.add_line_indices(p_a, p_b);
	}
}

int64_t DebugSphereDrawer::get_edge_key(int32_t p_a, int32_t p_b) {
	return (static_cast<int64_t>(std::min(p_a, p_b)) << 32) | std::max(p_a, p_b);
}
