#pragma once

#include <godot_cpp/classes/node3d.hpp>

#include <cstdint>

using namespace godot;

class DebugElementDrawer : public Node3D {
	GDCLASS(DebugElementDrawer, Node3D)

protected:
	static void _bind_methods() {}

	int64_t max_shape_instances = 0;

	void _push_too_many_shapes_warning() const;

public:
	void _enter_tree() override;
};
