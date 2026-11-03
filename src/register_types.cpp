#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "autoloads/global_debug_drawer.h"
#include "classes/debug_drawer_settings.h"
#include "nodes/debug_drawer.h"
#include "nodes/debug_element_drawer.h"
#include "nodes/debug_shape_drawers/debug_cube_drawer.h"
#include "nodes/debug_shape_drawers/debug_cylinder_drawer.h"
#include "nodes/debug_shape_drawers/debug_line_drawer.h"
#include "nodes/debug_shape_drawers/debug_shape_drawer.h"
#include "nodes/debug_shape_drawers/debug_sphere_drawer.h"
#include "nodes/debug_text_drawer.h"
#include "nodes/late_processer.h"

using namespace godot;

static GlobalDebugDrawer* global_debug_drawer = nullptr;

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	DebugDrawerSettings::register_settings();

	GDREGISTER_INTERNAL_CLASS(DebugElementDrawer);
	GDREGISTER_INTERNAL_CLASS(DebugShapeDrawer);
	GDREGISTER_INTERNAL_CLASS(DebugCubeDrawer);
	GDREGISTER_INTERNAL_CLASS(DebugLineDrawer);
	GDREGISTER_INTERNAL_CLASS(DebugCylinderDrawer);
	GDREGISTER_INTERNAL_CLASS(DebugSphereDrawer);
	GDREGISTER_INTERNAL_CLASS(DebugTextDrawer);
	GDREGISTER_INTERNAL_CLASS(LateProcesser);

	GDREGISTER_CLASS(DebugDrawer);
	GDREGISTER_ABSTRACT_CLASS(GlobalDebugDrawer);

	global_debug_drawer = memnew(GlobalDebugDrawer);
	Engine::get_singleton()->register_singleton("GlobalDebugDrawer", global_debug_drawer);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if (global_debug_drawer != nullptr) {
		Engine::get_singleton()->unregister_singleton("GlobalDebugDrawer");
		memdelete(global_debug_drawer);
		global_debug_drawer = nullptr;
	}
}

extern "C" {
// Initialization
GDExtensionBool GDE_EXPORT debug_drawer_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
