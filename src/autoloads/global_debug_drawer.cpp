#include "autoloads/global_debug_drawer.h"

#include "nodes/debug_drawer.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <algorithm>

GlobalDebugDrawer* GlobalDebugDrawer::instance = nullptr;

static constexpr const char* NO_GLOBAL_DRAWER_ERROR = "[Debug Drawer] Attempting to perform a global debug drawer operation with no global debug drawer";

void GlobalDebugDrawer::_bind_methods() {
	const Color transparent(0, 0, 0, 0);

	ClassDB::bind_method(D_METHOD("get_target_debug_drawer"), &GlobalDebugDrawer::get_target_debug_drawer);

	ClassDB::bind_method(D_METHOD("draw_cube", "center", "size", "line_color", "fill_color"), &GlobalDebugDrawer::draw_cube, DEFVAL(transparent));
	ClassDB::bind_method(D_METHOD("draw_cube_rotated", "center", "size", "rotation", "line_color", "fill_color"), &GlobalDebugDrawer::draw_cube_rotated, DEFVAL(transparent));
	ClassDB::bind_method(D_METHOD("draw_line", "start", "end", "line_color"), &GlobalDebugDrawer::draw_line);
	ClassDB::bind_method(D_METHOD("draw_path", "path", "line_color"), &GlobalDebugDrawer::draw_path);
	ClassDB::bind_method(D_METHOD("draw_vector", "origin", "vector", "line_color", "draw_arrow"), &GlobalDebugDrawer::draw_vector, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("draw_sphere", "center", "radius", "resolution", "line_color", "fill_color"), &GlobalDebugDrawer::draw_sphere, DEFVAL(transparent));
	ClassDB::bind_method(D_METHOD("draw_cylinder", "center", "height", "top_radius", "bottom_radius", "resolution", "line_color", "fill_color"), &GlobalDebugDrawer::draw_cylinder, DEFVAL(transparent));
	ClassDB::bind_method(D_METHOD("draw_cylinder_rotated", "center", "height", "top_radius", "bottom_radius", "rotation", "resolution", "line_color", "fill_color"), &GlobalDebugDrawer::draw_cylinder_rotated, DEFVAL(transparent));
	ClassDB::bind_method(D_METHOD("draw_text", "string", "position", "text_color", "outline_color", "font_size", "horizontal_alignment", "vertical_alignment"), &GlobalDebugDrawer::draw_text,
			DEFVAL(Color(1, 1, 1, 1)), DEFVAL(Color(0, 0, 0, 1)), DEFVAL(32), DEFVAL(HORIZONTAL_ALIGNMENT_CENTER), DEFVAL(VERTICAL_ALIGNMENT_CENTER));
}

GlobalDebugDrawer::GlobalDebugDrawer() {
	instance = this;
}

GlobalDebugDrawer::~GlobalDebugDrawer() {
	if (instance == this) {
		instance = nullptr;
	}
}

bool GlobalDebugDrawer::check_target() const {
	if (target_debug_drawer == nullptr) {
		UtilityFunctions::push_error(NO_GLOBAL_DRAWER_ERROR);
		return false;
	}
	return true;
}

void GlobalDebugDrawer::register_debug_drawer(DebugDrawer* p_debug_drawer) {
	if (p_debug_drawer == nullptr || p_debug_drawer == target_debug_drawer) {
		return;
	}

	if (target_debug_drawer != nullptr) {
		if (std::find(target_debug_drawer_queue.begin(), target_debug_drawer_queue.end(), p_debug_drawer) == target_debug_drawer_queue.end()) {
			target_debug_drawer_queue.push_back(p_debug_drawer);
			refresh_drawer_warnings();
		}
		return;
	}

	target_debug_drawer = p_debug_drawer;
}

