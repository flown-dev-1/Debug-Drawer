#include "nodes/debug_text_drawer.h"

#include <godot_cpp/classes/base_material3d.hpp>

void DebugTextDrawer::_enter_tree() {
	DebugElementDrawer::_enter_tree();

	labels.reserve(max_shape_instances);
}

void DebugTextDrawer::draw(const String& p_string, const Vector3& p_position, const Color& p_text_color, const Color& p_outline_color, int32_t p_font_size, HorizontalAlignment p_horizontal_alignment, VerticalAlignment p_vertical_alignment) {
	if (p_string.is_empty() || p_font_size <= 0 || (p_text_color.a <= 0.0f && p_outline_color.a <= 0.0f)) {
		return;
	}

	if (count >= max_shape_instances) {
		push_too_many_shapes_warning();
		return;
	}

	if (static_cast<size_t>(count) >= labels.size()) {
		labels.push_back(create_label());
	}

	Label3D* label = labels[count];

	if (label->get_text() != p_string) {
		label->set_text(p_string);
	}

	label->set_position(p_position);
	label->set_horizontal_alignment(p_horizontal_alignment);
	label->set_vertical_alignment(p_vertical_alignment);
	label->set_font_size(p_font_size);
	label->set_modulate(p_text_color);
	label->set_outline_modulate(p_outline_color);

	count++;
}

void DebugTextDrawer::reset() {
	while (labels.size() > static_cast<size_t>(count)) {
		memdelete(labels.back());
		labels.pop_back();
	}

	count = 0;
}

Label3D* DebugTextDrawer::create_label() {
	Label3D* label = memnew(Label3D);

	label->set_billboard_mode(BaseMaterial3D::BILLBOARD_ENABLED);
	label->set_draw_flag(Label3D::FLAG_DOUBLE_SIDED, false);
	label->set_alpha_cut_mode(Label3D::ALPHA_CUT_DISCARD);

	add_child(label);

	return label;
}
