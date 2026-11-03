#pragma once

#include <godot_cpp/classes/node.hpp>

using namespace godot;

class DebugDrawer;

class LateProcesser : public Node {
	GDCLASS(LateProcesser, Node)

	DebugDrawer* owner_drawer = nullptr;

protected:
	static void _bind_methods() {}

public:
	void set_owner_drawer(DebugDrawer* p_drawer) {
		owner_drawer = p_drawer;
	}

	void _enter_tree() override;
	void _process(double p_delta) override;
};
