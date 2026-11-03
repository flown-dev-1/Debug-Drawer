#include "nodes/debug_drawer.h"

#include "autoloads/global_debug_drawer.h"
#include "nodes/debug_shape_drawers/debug_cube_drawer.h"
#include "nodes/debug_shape_drawers/debug_cylinder_drawer.h"
#include "nodes/debug_shape_drawers/debug_line_drawer.h"
#include "nodes/debug_shape_drawers/debug_sphere_drawer.h"
#include "nodes/debug_text_drawer.h"
#include "nodes/late_processer.h"

#include <godot_cpp/core/class_db.hpp>

static constexpr const char* MULTIPLE_DRAWERS_WARNING = "Multiple global debug drawers defined.";
static constexpr const char* NOT_IN_TREE_ERROR = "[Debug Drawer] DebugDrawer must be inside the scene tree to draw.";

void DebugDrawer::_bind_methods() {
	BIND_ENUM_CONSTANT(LOWEST);
	BIND_ENUM_CONSTANT(LOW);
	BIND_ENUM_CONSTANT(MEDIUM);
	BIND_ENUM_CONSTANT(HIGH);
	BIND_ENUM_CONSTANT(HIGHEST);

	ClassDB::bind_method(D_METHOD("set_global", "global"), &DebugDrawer::set_global);
	ClassDB::bind_method(D_METHOD("is_global"), &DebugDrawer::is_global);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "global"), "set_global", "is_global");

	const Color transparent(0, 0, 0, 0);

	ClassDB::bind_method(D_METHOD("draw_cube", "center", "size", "line_color", "fill_color"), &DebugDrawer::draw_cube, DEFVAL(transparent));
	ClassDB::bind_method(D_METHOD("draw_cube_rotated", "center", "size", "rotation", "line_color", "fill_color"), &DebugDrawer::draw_cube_rotated, DEFVAL(transparent));
	ClassDB::bind_method(D_METHOD("draw_line", "start", "end", "line_color"), &DebugDrawer::draw_line);
	ClassDB::bind_method(D_METHOD("draw_path", "path", "line_color"), &DebugDrawer::draw_path);
	ClassDB::bind_method(D_METHOD("draw_vector", "origin", "vector", "line_color", "draw_arrow"), &DebugDrawer::draw_vector, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("draw_sphere", "center", "radius", "resolution", "line_color", "fill_color"), &DebugDrawer::draw_sphere, DEFVAL(transparent));
	ClassDB::bind_method(D_METHOD("draw_cylinder", "center", "height", "top_radius", "bottom_radius", "resolution", "line_color", "fill_color"), &DebugDrawer::draw_cylinder, DEFVAL(transparent));
	ClassDB::bind_method(D_METHOD("draw_cylinder_rotated", "center", "height", "top_radius", "bottom_radius", "rotation", "resolution", "line_color", "fill_color"), &DebugDrawer::draw_cylinder_rotated, DEFVAL(transparent));
	ClassDB::bind_method(D_METHOD("draw_text", "string", "position", "text_color", "outline_color", "font_size", "horizontal_alignment", "vertical_alignment"), &DebugDrawer::draw_text,
			DEFVAL(Color(1, 1, 1, 1)), DEFVAL(Color(0, 0, 0, 1)), DEFVAL(32), DEFVAL(HORIZONTAL_ALIGNMENT_CENTER), DEFVAL(VERTICAL_ALIGNMENT_CENTER));
}

void DebugDrawer::set_global(bool p_global) {
	if (p_global == global) {
		return;
	}

	global = p_global;

	if (!is_inside_tree()) {
		return;
	}

	GlobalDebugDrawer* global_drawer = GlobalDebugDrawer::get_instance();
	if (global_drawer == nullptr) {
		return;
	}

	if (global) {
		global_drawer->register_debug_drawer(this);
	} else {
		global_drawer->unregister_debug_drawer(this);
		update_configuration_warnings();
	}
}

bool DebugDrawer::is_global() const {
	return global;
}

void DebugDrawer::_enter_tree() {
	if (late_processer == nullptr) {
		late_processer = memnew(LateProcesser);
		late_processer->set_owner_drawer(this);
		add_child(late_processer, false, Node::INTERNAL_MODE_FRONT);
	}

	if (cube_drawer == nullptr) {
		cube_drawer = memnew(DebugCubeDrawer);
		add_child(cube_drawer, false, Node::INTERNAL_MODE_FRONT);
	}

	if (line_drawer == nullptr) {
		line_drawer = memnew(DebugLineDrawer);
		add_child(line_drawer, false, Node::INTERNAL_MODE_FRONT);
	}

	if (sphere_drawer == nullptr) {
		sphere_drawer = memnew(DebugSphereDrawer);
		add_child(sphere_drawer, false, Node::INTERNAL_MODE_FRONT);
	}

	if (cylinder_drawer == nullptr) {
		cylinder_drawer = memnew(DebugCylinderDrawer);
		add_child(cylinder_drawer, false, Node::INTERNAL_MODE_FRONT);
	}

	if (text_drawer == nullptr) {
		text_drawer = memnew(DebugTextDrawer);
		add_child(text_drawer, false, Node::INTERNAL_MODE_FRONT);
	}

	set_process_priority(-1000);

	if (global) {
		if (GlobalDebugDrawer* global_drawer = GlobalDebugDrawer::get_instance()) {
			global_drawer->register_debug_drawer(this);
		}
	}
}

