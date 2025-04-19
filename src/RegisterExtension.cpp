// Copied from godot-cpp/test/src and modified.

#include "gdextension_interface.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/defs.hpp"
#include "godot_cpp/godot.hpp"
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_format_loader.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "OpusLoader.hpp"

/// @file
/// Register our classes with Godot.

static Ref<OpusLoader> opus_loader;

namespace
{
    /// @brief Called by Godot to let us register our classes with Godot.
    ///
    /// @param p_level the level being initialized by Godot
    ///
    /// @see GDExtensionInit
    void initializeExtension( godot::ModuleInitializationLevel p_level )
    {
        godot::UtilityFunctions::print( "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" );
        if ( p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE )
        {
            return;
        }

        GDREGISTER_CLASS( OpusLoader );

        opus_loader.instantiate();
        godot::ResourceLoader::get_singleton()->add_resource_format_loader( opus_loader.ptr() );
    }

    /// @brief Called by Godot to let us do any cleanup.
    ///
    /// @see GDExtensionInit
    void uninitializeExtension( godot::ModuleInitializationLevel p_level )
    {
        if ( p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE )
        {
            return;
        }

        godot::ResourceLoader::get_singleton()->remove_resource_format_loader( opus_loader.ptr() );
        opus_loader.unref();
    }
}

extern "C"
{
    /// @brief This is the entry point for the shared library.
    ///
    /// @note The name of this function must match the "entry_symbol" in
    /// templates/template.*.gdextension.in
    ///
    /// @param p_get_proc_address the interface (need more info)
    /// @param p_library the library (need more info)
    /// @param r_initialization the intialization (need more info)
    ///
    /// @returns GDExtensionBool
    GDExtensionBool GDE_EXPORT GDExtensionInit(
        GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization *r_initialization )
    {
        {
            godot::GDExtensionBinding::InitObject init_obj( p_get_proc_address, p_library,
                                                            r_initialization );

            init_obj.register_initializer( initializeExtension );
            init_obj.register_terminator( uninitializeExtension );
            init_obj.set_minimum_library_initialization_level(
                godot::MODULE_INITIALIZATION_LEVEL_SCENE );

            return init_obj.init();
        }
    }
}
