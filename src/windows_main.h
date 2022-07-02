#pragma once
#include <windows.h>
#include <comdef.h>
#include <stdio.h>
#include "game.h"

struct Windows_Game_Code
{
    HMODULE gameCodeDLL;
    FILETIME dllLastWriteTime;
    game_update_and_render *UpdateAndRender;
    // game_get_sound_samples *GetSoundSamples;
    bool isValid;
};

struct Windows_Replay_Buffer
{
    HANDLE memoryMap;
    HANDLE fileHandle;
    char filename[ MAX_PATH ];
    void *memoryBlock;
};

struct Windows_State
{
    char exePath[ MAX_PATH ];
    char *exeFilename;

    u64 totalSize;
    void *gameMemoryBlock;
    Windows_Replay_Buffer replayBuffers[ 4 ];

    HANDLE recordingHandle;
    int inputRecordingIndex;

    HANDLE playbackHandle;
    int inputPlayingIndex;
};

#ifdef INTERNAL
    #define __FILENAME__   ( strrchr( __FILE__, '\\' ) ? strrchr( __FILE__, '\\' ) + 1 : __FILE__ )
    #define Log( message ) printf( "[%s:%d] %s\n", __FILENAME__, __LINE__, message )
    #define LogResult( hresult, message )                                                            \
        {                                                                                            \
            _com_error log_error( hresult );                                                         \
            wchar_t log_wideError[ 256 ] = {};                                                       \
            MultiByteToWideChar( CP_ACP, 0, log_error.ErrorMessage(), -1, log_wideError, 256 );      \
            char log_errorMessage[ 256 ] = {};                                                       \
            WideCharToMultiByte( CP_UTF8, 0, log_wideError, -1, log_errorMessage, 256, NULL, NULL ); \
            printf( "[%s:%d] %s | %s\n", __FILENAME__, __LINE__, message, log_errorMessage );        \
        }
    #define LogResultF( hresult, format, ... )                                                  \
        {                                                                                       \
            _com_error error( hresult );                                                        \
            char messageBuffer[ 256 ] = {};                                                     \
            _snprintf_s( messageBuffer, 256, format, __VA_ARGS__ );                             \
            wchar_t wideError[ 256 ] = {};                                                      \
            MultiByteToWideChar( CP_ACP, 0, error.ErrorMessage(), -1, wideError, 256 );         \
            char errorMessage[ 256 ] = {};                                                      \
            WideCharToMultiByte( CP_UTF8, 0, wideError, -1, errorMessage, 256, NULL, NULL );    \
            printf( "[%s:%d] %s | %s\n", __FILENAME__, __LINE__, messageBuffer, errorMessage ); \
        }
#else
    #define Log( message )
    #define LogResult( hresult, message )
    #define LogResultF( hresult, format, args... )
#endif

DEBUG_PLATFORM_READ_ENTIRE_FILE( DebugPlatformReadEntireFile );
DEBUG_PLATFORM_FREE_FILE_MEMORY( DebugPlatformFreeFileMemory );
DEBUG_PLATFORM_WRITE_ENTIRE_FILE( DebugPlatformWriteEntireFile );
