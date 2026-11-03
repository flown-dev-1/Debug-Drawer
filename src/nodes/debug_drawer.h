#pragma once

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/quaternion.hpp>

using namespace godot;

class DebugCubeDrawer;
class DebugLineDrawer;
class DebugSphereDrawer;
class DebugCylinderDrawer;
class DebugTextDrawer;
class LateProcesser;

class DebugDrawer : public Node3D {
	GDCLASS(DebugDrawer, Node3D)

public:
	enum SphereResolution {
		LOWEST = 0,
		LOW = 1,
		MEDIUM = 2,
		HIGH = 3,
		HIGHEST = 4,
	};

private:
	bool global = true;

	LateProcesser* late_processer = nullptr;
	DebugCubeDrawer* cube_drawer = nullptr;
	DebugLineDrawer* line_drawer = nullptr;
	DebugSphereDrawer* sphere_drawer = nullptr;
	DebugCylinderDrawer* cylinder_drawer = nullptr;
	DebugTextDrawer* text_drawer = nullptr;

	bool _has_drawers() const {
		return cube_drawer != nullptr;
	}

protected:
	static void _bind_methods();

public:
	void set_global(bool p_global);
	bool is_global() const;

	void _enter_tree() override;
	void _exit_tree() override;
	void _process(double p_delta) override;
	PackedStringArray _get_configuration_warnings() const override;

	void late_process(double p_delta);

	void draw_cube(const Vector3& p_center, const Vector3& p_size, const Color& p_line_color, const Color& p_fill_color = Color(0, 0, 0, 0));
	void draw_cube_rotated(const Vector3& p_center, const Vector3& p_size, const Quaternion& p_rotation, const Color& p_line_color, const Color& p_fill_color = Color(0, 0, 0, 0));
	void draw_line(const Vector3& p_start, const Vector3& p_end, const Color& p_line_color);
	void draw_path(const PackedVector3Array& p_path, const Color& p_line_color);
	void draw_vector(const Vector3& p_origin, const Vector3& p_vector, const Color& p_line_color, bool p_draw_arrow = true);
	void draw_sphere(const Vector3& p_center, real_t p_radius, SphereResolution p_resolution, const Color& p_line_color, const Color& p_fill_color = Color(0, 0, 0, 0));
	void draw_cylinder(const Vector3& p_center, real_t p_height, real_t p_top_radius, real_t p_bottom_radius, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color = Color(0, 0, 0, 0));
	void draw_cylinder_rotated(const Vector3& p_center, real_t p_height, real_t p_top_radius, real_t p_bottom_radius, const Quaternion& p_rotation, int32_t p_resolution, const Color& p_line_color, const Color& p_fill_color = Color(0, 0, 0, 0));
	void draw_text(const String& p_string, const Vector3& p_position, const Color& p_text_color = Color(1, 1, 1, 1), const Color& p_outline_color = Color(0, 0, 0, 1), int32_t p_font_size = 32, HorizontalAlignment p_horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER, VerticalAlignment p_vertical_alignment = VERTICAL_ALIGNMENT_CENTER);
};

VARIANT_ENUM_CAST(DebugDrawer::SphereResolution);
