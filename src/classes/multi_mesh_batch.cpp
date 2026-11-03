#include "classes/multi_mesh_batch.h"

#include <godot_cpp/core/memory.hpp>

using namespace godot;

void MultiMeshBatch::finalize() {
	if (multimesh.is_valid()) {
		multimesh->set_visible_instance_count(count);
	}
}

void finalize_batches(ResolutionBatches& p_batches) {
	for (auto& entry : p_batches) {
		entry.second.finalize();
	}
}

void reset_batches(ResolutionBatches& p_batches) {
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
