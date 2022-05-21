#pragma once
#include <windows.h>
#include "game.h"

struct Windows_Game_Code
{
    HMODULE gameCodeDLL;
    FILETIME dllLastWriteTime;
    game_init *Init;
    game_update_and_render *UpdateAndRender;
    game_window_changed *WindowChanged;
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
