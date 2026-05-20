extends Node2D

var pose = null
var scale_factor := 180.0
var origin := Vector2(500, 500)
var core = ClimbCore.new()
var draggable_joints := [8, 12, 16, 20] # LeftHand, RightHand, LeftFoot, RightFoot
var dragged_joint := -1
var drag_radius := 14.0
@onready var save_pose_button: Button = $"SavePoseButton"
@onready var load_pose_button: Button = $"LoadPoseButton"
@onready var pose_id_input: LineEdit = $"PoseIDInput"
const JOINT_NAMES := [
	"Head", "Neck", "Chest", "Spine", "Pelvis",
	"LeftShoulder", "LeftElbow", "LeftWrist", "LeftHand",
	"RightShoulder", "RightElbow", "RightWrist", "RightHand",
	"LeftHip", "LeftKnee", "LeftAnkle", "LeftFoot",
	"RightHip", "RightKnee", "RightAnkle", "RightFoot"
]



func _ready():
	save_pose_button.pressed.connect(_on_save_pose_button_pressed)
	load_pose_button.pressed.connect(_on_load_pose_button_pressed)

func initialize_user():
	pose = core.create_default_pose()
	queue_redraw()

func _draw():
	if pose == null:
		return

	var joints = pose.get("joints", [])
	var limbs = pose.get("limbs", [])

	for limb in limbs:
		var start_index = limb["startJointIndex"]
		var end_index = limb["endJointIndex"]

		if start_index < 0 or start_index >= joints.size():
			continue

		if end_index < 0 or end_index >= joints.size():
			continue

		var start_joint = joints[start_index]
		var end_joint = joints[end_index]

		draw_line(to_screen(start_joint), to_screen(end_joint), Color.WHITE, 4.0)

	for joint in joints:
		draw_circle(to_screen(joint), 6.0, Color.DODGER_BLUE)

	if pose.has("centerOfGravity") and pose["centerOfGravity"] != null:
		var cog = pose["centerOfGravity"]
		draw_circle(to_screen(cog), 9.0, Color.RED)

func to_screen(point) -> Vector2:
	return Vector2(
		origin.x + point["x"] * scale_factor,
		origin.y - point["y"] * scale_factor
	)
	
func translate_pose(x, y, z):
	if pose == null:
		return

	pose = core.translate_pose(pose, x, y, z)
	queue_redraw()
	
func to_world(screen_pos: Vector2) -> Vector2:
	return Vector2(
		(screen_pos.x - origin.x) / scale_factor,
		-(screen_pos.y - origin.y) / scale_factor
	)

func _input(event):
	if pose == null:
		return

	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT:
		if event.pressed:
			dragged_joint = get_joint_at_screen_position(event.position)
		else:
			dragged_joint = -1

	if event is InputEventMouseMotion and dragged_joint != -1:
		var world_pos = to_world(event.position)
		move_joint_to(dragged_joint, world_pos.x, world_pos.y)

func get_joint_at_screen_position(screen_pos: Vector2) -> int:
	var joints = pose.get("joints", [])

	for joint_type in draggable_joints:
		if joint_type < 0 or joint_type >= joints.size():
			continue

		var joint = joints[joint_type]
		var joint_screen_pos = to_screen(joint)

		if joint_screen_pos.distance_to(screen_pos) <= drag_radius:
			return joint_type

	return -1

func move_joint_to(joint_type: int, x: float, y: float):
	if pose == null:
		print("Initialize user first.")
		return

	pose = core.move_joint_to(pose, joint_type, x, y)
	queue_redraw()
	
func get_pose_save_data() -> Dictionary:
	var pose_data := {}

	pose_data["body"] = core.get_body_data()
	pose_data["limbs"] = core.get_limb_data()
	pose_data["joints"] = []

	for joint_name in JOINT_NAMES:
		var pos: Vector3 = core.get_joint_position(joint_name)

		pose_data["joints"].append({
			"type": joint_name,
			"position": {
				"x": pos.x,
				"y": pos.y,
				"z": pos.z
			}
		})
	var cog: Vector3 = core.get_center_of_gravity()
	pose_data["centerOfGravity"] = {
		"x": cog.x,
		"y": cog.y,
		"z": cog.z
	}

	return pose_data

