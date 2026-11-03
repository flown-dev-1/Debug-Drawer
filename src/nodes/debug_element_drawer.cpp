#include "nodes/debug_element_drawer.h"

#include "classes/debug_drawer_settings.h"

#include <godot_cpp/variant/utility_functions.hpp>

void DebugElementDrawer::_enter_tree() {
	max_shape_instances = DebugDrawerSettings::get_max_shape_instances();
}

void DebugElementDrawer::push_too_many_shapes_warning() const {
	UtilityFunctions::push_warning(vformat(
			"[Debug Drawer] Attempted to draw too many shapes in one frame. The limit is set to %d, try increasing it in Project Settings > %s",
			max_shape_instances, DebugDrawerSettings::MAX_SHAPE_INSTANCES_PATH));
}
