#pragma once

#include <godot_cpp/classes/resource_format_loader.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/variant/string.hpp>

#include <stdint.h>

using namespace godot;

class OpusLoader : public ResourceFormatLoader
{
    GDCLASS( OpusLoader, ResourceFormatLoader )

protected:
    static void _bind_methods();

public:
    virtual PackedStringArray _get_recognized_extensions() const override;
    virtual bool _handles_type( const StringName &type ) const override;
    virtual String _get_resource_type( const String &p_path ) const override;
    virtual Variant _load( const String &path, const String &original_path = String(),
                           bool use_sub_threads = true,
                           int32_t cache_mode = CACHE_MODE_REUSE ) const override;
};