func load_pose_data(pose_data: Dictionary) -> void:
	print("Applying pose data")

	core.set_body_data(pose_data["body"])

	for i in range(pose_data["joints"].size()):
		var joint = pose_data["joints"][i]
		var joint_type: String = JOINT_NAMES[i]
		var new_pos := Vector3(joint["x"], joint["y"], joint.get("z", 0.0))
		core.set_joint_position(joint_type, new_pos)

	# IMPORTANT: use the normalized pose directly
	pose = pose_data
	queue_redraw()
	print("Redraw queued")

func save_pose_to_api() -> void:
	print("Saving pose to API...")

	var request := HTTPRequest.new()
	add_child(request)

	request.request_completed.connect(_on_pose_saved)

	var pose_data := get_pose_save_data()
	var json := JSON.stringify(pose_data)

	print("JSON being sent:")
	print(json)

	var error = request.request(
		"http://localhost:3000/poses",
		["Content-Type: application/json"],
		HTTPClient.METHOD_POST,
		json
	)

	print("Request error code: ", error)

func load_pose_from_api(pose_id: String) -> void:
	var request := HTTPRequest.new()
	add_child(request)

	request.request_completed.connect(_on_pose_loaded)

	request.request(
		"http://localhost:3000/poses/" + pose_id,
		[],
		HTTPClient.METHOD_GET
	)
	
func _on_pose_saved(result, response_code, headers, body) -> void:
	print("Status code: ", response_code)

	if response_code != 201:
		print("Failed to save pose")
		print(body.get_string_from_utf8())
		return

	var json_string: String = body.get_string_from_utf8()
	var saved_pose = JSON.parse_string(json_string)

	print("Pose saved with id: ", saved_pose["id"])
	pose_id_input.text = saved_pose["id"]

func _on_pose_loaded(result, response_code, headers, body) -> void:
	print("Load request completed")
	print("Status: ", response_code)

	var json_string: String = body.get_string_from_utf8()
	print(json_string)

	if response_code != 200:
		return

	var pose_data = JSON.parse_string(json_string)

	if pose_data == null:
		print("Failed to parse pose data")
		return

	pose_data = normalize_pose_data(pose_data)
	load_pose_data(pose_data)

func _on_save_pose_button_pressed() -> void:
	print("Save button pressed")
	save_pose_to_api()

func _on_load_pose_button_pressed() -> void:
	print("Load button pressed")
	print("Input visible: ", pose_id_input.visible)
	print("Input text: ", pose_id_input.text)

	if !pose_id_input.visible:
		pose_id_input.visible = true
		pose_id_input.grab_focus()
		return

	var pose_id: String = pose_id_input.text.strip_edges()

	if pose_id.is_empty():
		print("Enter a pose id first")
		return

	print("Loading pose id: ", pose_id)
	load_pose_from_api(pose_id)


func normalize_pose_data(api_pose: Dictionary) -> Dictionary:
	var normalized := {}

	normalized["body"] = api_pose["body"]
	normalized["centerOfGravity"] = api_pose.get("centerOfGravity", null)

	# Normalize joints
	var normalized_joints := []

	for joint in api_pose.get("joints", []):
		normalized_joints.append(joint["position"])

	normalized["joints"] = normalized_joints

	# Normalize limbs
	var normalized_limbs := []

	for limb in api_pose.get("limbs", []):
		normalized_limbs.append({
			"type": limb.get("type", ""),
			"startJointIndex": limb.get("startJointIndex", limb.get("start_joint_index", -1)),
			"endJointIndex": limb.get("endJointIndex", limb.get("end_joint_index", -1))
		})
	var cog = api_pose.get("centerOfGravity", null)

	if cog != null:
		normalized["centerOfGravity"] = {
			"x": cog["x"],
			"y": cog["y"],
			"z": cog.get("z", 0.0)
		}
	else:
		normalized["centerOfGravity"] = null

	normalized["limbs"] = normalized_limbs

	return normalized

func _on_initialize_user_pressed():
	initialize_user()


func _on_translate_up_pressed() -> void:
	translate_pose(0.0, 1, 0.0)


func _on_translate_down_pressed() -> void:
	translate_pose(0.0, -1, 0.0)
	
	
	
	