void DebugDrawer::_exit_tree() {
	if (GlobalDebugDrawer* global_drawer = GlobalDebugDrawer::get_instance()) {
		global_drawer->unregister_debug_drawer(this);
	}
}

void DebugDrawer::_process(double p_delta) {
	if (!has_drawers()) {
		return;
	}

	cube_drawer->reset();
	line_drawer->reset();
	sphere_drawer->reset();
	cylinder_drawer->reset();
	text_drawer->reset();
}

void DebugDrawer::late_process(double p_delta) {
	if (!has_drawers()) {
		return;
	}

	cube_drawer->finalize();
	line_drawer->finalize();
	sphere_drawer->finalize();
	cylinder_drawer->finalize();
}

PackedStringArray DebugDrawer::_get_configuration_warnings() const {
	PackedStringArray warnings;

	GlobalDebugDrawer* global_drawer = GlobalDebugDrawer::get_instance();
	if (global && global_drawer != nullptr && global_drawer->are_drawers_queued()) {
		warnings.push_back(MULTIPLE_DRAWERS_WARNING);
	}

	return warnings;
}

void DebugDrawer::draw_cube(const Vector3& p_center, const Vector3& p_size, const Color& p_line_color, const Color& p_fill_color) {
	ERR_FAIL_COND_MSG(!has_drawers(), NOT_IN_TREE_ERROR);
	cube_drawer->draw(p_center, p_size, Quaternion(), p_line_color, p_fill_color);
}

void DebugDrawer::draw_cube_rotated(const Vector3& p_center, const Vector3& p_size, const Quaternion& p_rotation, const Color& p_line_color, const Color& p_fill_color) {
	ERR_FAIL_COND_MSG(!has_drawers(), NOT_IN_TREE_ERROR);
	cube_drawer->draw(p_center, p_size, p_rotation, p_line_color, p_fill_color);
}

void DebugDrawer::draw_line(const Vector3& p_start, const Vector3& p_end, const Color& p_line_color) {
	ERR_FAIL_COND_MSG(!has_drawers(), NOT_IN_TREE_ERROR);
	line_drawer->draw(p_start, p_end, p_line_color);
}

void DebugDrawer::draw_path(const PackedVector3Array& p_path, const Color& p_line_color) {
	ERR_FAIL_COND_MSG(!has_drawers(), NOT_IN_TREE_ERROR);
	for (int64_t i = 0; i < p_path.size() - 1; i++) {
		line_drawer->draw(p_path[i], p_path[i + 1], p_line_color);
	}
}

void DebugDrawer::draw_vector(const Vector3& p_origin, const Vector3& p_vector, const Color& p_line_color, bool p_draw_arrow) {
	ERR_FAIL_COND_MSG(!has_drawers(), NOT_IN_TREE_ERROR);
	line_drawer->draw(p_origin, p_origin + p_vector, p_line_color);

	if (!p_draw_arrow || p_vector.is_zero_approx()) {
		return;
	}

	const real_t vector_length = p_vector.length();
	const real_t arrow_scale = vector_length / 20.0f;
	const Vector3 normalized_vector = p_vector.normalized();

	cylinder_drawer->draw(
			p_origin + p_vector - (normalized_vector * arrow_scale) / 2.0f,
			arrow_scale * 2.0f,
			0.0f,
			arrow_scale / 1.5f,
			Quaternion(Vector3(0, 1, 0), normalized_vector),
			16,
			Color(0, 0, 0, 0),
			p_line_color);
}

void DebugDrawer::draw_sphere(const Vector3& p_center, real_t p_radius, SphereResolution p_resolution, const Color& p_line_color, const Color& p_fill_color) {
	ERR_FAIL_COND_MSG(!has_drawers(), NOT_IN_TREE_ERROR);
	sphere_drawer->draw(p_center, p_radius, p_resolution, p_line_color, p_fill_color);
}

void DebugDrawer::draw_cylinder(const Vector3& p_center, real_t p_height, real_t p_top_radius, real_t p_bottom_radius, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color) {
	ERR_FAIL_COND_MSG(!has_drawers(), NOT_IN_TREE_ERROR);
	cylinder_drawer->draw(p_center, p_height, p_top_radius, p_bottom_radius, Quaternion(), p_resolution, p_line_color, p_fill_color);
}

void DebugDrawer::draw_cylinder_rotated(const Vector3& p_center, real_t p_height, real_t p_top_radius, real_t p_bottom_radius, const Quaternion& p_rotation, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color) {
	ERR_FAIL_COND_MSG(!has_drawers(), NOT_IN_TREE_ERROR);
	cylinder_drawer->draw(p_center, p_height, p_top_radius, p_bottom_radius, p_rotation, p_resolution, p_line_color, p_fill_color);
}

void DebugDrawer::draw_text(const String& p_string, const Vector3& p_position, const Color& p_text_color, const Color& p_outline_color, int32_t p_font_size, HorizontalAlignment p_horizontal_alignment, VerticalAlignment p_vertical_alignment) {
	ERR_FAIL_COND_MSG(!has_drawers(), NOT_IN_TREE_ERROR);
	text_drawer->draw(p_string, p_position, p_text_color, p_outline_color, p_font_size, p_horizontal_alignment, p_vertical_alignment);
}
