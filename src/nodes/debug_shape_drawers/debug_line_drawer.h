#pragma once

#include "classes/mesh_data.h"
#include "nodes/debug_shape_drawers/debug_shape_drawer.h"

class DebugLineDrawer : public DebugShapeDrawer {
	GDCLASS(DebugLineDrawer, DebugShapeDrawer)

	MultiMeshBatch lines;

	static MeshData create_lines_line();

protected:
	static void _bind_methods() {}

public:
	void _enter_tree() override;

	void draw(const Vector3& p_start, const Vector3& p_end, const Color& p_line_color);
	void finalize();
	void reset();
};
