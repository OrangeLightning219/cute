#pragma once
#include "shared.h"

#ifndef UNITY_BUILD
#endif

struct Game_Button_State
{
    int halfTransitionCount;
    bool endedDown;
};

struct Game_Controller_Input
{
    bool isAnalog;
    bool isConnected;

    float32 leftStickAverageX, leftStickAverageY;
    float32 rightStickAverageX, rightStickAverageY;
    float32 leftTriggerAverage;
    float32 rightTriggerAverage;

    union
    {
        Game_Button_State buttons[ 14 ];
        struct
        {
            Game_Button_State up;
            Game_Button_State down;
            Game_Button_State left;
            Game_Button_State right;
            Game_Button_State a;
            Game_Button_State b;
            Game_Button_State x;
            Game_Button_State y;
            Game_Button_State l1;
            Game_Button_State r1;
            Game_Button_State l3;
            Game_Button_State r3;
            Game_Button_State start;
            Game_Button_State back;
        };
    };
};

struct Game_Input
{
    Game_Button_State mouseButtons[ 5 ];
    s32 mouseX;
    s32 mouseY;
    s32 mouseZ;

    float32 dtForUpdate;

    Game_Controller_Input controllers[ 5 ];
};

inline Game_Controller_Input *getController( Game_Input *input, int controllerIndex )
{
    Assert( controllerIndex < ( int ) ArrayCount( input->controllers ) );
    return &input->controllers[ controllerIndex ];
}

#ifdef INTERNAL
struct Debug_Read_File_Result
{
    u32 contentSize;
    void *content;
};

    #define DEBUG_PLATFORM_READ_ENTIRE_FILE( name ) Debug_Read_File_Result name( char *filename )
typedef DEBUG_PLATFORM_READ_ENTIRE_FILE( debug_platform_read_entire_file );

    #define DEBUG_PLATFORM_FREE_FILE_MEMORY( name ) void name( void *fileMemory )
typedef DEBUG_PLATFORM_FREE_FILE_MEMORY( debug_platform_free_file_memory );

    #define DEBUG_PLATFORM_WRITE_ENTIRE_FILE( name ) bool name( char *filename, u32 memorySize, void *memory )
typedef DEBUG_PLATFORM_WRITE_ENTIRE_FILE( debug_platform_write_entire_file );

#endif

#define RENDERER_INIT( name ) bool name()
typedef RENDERER_INIT( renderer_init );

#define RENDERER_BEGIN_FRAME( name ) void name( Matrix4 viewProjectionMatrix )
typedef RENDERER_BEGIN_FRAME( renderer_begin_frame );

#define RENDERER_END_FRAME( name ) void name()
typedef RENDERER_END_FRAME( renderer_end_frame );

#define RENDERER_DRAW_CUBE( name ) void name( Transform transform, Vector4 color )
typedef RENDERER_DRAW_CUBE( renderer_draw_cube );

struct Platform_Api
{
#ifdef INTERNAL
    debug_platform_read_entire_file *DebugReadEntireFile;
    debug_platform_write_entire_file *DebugWriteEntireFile;
    debug_platform_free_file_memory *DebugFreeFileMemory;
#endif

    renderer_init *RendererInit;
    renderer_begin_frame *RendererBeginFrame;
    renderer_end_frame *RendererEndFrame;
    renderer_draw_cube *RendererDrawCube;
};

extern Platform_Api platform;

struct Game_Memory
{
    bool isInitialized;
    u64 permanentStorageSize;
    void *permanentStorage;
    u64 transientStorageSize;
    void *transientStorage;

    Platform_Api platformApi;
};

// services for the platform layer
// @TODO: Get rid of windowWidth and windowHeight from here.
#define GAME_UPDATE_AND_RENDER( name ) void name( Game_Memory *memory, Game_Input *input, int windowWidth, int windowHeight )
typedef GAME_UPDATE_AND_RENDER( game_update_and_render );

typedef size_t memory_index;

struct Memory_Arena
{
    memory_index size;
    u8 *base;
    memory_index used;
};

#define PushStruct( arena, type )       ( type * ) _PushSize( arena, sizeof( type ) )
#define PushArray( arena, count, type ) ( type * ) _PushSize( arena, count * sizeof( type ) )
internal void *_PushSize( Memory_Arena *arena, memory_index size )
{
    Assert( arena->used + size <= arena->size );
    void *result = arena->base + arena->used;
    arena->used += size;
    return result;
}
