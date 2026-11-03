#include "nodes/debug_shape_drawers/debug_shape_drawer.h"

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