void GlobalDebugDrawer::unregister_debug_drawer(DebugDrawer* p_debug_drawer) {
	if (target_debug_drawer == p_debug_drawer) {
		if (!target_debug_drawer_queue.empty()) {
			target_debug_drawer = target_debug_drawer_queue.front();
			target_debug_drawer_queue.pop_front();
			refresh_drawer_warnings();
		} else {
			target_debug_drawer = nullptr;
		}
		return;
	}

	auto it = std::find(target_debug_drawer_queue.begin(), target_debug_drawer_queue.end(), p_debug_drawer);
	if (it != target_debug_drawer_queue.end()) {
		target_debug_drawer_queue.erase(it);
		refresh_drawer_warnings();
	}
}

void GlobalDebugDrawer::refresh_drawer_warnings() {
	if (target_debug_drawer != nullptr) {
		target_debug_drawer->update_configuration_warnings();
	}

	for (DebugDrawer* debug_drawer : target_debug_drawer_queue) {
		debug_drawer->update_configuration_warnings();
	}
}

void GlobalDebugDrawer::draw_cube(const Vector3& p_center, const Vector3& p_size, const Color& p_line_color, const Color& p_fill_color) {
	if (check_target()) {
		target_debug_drawer->draw_cube(p_center, p_size, p_line_color, p_fill_color);
	}
}

void GlobalDebugDrawer::draw_cube_rotated(const Vector3& p_center, const Vector3& p_size, const Quaternion& p_rotation, const Color& p_line_color, const Color& p_fill_color) {
	if (check_target()) {
		target_debug_drawer->draw_cube_rotated(p_center, p_size, p_rotation, p_line_color, p_fill_color);
	}
}

void GlobalDebugDrawer::draw_line(const Vector3& p_start, const Vector3& p_end, const Color& p_line_color) {
	if (check_target()) {
		target_debug_drawer->draw_line(p_start, p_end, p_line_color);
	}
}

void GlobalDebugDrawer::draw_path(const PackedVector3Array& p_path, const Color& p_line_color) {
	if (check_target()) {
		target_debug_drawer->draw_path(p_path, p_line_color);
	}
}

void GlobalDebugDrawer::draw_vector(const Vector3& p_origin, const Vector3& p_vector, const Color& p_line_color, bool p_draw_arrow) {
	if (check_target()) {
		target_debug_drawer->draw_vector(p_origin, p_vector, p_line_color, p_draw_arrow);
	}
}

void GlobalDebugDrawer::draw_sphere(const Vector3& p_center, real_t p_radius, DebugDrawer::SphereResolution p_resolution, const Color& p_line_color, const Color& p_fill_color) {
	if (check_target()) {
		target_debug_drawer->draw_sphere(p_center, p_radius, p_resolution, p_line_color, p_fill_color);
	}
}

void GlobalDebugDrawer::draw_cylinder(const Vector3& p_center, real_t p_height, real_t p_top_radius, real_t p_bottom_radius, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color) {
	if (check_target()) {
		target_debug_drawer->draw_cylinder(p_center, p_height, p_top_radius, p_bottom_radius, p_resolution, p_line_color, p_fill_color);
	}
}

void GlobalDebugDrawer::draw_cylinder_rotated(const Vector3& p_center, real_t p_height, real_t p_top_radius, real_t p_bottom_radius, const Quaternion& p_rotation, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color) {
	if (check_target()) {
		target_debug_drawer->draw_cylinder_rotated(p_center, p_height, p_top_radius, p_bottom_radius, p_rotation, p_resolution, p_line_color, p_fill_color);
	}
}

void GlobalDebugDrawer::draw_text(const String& p_string, const Vector3& p_position, const Color& p_text_color, const Color& p_outline_color, int32_t p_font_size, HorizontalAlignment p_horizontal_alignment, VerticalAlignment p_vertical_alignment) {
	if (check_target()) {
		target_debug_drawer->draw_text(p_string, p_position, p_text_color, p_outline_color, p_font_size, p_horizontal_alignment, p_vertical_alignment);
	}
}
