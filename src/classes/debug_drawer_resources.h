#pragma once

#include "classes/mesh_data.h"

#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

namespace DebugDrawerResources {

godot::Ref<godot::ArrayMesh> create_array_mesh(const MeshData& p_mesh_data, godot::Mesh::PrimitiveType p_primitive, const godot::Ref<godot::Material>& p_material);
godot::Ref<godot::StandardMaterial3D> create_lines_material();
godot::Ref<godot::StandardMaterial3D> create_fill_material();
godot::Ref<godot::ShaderMaterial> create_shader_material(const char* p_code);

}
