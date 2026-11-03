#include "nodes/debug_shape_drawers/debug_shape_drawer.h"

#include <godot_cpp/classes/base_material3d.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/variant/array.hpp>

MultiMeshBatch DebugShapeDrawer::_create_batch(const Ref<ArrayMesh>& p_mesh, bool p_use_custom_data) {
	MultiMeshBatch batch;
	batch.multimesh = _create_multimesh(p_mesh, p_use_custom_data);
	batch.instance = _create_multi_mesh_instance(batch.multimesh);
	return batch;
}

MultiMeshInstance3D* DebugShapeDrawer::_create_multi_mesh_instance(const Ref<MultiMesh>& p_multimesh) {
	MultiMeshInstance3D* instance = memnew(MultiMeshInstance3D);
	add_child(instance, false, Node::INTERNAL_MODE_FRONT);
	instance->set_multimesh(p_multimesh);
	return instance;
}

Ref<MultiMesh> DebugShapeDrawer::_create_multimesh(const Ref<ArrayMesh>& p_mesh, bool p_use_custom_data) const {
	Ref<MultiMesh> multimesh;
	multimesh.instantiate();

	multimesh->set_mesh(p_mesh);
	multimesh->set_transform_format(MultiMesh::TRANSFORM_3D);
	multimesh->set_use_colors(true);
	multimesh->set_use_custom_data(p_use_custom_data);
	multimesh->set_instance_count(static_cast<int32_t>(max_shape_instances));
	multimesh->set_visible_instance_count(0);

	return multimesh;
}

Ref<ArrayMesh> DebugShapeDrawer::_create_array_mesh(const MeshData& p_mesh_data, Mesh::PrimitiveType p_primitive, const Ref<Material>& p_material) {
	Ref<ArrayMesh> array_mesh;
	array_mesh.instantiate();

	Array arrays;
	arrays.resize(Mesh::ARRAY_MAX);
	arrays[Mesh::ARRAY_VERTEX] = p_mesh_data.get_packed_vertices();
	arrays[Mesh::ARRAY_INDEX] = p_mesh_data.get_packed_indices();

	array_mesh->add_surface_from_arrays(p_primitive, arrays);
	array_mesh->surface_set_material(0, p_material);

	return array_mesh;
}

Ref<StandardMaterial3D> DebugShapeDrawer::_create_lines_material() {
	Ref<StandardMaterial3D> material;
	material.instantiate();

	material->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	material->set_flag(BaseMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

	return material;
}

Ref<StandardMaterial3D> DebugShapeDrawer::_create_fill_material() {
	Ref<StandardMaterial3D> material;
	material.instantiate();

	material->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	material->set_flag(BaseMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
	material->set_transparency(BaseMaterial3D::TRANSPARENCY_ALPHA);
	material->set_cull_mode(BaseMaterial3D::CULL_DISABLED);

	return material;
}

Ref<ShaderMaterial> DebugShapeDrawer::_create_shader_material(const char* p_code) {
	Ref<Shader> shader;
	shader.instantiate();
	shader->set_code(p_code);

	Ref<ShaderMaterial> material;
	material.instantiate();
	material->set_shader(shader);

	return material;
}

bool DebugShapeDrawer::_push_instance(MultiMeshBatch& p_batch, const Transform3D& p_transform, const Color& p_color) {
	if (p_batch.count >= max_shape_instances) {
		_push_too_many_shapes_warning();
		return false;
	}

	p_batch.multimesh->set_instance_transform(p_batch.count, p_transform);
	p_batch.multimesh->set_instance_color(p_batch.count, p_color);
	p_batch.count++;

	return true;
}

bool DebugShapeDrawer::_push_instance(MultiMeshBatch& p_batch, const Transform3D& p_transform, const Color& p_color, const Color& p_custom_data) {
	const int32_t index = p_batch.count;
	if (!_push_instance(p_batch, p_transform, p_color)) {
		return false;
	}

	p_batch.multimesh->set_instance_custom_data(index, p_custom_data);
	return true;
}

void DebugShapeDrawer::_finalize_batch(MultiMeshBatch& p_batch) {
	if (p_batch.multimesh.is_valid()) {
		p_batch.multimesh->set_visible_instance_count(p_batch.count);
	}
}

void DebugShapeDrawer::_finalize_batches(ResolutionBatches& p_batches) {
	for (auto& entry : p_batches) {
		_finalize_batch(entry.second);
	}
}

void DebugShapeDrawer::_reset_batches(ResolutionBatches& p_batches) {
	for (auto it = p_batches.begin(); it != p_batches.end();) {
		MultiMeshBatch& batch = it->second;
		if (batch.count == 0) {
			if (batch.instance != nullptr) {
				memdelete(batch.instance);
			}
			it = p_batches.erase(it);
		} else {
			batch.count = 0;
			++it;
		}
	}
}
