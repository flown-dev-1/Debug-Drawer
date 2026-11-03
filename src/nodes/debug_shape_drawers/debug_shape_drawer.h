#pragma once

#include "classes/mesh_data.h"
#include "nodes/debug_element_drawer.h"

#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <map>

struct MultiMeshBatch {
	int32_t count = 0;
	Ref<MultiMesh> multimesh;
	MultiMeshInstance3D* instance = nullptr;
};

using ResolutionBatches = std::map<int32_t, MultiMeshBatch>;

class DebugShapeDrawer : public DebugElementDrawer {
	GDCLASS(DebugShapeDrawer, DebugElementDrawer)

protected:
	static void _bind_methods() {}

	MultiMeshBatch _create_batch(const Ref<ArrayMesh>& p_mesh, bool p_use_custom_data = false);
	MultiMeshInstance3D* _create_multi_mesh_instance(const Ref<MultiMesh>& p_multimesh);
	Ref<MultiMesh> _create_multimesh(const Ref<ArrayMesh>& p_mesh, bool p_use_custom_data = false) const;

	static Ref<ArrayMesh> _create_array_mesh(const MeshData& p_mesh_data, Mesh::PrimitiveType p_primitive, const Ref<Material>& p_material);
	static Ref<StandardMaterial3D> _create_lines_material();
	static Ref<StandardMaterial3D> _create_fill_material();
	static Ref<ShaderMaterial> _create_shader_material(const char* p_code);

	bool _push_instance(MultiMeshBatch& p_batch, const Transform3D& p_transform, const Color& p_color);
	bool _push_instance(MultiMeshBatch& p_batch, const Transform3D& p_transform, const Color& p_color, const Color& p_custom_data);

	static void _finalize_batch(MultiMeshBatch& p_batch);
	static void _finalize_batches(ResolutionBatches& p_batches);
	static void _reset_batches(ResolutionBatches& p_batches);
};
