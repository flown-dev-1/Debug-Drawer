#pragma once

#include "nodes/debug_drawer.h"

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/quaternion.hpp>

#include <deque>

using namespace godot;

class GlobalDebugDrawer : public Object {
	GDCLASS(GlobalDebugDrawer, Object)

	static GlobalDebugDrawer* instance;

	DebugDrawer* target_debug_drawer = nullptr;
	std::deque<DebugDrawer*> target_debug_drawer_queue;

	bool _check_target() const;
	void _update_configuration_warnings();

protected:
	static void _bind_methods();

public:
	static GlobalDebugDrawer* get_instance() {
		return instance;
	}

	GlobalDebugDrawer();
	~GlobalDebugDrawer() override;

	void register_debug_drawer(DebugDrawer* p_debug_drawer);
	void unregister_debug_drawer(DebugDrawer* p_debug_drawer);

	bool are_drawers_queued() const {
		return !target_debug_drawer_queue.empty();
	}

	DebugDrawer* get_target_debug_drawer() const {
		return target_debug_drawer;
	}

	void draw_cube(const Vector3& p_center, const Vector3& p_size, const Color& p_line_color, const Color& p_fill_color = Color(0, 0, 0, 0));
	void draw_cube_rotated(const Vector3& p_center, const Vector3& p_size, const Quaternion& p_rotation, const Color& p_line_color, const Color& p_fill_color = Color(0, 0, 0, 0));
	void draw_line(const Vector3& p_start, const Vector3& p_end, const Color& p_line_color);
	void draw_path(const PackedVector3Array& p_path, const Color& p_line_color);
	void draw_vector(const Vector3& p_origin, const Vector3& p_vector, const Color& p_line_color, bool p_draw_arrow = true);
	void draw_sphere(const Vector3& p_center, real_t p_radius, DebugDrawer::SphereResolution p_resolution, const Color& p_line_color, const Color& p_fill_color = Color(0, 0, 0, 0));
	void draw_cylinder(const Vector3& p_center, real_t p_height, real_t p_top_radius, real_t p_bottom_radius, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color = Color(0, 0, 0, 0));
	void draw_cylinder_rotated(const Vector3& p_center, real_t p_height, real_t p_top_radius, real_t p_bottom_radius, const Quaternion& p_rotation, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color = Color(0, 0, 0, 0));
	void draw_text(const String& p_string, const Vector3& p_position, const Color& p_text_color = Color(1, 1, 1, 1), const Color& p_outline_color = Color(0, 0, 0, 1), int32_t p_font_size = 32, HorizontalAlignment p_horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER, VerticalAlignment p_vertical_alignment = VERTICAL_ALIGNMENT_CENTER);
};
