#pragma once

namespace DebugDrawerShaders {

constexpr const char* CYLINDER_FILL = R"(shader_type spatial;
render_mode unshaded, cull_disabled;

void vertex() {
	float radius = VERTEX.y > 0.0 ? INSTANCE_CUSTOM.x : INSTANCE_CUSTOM.y;
	VERTEX.xz *= radius;
}

void fragment() {
	ALBEDO = COLOR.rgb;
	ALPHA = COLOR.a;
}
)";

}
