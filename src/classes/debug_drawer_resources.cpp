#include "classes/debug_drawer_resources.h"

#include <godot_cpp/classes/base_material3d.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/variant/array.hpp>

using namespace godot;

namespace DebugDrawerResources {

Ref<ArrayMesh> create_array_mesh(const MeshData& p_mesh_data, Mesh::PrimitiveType p_primitive, const Ref<Material>& p_material) {
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

Ref<StandardMaterial3D> create_lines_material() {
	Ref<StandardMaterial3D> material;
	material.instantiate();

	material->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	material->set_flag(BaseMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

	return material;
}

Ref<StandardMaterial3D> create_fill_material() {
	Ref<StandardMaterial3D> material;
	material.instantiate();

	material->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	material->set_flag(BaseMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
	material->set_transparency(BaseMaterial3D::TRANSPARENCY_ALPHA);
	material->set_cull_mode(BaseMaterial3D::CULL_DISABLED);

	return material;
}

Ref<ShaderMaterial> create_shader_material(const char* p_code) {
	Ref<Shader> shader;
	shader.instantiate();
	shader->set_code(p_code);

	Ref<ShaderMaterial> material;
	material.instantiate();
	material->set_shader(shader);

	return material;
}

}
