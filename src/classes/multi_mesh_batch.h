#pragma once

#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/classes/ref.hpp>

#include <cstdint>
#include <map>

struct MultiMeshBatch {
	int32_t count = 0;
	godot::Ref<godot::MultiMesh> multimesh;
	godot::MultiMeshInstance3D* instance = nullptr;

	void finalize();
};

using ResolutionBatches = std::map<int32_t, MultiMeshBatch>;

void finalize_batches(ResolutionBatches& p_batches);
void reset_batches(ResolutionBatches& p_batches);
