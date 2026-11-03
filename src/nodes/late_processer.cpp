#include "nodes/late_processer.h"

#include "nodes/debug_drawer.h"

void LateProcesser::_enter_tree() {
	set_process_priority(1000);
}

void LateProcesser::_process(double p_delta) {
	if (owner_drawer != nullptr) {
		owner_drawer->late_process(p_delta);
	}
}
