#pragma once

#include "glad.h"
#include "utils/utils.h"
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

struct OpenGL_Info
{
    char *vendor;
    char *renderer;
    char *version;
    char *shadingLanguageVersion;
    char **extensions;
    int extensionsCount;

    bool GL_EXT_texture_sRGB;
    bool GL_ARB_framebuffer_sRGB;
};

struct Platform_Api
{
#ifdef INTERNAL
    debug_platform_read_entire_file *DebugReadEntireFile;
    debug_platform_write_entire_file *DebugWriteEntireFile;
    debug_platform_free_file_memory *DebugFreeFileMemory;
#endif

    PFNGLENABLEPROC glEnable;
    PFNGLVIEWPORTPROC glViewport;
    PFNGLCLEARCOLORPROC glClearColor;
    PFNGLCLEARPROC glClear;
    PFNGLGENBUFFERSPROC glGenBuffers;
    PFNGLBINDBUFFERPROC glBindBuffer;
    PFNGLBUFFERDATAPROC glBufferData;
    PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
    PFNGLBUFFERSUBDATAPROC glBufferSubData;
    PFNGLGETINTEGERVPROC glGetIntegerv;

    PFNGLCREATESHADERPROC glCreateShader;
    PFNGLSHADERSOURCEPROC glShaderSource;
    PFNGLCOMPILESHADERPROC glCompileShader;
    PFNGLGETSHADERIVPROC glGetShaderiv;
    PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    PFNGLCREATEPROGRAMPROC glCreateProgram;
    PFNGLATTACHSHADERPROC glAttachShader;
    PFNGLLINKPROGRAMPROC glLinkProgram;
    PFNGLGETPROGRAMIVPROC glGetProgramiv;
    PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
    PFNGLDELETESHADERPROC glDeleteShader;
    PFNGLUSEPROGRAMPROC glUseProgram;
    PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
    PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
    PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    PFNGLUNIFORM1FPROC glUniform1f;
    PFNGLUNIFORM2FPROC glUniform2f;
    PFNGLUNIFORM3FPROC glUniform3f;
    PFNGLUNIFORM4FPROC glUniform4f;
    PFNGLUNIFORM1IPROC glUniform1i;
    PFNGLUNIFORM2IPROC glUniform2i;
    PFNGLUNIFORM3IPROC glUniform3i;
    PFNGLUNIFORM4IPROC glUniform4i;
    PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

    PFNGLGENTEXTURESPROC glGenTextures;
    PFNGLBINDTEXTUREPROC glBindTexture;
    PFNGLTEXPARAMETERIPROC glTexParameteri;
    PFNGLTEXIMAGE2DPROC glTexImage2D;
    PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
    PFNGLACTIVETEXTUREPROC glActiveTexture;
    PFNGLBLENDFUNCPROC glBlendFunc;

    PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
    PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    PFNGLDRAWARRAYSPROC glDrawArrays;

    PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
    PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl;
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
#define GAME_UPDATE_AND_RENDER( name ) void name( Game_Memory *memory, Game_Input *input )
typedef GAME_UPDATE_AND_RENDER( game_update_and_render );

#define GAME_INIT( name ) void name( Game_Memory *memory, int windowWidth, int windowHeight )
typedef GAME_INIT( game_init );

#define GAME_WINDOW_CHANGED( name ) void name( Game_Memory *memory, int windowWidth, int windowHeight )
typedef GAME_WINDOW_CHANGED( game_window_changed );

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
