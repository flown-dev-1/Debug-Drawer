#include "classes/debug_drawer_settings.h"

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

namespace DebugDrawerSettings {

void register_settings() {
	ProjectSettings* settings = ProjectSettings::get_singleton();
	if (settings == nullptr) {
		return;
	}

	if (!settings->has_setting(MAX_SHAPE_INSTANCES_PATH)) {
		settings->set_setting(MAX_SHAPE_INSTANCES_PATH, MAX_SHAPE_INSTANCES_DEFAULT);
	}

	settings->set_initial_value(MAX_SHAPE_INSTANCES_PATH, MAX_SHAPE_INSTANCES_DEFAULT);

	Dictionary info;
	info["name"] = MAX_SHAPE_INSTANCES_PATH;
	info["type"] = Variant::INT;
	info["hint"] = PROPERTY_HINT_RANGE;
	info["hint_string"] = "128,65536,1";
	settings->add_property_info(info);
}

int64_t get_max_shape_instances() {
	ProjectSettings* settings = ProjectSettings::get_singleton();
	if (settings == nullptr) {
		return MAX_SHAPE_INSTANCES_DEFAULT;
	}
	return settings->get_setting(MAX_SHAPE_INSTANCES_PATH, MAX_SHAPE_INSTANCES_DEFAULT);
}

}
