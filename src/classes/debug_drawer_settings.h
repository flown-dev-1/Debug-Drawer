#pragma once

#include <cstdint>

namespace DebugDrawerSettings {

constexpr const char* MAX_SHAPE_INSTANCES_PATH = "debug_drawer/max_shape_instances";
constexpr int64_t MAX_SHAPE_INSTANCES_DEFAULT = 1024;

void register_settings();
int64_t get_max_shape_instances();

}
