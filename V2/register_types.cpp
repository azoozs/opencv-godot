/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "opencv.h"

void initialize_opencv_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
			return;
	}
	ClassDB::register_class<opencv>();
}

void uninitialize_opencv_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
			return;
	}
   // Nothing to do here in this example.
}
