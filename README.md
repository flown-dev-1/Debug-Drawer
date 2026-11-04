# Debug Drawer
![Logo](https://github.com/flown-dev-1/DebugDrawer/blob/main/banner.png)

A 3D debug shape drawer addon for Godot. It allows you to draw primitive shapes using a simple, immediate-mode style API.

# Installation
You can install the addon manually or through the asset store.

> [!NOTE]
> If your version of Godot is not supported by the latest version of the addon you may need to download the addon manually so that you can pick an older version.

## Manual Installation
1. Download the addon from the GitHub Releases page.
2. Unzip the addon folder.
3. Create an `res://addons` folder in your Godot project if you do not already have one.
4. Drag the addon folder into your `res://addons` folder.
5. Enable "Debug Drawer" in `Project Settings > Plugins`.

## Asset Store Installation
1. Navigate to the `Asset Store` tab in your Godot project.
2. Locate the "Debug Drawer" addon and click on it.
3. Click the download button.
4. Enable "Debug Drawer" in `Project Settings > Plugins`.

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

# Getting Help
* If you are struggling with using Debug Drawer, feel free to ask for help in my [Discord server](https://flowndev.com/discord).
* If you have found a bug or issue with the addon, please create an [issue](https://github.com/flown-dev-1/Debug-Drawer/issues).

# AI Use

AI was used to port my original GDScript version of this addon to GDExtension. The generated code has been thoroughly reviewed by me.

You can find the original GDScript addon [here](https://github.com/flown-dev-1/Debug-Drawer-GDScript).
