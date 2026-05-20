#include "register_types.h"
#include "ClimbCore.h"

#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_climb_core(ModuleInitializationLevel level)
{
    if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<ClimbCore>();
}

void uninitialize_climb_core(ModuleInitializationLevel level)
{
    if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
GDExtensionBool GDE_EXPORT climb_core_library_init(GDExtensionInterfaceGetProcAddress get_proc_address,
                                                   GDExtensionClassLibraryPtr library,
                                                   GDExtensionInitialization *initialization)
{
    GDExtensionBinding::InitObject init_obj(get_proc_address, library, initialization);

    init_obj.register_initializer(initialize_climb_core);
    init_obj.register_terminator(uninitialize_climb_core);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}