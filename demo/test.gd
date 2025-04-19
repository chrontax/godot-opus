extends Node

func _ready() -> void:
	print(ResourceLoader.get_recognized_extensions_for_type("AudioStream"))
