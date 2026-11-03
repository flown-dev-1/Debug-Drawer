@tool
extends Node

#region state
var _stress_testing: bool = false

#region node_events
func _process(_delta: float) -> void:
	GlobalDebugDrawer.draw_cube_rotated(Vector3.ZERO, Vector3.ONE, Quaternion.from_euler(Vector3(0.5, 0.5, 0.0)), Color.RED, Color(1.0, 0.0, 0.0, 0.2))
	GlobalDebugDrawer.draw_cube(Vector3(0.0, 0.0, -2.0), Vector3(2.0, 1.0, 1.0), Color.AQUA, Color(0.0, 1.0, 1.0, 0.2))
	
	GlobalDebugDrawer.draw_line(Vector3(3.0, 0.0, 0.0), Vector3(4.0, 2.5, 1.0), Color.YELLOW)
	GlobalDebugDrawer.draw_line(Vector3(4.0, 2.5, 1.0), Vector3(4.0, 2.5, 0.0), Color.YELLOW)
	
	GlobalDebugDrawer.draw_sphere(Vector3(-2.5, 0.0, 0.0), sin(Time.get_ticks_msec() / 1000.0), DebugDrawer.SphereResolution.MEDIUM, Color.GREEN, Color(0.0, 1.0, 0.0, 0.2))
	
	GlobalDebugDrawer.draw_cylinder(Vector3(-6.0, 0.0, 0.0), 2.0, 0.5, 1.0, 20, Color.MAGENTA, Color(1.0, 0.0, 1.0, 0.2))
	GlobalDebugDrawer.draw_cylinder_rotated(Vector3(-6.0, 0.0, -3.0), 1.0, 0.5, sin(Time.get_ticks_msec() / 1000.0) + 1.0, Quaternion.from_euler(Vector3(-0.5, 0.0, 0.0)), 20, Color.WHITE, Color(1.0, 1.0, 1.0, 0.2))
	
	GlobalDebugDrawer.draw_vector(Vector3(3.0, 0.0, -2.0), Vector3(1.0, 0.5, -2.0), Color.CORAL)
	
	GlobalDebugDrawer.draw_path(PackedVector3Array([
		Vector3(3.0, 0.0, -6.0),
		Vector3(4.0, 1.0, -6.0),
		Vector3(4.0, 0.0, -6.0),
		Vector3(4.0, 2.0, -7.0)
	]), Color.BLUE_VIOLET)
	
	GlobalDebugDrawer.draw_text("Debug\nDrawer", Vector3(0.0, 3.0, 0.0), Color.ANTIQUE_WHITE, Color.SEA_GREEN, 64)
	
	#GlobalDebugDrawer.draw_sphere(Vector3(-1.75, 0.0, 0.0), 1.0, DebugDrawer.SphereResolution.MEDIUM, Color.GREEN, Color(0.0, 1.0, 0.0, 0.2))
	#
	#GlobalDebugDrawer.draw_cube_rotated(Vector3(2.25, 0.0, 0.0), Vector3(2.0, 1.0, 1.0), Quaternion.from_euler(Vector3(0.5, 0.0, -0.2)), Color.AQUA, Color(0.0, 1.0, 1.0, 0.2))
	
	if !Engine.is_editor_hint():
		if Input.is_action_just_pressed("toggle_stress_test"):
			_stress_testing = !_stress_testing
		
		if _stress_testing:
			for i: int in range(1000):
				GlobalDebugDrawer.draw_cylinder( \
					Vector3(randf_range(-10.0, 10.0), randf_range(-10.0, 10.0), randf_range(-10.0, 10.0)), \
					randf_range(0.0, 10.0), \
					randf_range(0.0, 1.0), \
					randf_range(0.0, 1.0), \
					randi_range(0, 40), \
					Color(randf_range(0.0, 1.0), randf_range(0.0, 1.0), randf_range(0.0, 1.0), 1.0), \
					Color(randf_range(0.0, 1.0), randf_range(0.0, 1.0), randf_range(0.0, 1.0), randf_range(0.0, 1.0)) \
				)
