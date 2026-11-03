# Debug Drawer (GDExtension)
![Logo](https://github.com/flown-dev-1/DebugDrawer/blob/main/banner.png)

A 3D debug shape drawer for Godot, with a simple immediate-mode style API. This is the C++ (GDExtension) version of the [Debug Drawer addon](https://github.com/flown-dev-1/DebugDrawer). It has the same API as the GDScript addon and is faster.

Requires Godot 4.3 or newer.

# Installation
1. Download the build for your platforms from the GitHub Releases page.
2. Create an `res://addons` folder in your Godot project if you do not already have one.
3. Copy the `debug_drawer` folder into your `res://addons` folder, so you have `res://addons/debug_drawer/debug_drawer.gdextension`.
4. Restart the Godot editor.

There is no plugin to enable. The `DebugDrawer` node and the `GlobalDebugDrawer` instance are available as soon as the extension loads.

> [!NOTE]
> If you are switching from the GDScript addon, remove the `addons/debug_drawer` folder and the `GlobalDebugDrawer` autoload (Project Settings > Globals) first. Scenes that used the old `debug_drawer.gd` script need their `DebugDrawer` node re-added as the built-in `DebugDrawer` type. `DebugSphereDrawer.SphereResolution` is now `DebugDrawer.SphereResolution`.

# Setup

To be able to draw shapes, you must first add a `DebugDrawer` node to your scene tree. This node is required because your shapes need to exist somewhere in the scene tree and depending on your setup the specific location might be important. If you are unsure where to put this node, I recommend adding it as a child of your root 3D node.

You will notice that the `DebugDrawer` node has a `global` toggle. The effects of this setting are covered in [Multiple Debug Drawers](#multiple-debug-drawers). For now, keep it turned on and do not add more than one `DebugDrawer` to your scene tree.

# Drawing Shapes

Debug Drawer uses an immediate-mode style API for drawing its shapes. This means that all of your drawing functions should be placed in `_process` and called on every frame that you want the shape to be visible.

> [!WARNING]
> Because your debug shapes are added as a child of the `DebugDrawer` node, they will be impacted by its transform. It is recommended to keep the `DebugDrawer` at the world origin.

With that, drawing a cube is as easy as calling `draw_cube` on `GlobalDebugDrawer`.

```gdscript
func _process(_delta: float) -> void:
  # Draws a red, wireframe, unit cube at the origin
  GlobalDebugDrawer.draw_cube(Vector3.ZERO, Vector3.ONE, Color.RED)
```

Most shape functions also allow you to specify a fill color on top of your wireframe color, here is an example of how to do that.

```gdscript
func _process(_delta: float) -> void:
  # Draws a red, partially filled, unit cube at the origin
  GlobalDebugDrawer.draw_cube(Vector3.ZERO, Vector3.ONE, Color.RED, Color(1.0, 0.0, 0.0, 0.2))
```

Debug Drawer provides a variety of different functions for drawing shapes, including:

| Function Name | Effect |
| --- | --- |
| `draw_cube` | Draws a cube. |
| `draw_cube_rotated` | Draws a cube rotated around its center. |
| `draw_line` | Draws a line from one point to another. |
| `draw_path` | Draws a path made up of one or more lines. |
| `draw_vector` | Draws an arrow given an origin point and a vector. |
| `draw_sphere` | Draws an (ico) sphere. |
| `draw_cylinder` | Draws a cylinder. |
| `draw_cylinder_rotated` | Draws a cylinder rotated around its center. |
| `draw_text` | Draws a given string with option for billboarding. |

Every function is documented in the editor's built-in help (search for `DebugDrawer` or `GlobalDebugDrawer`).

The maximum number of each shape per frame is set by `debug_drawer/max_shape_instances` in Project Settings (default 1024).

# Multiple Debug Drawers

Debug Drawer allows you to have multiple `DebugDrawer` nodes with the caveat that only one can be global. Global `DebugDrawer`s can be accessed from any script via the `GlobalDebugDrawer` instance, whereas non-global `DebugDrawer`s require a reference. For example:

```gdscript
@onready var local_debug_drawer: DebugDrawer = $DebugDrawer

func _process(_delta: float) -> void:
  # Drawing a cube via a global debug drawer
  GlobalDebugDrawer.draw_cube(Vector3.ZERO, Vector3.ONE, Color.RED)

  # Drawing a cube via a non-global debug drawer
  local_debug_drawer.draw_cube(Vector3.ZERO, Vector3.ONE, Color.RED)
```

# Building from source

1. Clone this repository and fetch godot-cpp: `git submodule update --init`
2. Install [SCons](https://scons.org/) and a C++17 compiler.
3. Run `scons` (debug build) or `scons target=template_release` from the repository root.

The library is written to `project/addons/debug_drawer/bin/<platform>/`. Open `project/` in Godot and run the main scene to test it. Press Space to toggle the stress test.

The `make_build.yml` workflow (run it from the GitHub Actions tab) builds every platform and uploads the libraries as a zip. To make a release, put them in `project/addons/debug_drawer/bin/` and zip the `debug_drawer` folder.

To generate `compile_commands.json` for your IDE, run `scons compiledb=yes`.

## Source layout

| File | Purpose |
| --- | --- |
| `src/autoloads/` | The `GlobalDebugDrawer` instance. |
| `src/classes/` | Project settings and mesh-building helpers. |
| `src/nodes/` | The `DebugDrawer` node, the text drawer and the late processer. |
| `src/nodes/debug_shape_drawers/` | One internal node per shape type, each batching its shapes into MultiMeshes. |
| `src/shaders/` | The cylinder shaders, embedded as strings. |
| `src/register_types.cpp` | Class registration, project setting and instance setup. |
| `doc_classes/` | Class reference shown in the editor's help. |

# Getting Help
* If you are struggling with using Debug Drawer, feel free to ask for help in my [Discord server](https://flowndev.com/discord).
* If you have found a bug or issue with the addon, please create an [issue](https://github.com/flown-dev-1/Debug-Drawer/issues).
