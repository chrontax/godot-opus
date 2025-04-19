#include <godot_cpp/classes/audio_stream_wav.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <string.h>

#include "OpusLoader.hpp"
#include "opus_types.h"
#include <opusfile.h>

void OpusLoader::_bind_methods()
{
}

PackedStringArray OpusLoader::_get_recognized_extensions() const
{
    PackedStringArray extensions;
    extensions.push_back( "opus" );
    return extensions;
}

bool OpusLoader::_handles_type( const StringName &type ) const
{
    return type == String( "AudioStream" );
}

String OpusLoader::_get_resource_type( const String &path ) const
{
    if ( path.to_lower().ends_with( ".opus" ) )
    {
        return "AudioStream";
    }
    return String();
}

Variant OpusLoader::_load( const String &path, const String &original_path, bool use_sub_threads,
                           int32_t cache_mode ) const
{
    // Read file data
    Ref<FileAccess> f = FileAccess::open( path, FileAccess::READ );
    if ( f.is_null() )
    {
        return Ref<Resource>();
    }
    PackedByteArray data = f->get_buffer( f->get_length() );

    // Decode with libopusfile
    int64_t size = data.size();
    int error;
    OggOpusFile *of = op_open_memory( (unsigned char *)data.ptr(), size, &error );
    if ( !of )
    {
        return Ref<Resource>();
    }

    // Get info
    const OpusHead *head = op_head( of, -1 );
    int channels = head->channel_count;
    int rate = 48000; // libopus fixed encoding rate

    // Decode all samples
    Vector<int16_t> pcm;
    pcm.resize( 0 );
    opus_int16 pcm_buf[960 * 6];
    while ( true )
    {
        int samples = op_read( of, pcm_buf, 960 * 6, nullptr );
        if ( samples <= 0 )
        {
            break;
        }
        for ( int i = 0; i < samples * channels; ++i )
        {
            pcm.push_back( pcm_buf[i] );
        }
    }
    op_free( of );

    // Create AudioStreamSample
    Ref<AudioStreamWAV> sample;
    sample.instantiate();
    sample->set_format( AudioStreamWAV::FORMAT_16_BITS );
    sample->set_stereo( channels == 2 );
    sample->set_mix_rate( rate );

    // Fill data
    PackedByteArray bytes;
    bytes.resize( pcm.size() * sizeof( opus_int16 ) );
    memcpy( bytes.ptrw(), pcm.ptr(), pcm.size() * sizeof( int16_t ) );
    sample->set_data( bytes );

    return sample;
}
