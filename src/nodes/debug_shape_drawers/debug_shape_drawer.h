#pragma once

#include "classes/multi_mesh_batch.h"
#include "nodes/debug_element_drawer.h"

#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/transform3d.hpp>

class DebugShapeDrawer : public DebugElementDrawer {
	GDCLASS(DebugShapeDrawer, DebugElementDrawer)

protected:
	static void _bind_methods() {}

	MultiMeshBatch create_batch(const Ref<ArrayMesh>& p_mesh, bool p_use_custom_data = false);
	MultiMeshInstance3D* create_multi_mesh_instance(const Ref<MultiMesh>& p_multimesh);
	Ref<MultiMesh> create_multimesh(const Ref<ArrayMesh>& p_mesh, bool p_use_custom_data = false) const;

	bool push_instance(MultiMeshBatch& p_batch, const Transform3D& p_transform, const Color& p_color);
	bool push_instance(MultiMeshBatch& p_batch, const Transform3D& p_transform, const Color& p_color, const Color& p_custom_data);
};
