#pragma once

#include "nodes/debug_element_drawer.h"

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label3d.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/string.hpp>

#include <vector>

class DebugTextDrawer : public DebugElementDrawer {
	GDCLASS(DebugTextDrawer, DebugElementDrawer)

	int32_t count = 0;
	std::vector<Label3D*> labels;

	Label3D* _create_label();

protected:
	static void _bind_methods() {}

public:
	void _enter_tree() override;

	void draw(const String& p_string, const Vector3& p_position, const Color& p_text_color, const Color& p_outline_color, int32_t p_font_size, HorizontalAlignment p_horizontal_alignment, VerticalAlignment p_vertical_alignment);
	void reset();
};
