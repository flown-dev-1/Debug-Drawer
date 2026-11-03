#pragma once

#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include <cstdint>
#include <cstring>
#include <vector>

struct MeshData {
	std::vector<godot::Vector3> vertices;
	std::vector<int32_t> indices;

	void resize_for_vertices(size_t p_amount) {
		vertices.resize(p_amount);
	}

	void resize_for_indices(size_t p_amount) {
		indices.resize(p_amount);
	}

	void resize_for_triangles(size_t p_amount) {
		indices.resize(p_amount * 3);
	}

	void resize_for_lines(size_t p_amount) {
		indices.resize(p_amount * 2);
	}

	int32_t add_vertex(const godot::Vector3& p_position) {
		vertices.push_back(p_position);
		return static_cast<int32_t>(vertices.size() - 1);
	}

	void set_vertex(size_t p_index, const godot::Vector3& p_position) {
		vertices[p_index] = p_position;
	}

	void add_triangle_indices(int32_t p_a, int32_t p_b, int32_t p_c) {
		indices.push_back(p_a);
		indices.push_back(p_b);
		indices.push_back(p_c);
	}

	void set_triangle_indices(size_t p_index, int32_t p_a, int32_t p_b, int32_t p_c) {
		const size_t i = p_index * 3;
		indices[i + 0] = p_a;
		indices[i + 1] = p_b;
		indices[i + 2] = p_c;
	}

	void add_line_indices(int32_t p_a, int32_t p_b) {
		indices.push_back(p_a);
		indices.push_back(p_b);
	}

	void set_line_indices(size_t p_index, int32_t p_a, int32_t p_b) {
		const size_t i = p_index * 2;
		indices[i + 0] = p_a;
		indices[i + 1] = p_b;
	}

	godot::PackedVector3Array get_packed_vertices() const {
		godot::PackedVector3Array result;
		result.resize(static_cast<int64_t>(vertices.size()));

		if (!vertices.empty()) {
			memcpy(result.ptrw(), vertices.data(), vertices.size() * sizeof(godot::Vector3));
		}

		return result;
	}

	godot::PackedInt32Array get_packed_indices() const {
		godot::PackedInt32Array result;
		result.resize(static_cast<int64_t>(indices.size()));

		if (!indices.empty()) {
			memcpy(result.ptrw(), indices.data(), indices.size() * sizeof(int32_t));
		}

		return result;
	}
};
