#include "glad.c"
#include "windows_main.h"
#include <malloc.h>
#include <stdio.h>
#include <xinput.h>
#include <shellscalingapi.h>
// #define STB_IMAGE_IMPLEMENTATION

Platform_Api platform;

global_variable int windowWidth = 1920;
global_variable int windowHeight = 1920;
global_variable bool windowChanged = false;

#define X_INPUT_GET_STATE( name ) DWORD WINAPI name( DWORD dwUserIndex, XINPUT_STATE *pState )
#define X_INPUT_SET_STATE( name ) DWORD WINAPI name( DWORD dwUserIndex, XINPUT_VIBRATION *pVibration )
typedef X_INPUT_GET_STATE( x_input_get_state );
typedef X_INPUT_SET_STATE( x_input_set_state );

X_INPUT_GET_STATE( XInputGetStateStub )
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

X_INPUT_SET_STATE( XInputSetStateStub )
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub;
global_variable x_input_set_state *XInputSetState_ = XInputSetStateStub;

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

//@TODO: Probably move opengl stuff to separate files (or dll?)
// ================== OpenGL ========================
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB               0x2093
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB                 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef BOOL WINAPI wgl_swap_interval_ext( int interval );
global_variable wgl_swap_interval_ext *wglSwapIntervalEXT;

typedef HGLRC WINAPI wgl_create_context_attribs_arb( HDC hDC, HGLRC hShareContext, const int *attribList );
// ===================================================
global_variable bool globalRunning;
global_variable bool globalPause;
global_variable s64 globalPerformanceCounterFrequency;

DEBUG_PLATFORM_FREE_FILE_MEMORY( DebugPlatformFreeFileMemory )
{
    if ( fileMemory )
    {
        VirtualFree( fileMemory, 0, MEM_RELEASE );
    }
}

DEBUG_PLATFORM_READ_ENTIRE_FILE( DebugPlatformReadEntireFile )
{
    Debug_Read_File_Result result = {};
    HANDLE fileHandle = CreateFile( filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
    if ( fileHandle != INVALID_HANDLE_VALUE )
    {
        LARGE_INTEGER fileSize;
        if ( GetFileSizeEx( fileHandle, &fileSize ) )
        {
            u32 fileSize32 = SafeTruncateU64( fileSize.QuadPart );
            result.content = VirtualAlloc( 0, fileSize32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
            if ( result.content )
            {
                DWORD bytesRead;
                if ( ReadFile( fileHandle, result.content, fileSize32, &bytesRead, 0 ) && bytesRead == fileSize32 )
                {
                    result.contentSize = fileSize32;
                }
                else
                {
                    //@TODO: Logging
                    DebugPlatformFreeFileMemory( result.content );
                    result.content = 0;
                }
            }
            else
            {
                //@TODO: Logging
            }
        }
        else
        {
            //@TODO: Logging
        }
        CloseHandle( fileHandle );
    }
    else
    {
        //@TODO: Logging
    }
    return result;
}

DEBUG_PLATFORM_WRITE_ENTIRE_FILE( DebugPlatformWriteEntireFile )
{
    bool result = false;
    HANDLE fileHandle = CreateFile( filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0 );
    if ( fileHandle != INVALID_HANDLE_VALUE )
    {
        DWORD bytesWritten;
        if ( WriteFile( fileHandle, memory, memorySize, &bytesWritten, 0 ) )
        {
            result = bytesWritten == memorySize;
        }
        else
        {
            //@TODO: Logging
        }

        CloseHandle( fileHandle );
    }
    else
    {
        //@TODO: Logging
    }
    return result;
}

internal void WindowsGetExeFilename( Windows_State *state )
{
    GetModuleFileName( 0, state->exePath, sizeof( state->exePath ) );
    state->exeFilename = state->exePath;
    for ( char *scan = state->exePath; *scan; ++scan )
    {
        if ( *scan == '\\' )
        {
            state->exeFilename = scan + 1;
        }
    }
}

internal void WindowsBuildExePathFilename( Windows_State *state, char *filename, char *destination, int destinationCount )
{
    ConcatenateStrings( state->exePath, ( int ) ( state->exeFilename - state->exePath ),
                        filename, StringLength( filename ),
                        destination );
}

inline FILETIME WindowsGetLastWriteTime( char *filename )
{
    WIN32_FILE_ATTRIBUTE_DATA data;
    if ( GetFileAttributesEx( filename, GetFileExInfoStandard, &data ) )
    {
        return data.ftLastWriteTime;
    }
    return {};
}

internal Windows_Game_Code WindowsLoadGameCode( char *sourceDLLName, char *tempDllName )
{
    CopyFile( sourceDLLName, tempDllName, FALSE );
    Windows_Game_Code gameCode = {};
    gameCode.dllLastWriteTime = WindowsGetLastWriteTime( sourceDLLName );
    gameCode.gameCodeDLL = LoadLibrary( tempDllName );
    if ( gameCode.gameCodeDLL )
    {
        // gameCode.GetSoundSamples = ( game_get_sound_samples * ) GetProcAddress( gameCode.gameCodeDLL, "GameGetSoundSamples" );
        gameCode.UpdateAndRender = ( game_update_and_render * ) GetProcAddress( gameCode.gameCodeDLL, "GameUpdateAndRender" );
        gameCode.Init = ( game_init * ) GetProcAddress( gameCode.gameCodeDLL, "GameInit" );
        gameCode.WindowChanged = ( game_window_changed * ) GetProcAddress( gameCode.gameCodeDLL, "GameWindowChanged" );
        // gameCode.isValid = gameCode.GetSoundSamples && gameCode.UpdateAndRender;
        gameCode.isValid = gameCode.UpdateAndRender && gameCode.Init && gameCode.WindowChanged;
    }
    if ( !gameCode.isValid )
    {
        // gameCode.GetSoundSamples = 0;
        gameCode.UpdateAndRender = 0;
    }
    return gameCode;
}

internal void WindowsUnloadGameCode( Windows_Game_Code gameCode )
{
    if ( gameCode.gameCodeDLL )
    {
        FreeLibrary( gameCode.gameCodeDLL );
        gameCode.gameCodeDLL = 0;
    }
    gameCode.isValid = false;
    // gameCode.GetSoundSamples = 0;
    gameCode.UpdateAndRender = 0;
}

internal void WindowsLoadXInput()
{
    HMODULE xInputLibrary = LoadLibrary( "xinput1_4.dll" );

    if ( !xInputLibrary )
    {
        xInputLibrary = LoadLibrary( "xinput9_1_0.dll" );
    }

    if ( !xInputLibrary )
    {
        xInputLibrary = LoadLibrary( "xinput1_3.dll" );
    }

    if ( xInputLibrary )
    {
        XInputGetState = ( x_input_get_state * ) GetProcAddress( xInputLibrary, "XInputGetState" );
        XInputSetState = ( x_input_set_state * ) GetProcAddress( xInputLibrary, "XInputSetState" );
    }
}

internal void WindowsProcessKeyboardMessage( Game_Button_State *newState, bool isDown )
{
    if ( newState->endedDown != isDown )
    {
        newState->endedDown = isDown;
        ++newState->halfTransitionCount;
    }
}

internal void WindowsProcessXInputDigitalButton( DWORD XInputButtonState, Game_Button_State *oldState,
                                                 DWORD buttonBit, Game_Button_State *newState )
{
    newState->halfTransitionCount = oldState->endedDown != newState->endedDown ? 1 : 0;
    newState->endedDown = ( XInputButtonState & buttonBit ) == buttonBit;
}

internal float32 WindowsProcessXInputStickValue( SHORT stickValue, SHORT deadzone )
{
    if ( stickValue > deadzone )
    {
        return ( float32 ) stickValue / 32767.0f;
    }
    else if ( stickValue < -deadzone )
    {
        return ( float32 ) stickValue / 32768.0f;
    }
    return 0.0f;
}

internal float32 WindowsProcessXInputTriggerValue( BYTE triggerValue, BYTE deadzone )
{
    return triggerValue > deadzone ? ( float32 ) triggerValue / 255.0f : 0.0f;
}

internal void WindowsGetInputRecordingFileLocation( Windows_State *state, bool inputStream, int slotIndex, char *destination, int destinationCount )
{
    char temp[ 64 ];
    wsprintf( temp, "recording_%d_%s.input", slotIndex, inputStream ? "input" : "state" );
    WindowsBuildExePathFilename( state, temp, destination, destinationCount );
}

internal Windows_Replay_Buffer *WindowsGetReplayBuffer( Windows_State *state, int inputRecordingIndex )
{
    Assert( inputRecordingIndex < ArrayCount( state->replayBuffers ) );
    return &state->replayBuffers[ inputRecordingIndex ];
}

internal void WindowsBeginRecordingInput( Windows_State *state, int inputRecordingIndex )
{
    Windows_Replay_Buffer *replayBuffer = WindowsGetReplayBuffer( state, inputRecordingIndex );
    if ( !replayBuffer->memoryBlock )
    {
        return;
    }
    state->inputRecordingIndex = inputRecordingIndex;
    char filename[ MAX_PATH ];
    WindowsGetInputRecordingFileLocation( state, true, inputRecordingIndex, filename, sizeof( filename ) );
    state->recordingHandle = CreateFile( filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0 );
    CopyMemory( replayBuffer->memoryBlock, state->gameMemoryBlock, state->totalSize );
}

internal void WindowsEndRecordingInput( Windows_State *state )
{
    CloseHandle( state->recordingHandle );
    state->inputRecordingIndex = -1;
}

internal void WindowsBeginInputPlayback( Windows_State *state, int inputPlayingIndex )
{
    Windows_Replay_Buffer *replayBuffer = WindowsGetReplayBuffer( state, inputPlayingIndex );
    if ( !replayBuffer->memoryBlock )
    {
        return;
    }
    state->inputPlayingIndex = inputPlayingIndex;
    char filename[ MAX_PATH ];
    WindowsGetInputRecordingFileLocation( state, true, inputPlayingIndex, filename, sizeof( filename ) );
    state->playbackHandle = CreateFile( filename, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0 );
    CopyMemory( state->gameMemoryBlock, replayBuffer->memoryBlock, state->totalSize );
}

internal void WindowsEndInputPlayback( Windows_State *state )
{
    CloseHandle( state->playbackHandle );
    state->inputPlayingIndex = -1;
}

internal void WindowsRecordInput( Windows_State *state, Game_Input *input )
{
    DWORD bytesWritten;
    WriteFile( state->recordingHandle, input, sizeof( *input ), &bytesWritten, 0 );
}

internal void WindowsPlaybackInput( Windows_State *state, Game_Input *input )
{
    DWORD bytesRead;
    if ( ReadFile( state->playbackHandle, input, sizeof( *input ), &bytesRead, 0 ) )
    {
        if ( bytesRead == 0 )
        {
            int playingIndex = state->inputPlayingIndex;
            WindowsEndInputPlayback( state );
            WindowsBeginInputPlayback( state, playingIndex );
            ReadFile( state->playbackHandle, input, sizeof( *input ), &bytesRead, 0 );
        }
    }
}

inline LARGE_INTEGER WindowsGetWallClock()
{
    LARGE_INTEGER result;
    QueryPerformanceCounter( &result );
    return result;
}

inline float32 WindowsGetSecondsElapsed( LARGE_INTEGER start, LARGE_INTEGER end )
{
    return ( float32 ) ( end.QuadPart - start.QuadPart ) / ( float32 ) globalPerformanceCounterFrequency;
}

// void FramebufferSizeCallback( GLFWwindow *window, int width, int height )
// {
//     windowWidth = width;
//     windowHeight = height;
//     glViewport( 0, 0, width, height );
// }

// void MouseCallback( GLFWwindow *window, float64 mouseX, float64 mouseY )
// {
//     if ( firstMouse )
//     {
//         lastMouseX = ( float32 ) mouseX;
//         lastMouseY = ( float32 ) mouseY;
//         firstMouse = false;
//     }

//     float32 xOffset = ( float32 ) mouseX - lastMouseX;
//     float32 yOffset = lastMouseY - ( float32 ) mouseY;
//     lastMouseX = ( float32 ) mouseX;
//     lastMouseY = ( float32 ) mouseY;
//     CameraProcessMouse( &camera, xOffset, yOffset );
// }

internal OpenGL_Info OpenGLGetInfo()
{
    OpenGL_Info result = {};

    result.vendor = ( char * ) glad_glGetString( GL_VENDOR );
    result.renderer = ( char * ) glad_glGetString( GL_RENDERER );
    result.version = ( char * ) glad_glGetString( GL_VERSION );
    result.shadingLanguageVersion = ( char * ) glad_glGetString( GL_SHADING_LANGUAGE_VERSION );
    get_exts(); // from glad
    result.extensions = exts_i;
    result.extensionsCount = num_exts_i;

    for ( int i = 0; i < result.extensionsCount; ++i )
    {
        char *extension = result.extensions[ i ];
        int length = StringLength( extension );
        if ( StringsAreEqual( extension, length, "GL_EXT_texture_sRGB" ) ) { result.GL_EXT_texture_sRGB = true; }
        else if ( StringsAreEqual( extension, length, "GL_ARB_framebuffer_sRGB" ) ) { result.GL_ARB_framebuffer_sRGB = true; }
    }
    return result;
}

internal void WindowsInitOpenGL( HWND window )
{
    PIXELFORMATDESCRIPTOR desiredPixelFormat = {};
    desiredPixelFormat.nSize = sizeof( PIXELFORMATDESCRIPTOR );
    desiredPixelFormat.nVersion = 1;
    desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
    desiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    desiredPixelFormat.cColorBits = 32;
    desiredPixelFormat.cAlphaBits = 8;
    desiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

    HDC windowDC = GetDC( window );
    int suggestedPixelFormatIndex = ChoosePixelFormat( windowDC, &desiredPixelFormat );
    PIXELFORMATDESCRIPTOR suggestedPixelFormat;
    DescribePixelFormat( windowDC, suggestedPixelFormatIndex, sizeof( suggestedPixelFormat ), &suggestedPixelFormat );
    SetPixelFormat( windowDC, suggestedPixelFormatIndex, &suggestedPixelFormat );

    HGLRC openGLContext = wglCreateContext( windowDC );
    if ( wglMakeCurrent( windowDC, openGLContext ) )
    {
        HMODULE openGLDLL = LoadLibrary( "opengl32.dll" );
        if ( openGLDLL == NULL )
        {
            printf( "Could not load opengl32.dll!\n" );
            return;
        }
        PFNWGLGETPROCADDRESSPROC_PRIVATE wglGetProcAddress = ( PFNWGLGETPROCADDRESSPROC_PRIVATE ) GetProcAddress( openGLDLL, "wglGetProcAddress" );
        FreeLibrary( openGLDLL );
        wgl_create_context_attribs_arb *wglCreateContextAttribsARB = ( wgl_create_context_attribs_arb * ) wglGetProcAddress( "wglCreateContextAttribsARB" );
        if ( wglCreateContextAttribsARB )
        {
            int flags = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
#ifdef INTERNAL
            flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif

            HGLRC shareContext = 0;
            int attributes[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                WGL_CONTEXT_FLAGS_ARB, flags,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0
            };
            HGLRC modernContext = wglCreateContextAttribsARB( windowDC, shareContext, attributes );
            if ( modernContext )
            {
                wglMakeCurrent( windowDC, modernContext );
                wglDeleteContext( openGLContext );
                openGLContext = modernContext;
            }
        }

        if ( !gladLoadGL() )
        {
            printf( "Could not load OpenGL!\n" );
            return;
        }

        OpenGL_Info info = OpenGLGetInfo();

        // if(info.EXTENSION_NAME)
        // {
        //     // do something
        // }

        wglSwapIntervalEXT = ( wgl_swap_interval_ext * ) wglGetProcAddress( "wglSwapIntervalEXT" );
        if ( wglSwapIntervalEXT )
        {
            wglSwapIntervalEXT( 1 );
        }

        platform.glEnable = glad_glEnable;
        platform.glViewport = glad_glViewport;
        platform.glClearColor = glad_glClearColor;
        platform.glClear = glad_glClear;
        platform.glGenBuffers = glad_glGenBuffers;
        platform.glBindBuffer = glad_glBindBuffer;
        platform.glBufferData = glad_glBufferData;
        platform.glBindBufferRange = glad_glBindBufferRange;
        platform.glBufferSubData = glad_glBufferSubData;
        platform.glGetIntegerv = glad_glGetIntegerv;

        platform.glCreateShader = glad_glCreateShader;
        platform.glShaderSource = glad_glShaderSource;
        platform.glCompileShader = glad_glCompileShader;
        platform.glGetShaderiv = glad_glGetShaderiv;
        platform.glGetShaderInfoLog = glad_glGetShaderInfoLog;
        platform.glCreateProgram = glad_glCreateProgram;
        platform.glAttachShader = glad_glAttachShader;
        platform.glLinkProgram = glad_glLinkProgram;
        platform.glGetProgramiv = glad_glGetProgramiv;
        platform.glGetProgramInfoLog = glad_glGetProgramInfoLog;
        platform.glDeleteShader = glad_glDeleteShader;
        platform.glUseProgram = glad_glUseProgram;
        platform.glGetUniformBlockIndex = glad_glGetUniformBlockIndex;
        platform.glUniformBlockBinding = glad_glUniformBlockBinding;
        platform.glGetUniformLocation = glad_glGetUniformLocation;
        platform.glUniform1f = glad_glUniform1f;
        platform.glUniform2f = glad_glUniform2f;
        platform.glUniform3f = glad_glUniform3f;
        platform.glUniform4f = glad_glUniform4f;
        platform.glUniform1i = glad_glUniform1i;
        platform.glUniform2i = glad_glUniform2i;
        platform.glUniform3i = glad_glUniform3i;
        platform.glUniform4i = glad_glUniform4i;
        platform.glUniformMatrix4fv = glad_glUniformMatrix4fv;

        platform.glGenTextures = glad_glGenTextures;
        platform.glBindTexture = glad_glBindTexture;
        platform.glTexParameteri = glad_glTexParameteri;
        platform.glTexImage2D = glad_glTexImage2D;
        platform.glGenerateMipmap = glad_glGenerateMipmap;
        platform.glActiveTexture = glad_glActiveTexture;
        platform.glBlendFunc = glad_glBlendFunc;

        platform.glGenVertexArrays = glad_glGenVertexArrays;
        platform.glBindVertexArray = glad_glBindVertexArray;
        platform.glEnableVertexAttribArray = glad_glEnableVertexAttribArray;
        platform.glVertexAttribPointer = glad_glVertexAttribPointer;
        platform.glDrawArrays = glad_glDrawArrays;

        platform.glDebugMessageCallback = glad_glDebugMessageCallback;
        platform.glDebugMessageControl = glad_glDebugMessageControl;
    }
    else
    {
        Assert( !"InvalidCodePath" )
    }
    ReleaseDC( window, windowDC );
}

internal void WindowsProcessPendingMessages( Windows_State *state, Game_Controller_Input *keyboardController )
{
    MSG message;
    while ( PeekMessage( &message, 0, 0, 0, PM_REMOVE ) )
    {
        switch ( message.message )
        {
            case WM_QUIT:
            {
                globalRunning = false;
            }
            break;
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                u32 vkCode = ( u32 ) message.wParam;
                bool wasDown = ( message.lParam & ( 1 << 30 ) ) != 0;
                bool isDown = ( message.lParam & ( 1 << 31 ) ) == 0;
                bool altWasDown = ( message.lParam & ( 1 << 29 ) ) != 0;
                if ( wasDown != isDown )
                {
                    if ( vkCode == 'W' )
                    {
                        WindowsProcessKeyboardMessage( &keyboardController->up, isDown );
                    }
                    else if ( vkCode == 'A' )
                    {
                        WindowsProcessKeyboardMessage( &keyboardController->left, isDown );
                    }
                    else if ( vkCode == 'S' )
                    {
                        WindowsProcessKeyboardMessage( &keyboardController->down, isDown );
                    }
                    else if ( vkCode == 'D' )
                    {
                        WindowsProcessKeyboardMessage( &keyboardController->right, isDown );
                    }
                    else if ( vkCode == 'Q' )
                    {
                    }
                    else if ( vkCode == 'E' )
                    {
                    }
                    else if ( vkCode == VK_UP )
                    {
                    }
                    else if ( vkCode == VK_LEFT )
                    {
                    }
                    else if ( vkCode == VK_DOWN )
                    {
                    }
                    else if ( vkCode == VK_RIGHT )
                    {
                    }
                    else if ( vkCode == VK_SPACE )
                    {
                        WindowsProcessKeyboardMessage( &keyboardController->a, isDown );
                    }
                    else if ( vkCode == VK_ESCAPE )
                    {
                        globalRunning = false;
                    }
#ifdef INTERNAL
                    if ( vkCode == 'P' && isDown )
                    {
                        globalPause = !globalPause;
                    }
                    else if ( vkCode == 'L' && isDown )
                    {
                        if ( state->inputPlayingIndex == -1 )
                        {
                            if ( state->inputRecordingIndex == -1 )
                            {
                                WindowsBeginRecordingInput( state, 0 );
                            }
                            else
                            {
                                WindowsEndRecordingInput( state );
                                WindowsBeginInputPlayback( state, 0 );
                            }
                        }
                        else
                        {
                            WindowsEndInputPlayback( state );
                        }
                    }
#endif
                    if ( vkCode == VK_F4 && altWasDown )
                    {
                        globalRunning = false;
                    }
                }
            }
            break;
            default:
            {
                TranslateMessage( &message );
                DispatchMessage( &message );
            }
            break;
        }
    }
}

LRESULT CALLBACK WindowsMainWindowCallback( HWND window, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT result = 0;
    switch ( message )
    {
        case WM_SIZE:
        {
            windowWidth = LOWORD( lParam );
            windowHeight = HIWORD( lParam );
            windowChanged = true;
        }
        break;

        case WM_DESTROY:
        {
            globalRunning = false;
        }
        break;

        case WM_CLOSE:
        {
            globalRunning = false;
        }
        break;

        case WM_ACTIVATEAPP:
        {
        }
        break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            Assert( !"Keyboard input came in throught a non dispatch message" );
        }
        break;

        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint( window, &paint );
            EndPaint( window, &paint );
        }
        break;

        default:
        {
            result = DefWindowProc( window, message, wParam, lParam );
        }
        break;
    }
    return result;
}

int WinMain( HINSTANCE instance,
             HINSTANCE prevInstance,
             LPSTR commamdLine,
             int showCode )
{
    Windows_State state = {};
    WindowsGetExeFilename( &state );
    char sourceGameCodeDLLFullPath[ MAX_PATH ];
    WindowsBuildExePathFilename( &state, "game.dll", sourceGameCodeDLLFullPath, sizeof( sourceGameCodeDLLFullPath ) );
    char tempGameCodeDLLFullPath[ MAX_PATH ];
    WindowsBuildExePathFilename( &state, "game_temp.dll", tempGameCodeDLLFullPath, sizeof( tempGameCodeDLLFullPath ) );

    LARGE_INTEGER globalPerformanceCounterFrequencyResult;
    QueryPerformanceFrequency( &globalPerformanceCounterFrequencyResult );
    globalPerformanceCounterFrequency = globalPerformanceCounterFrequencyResult.QuadPart;

    UINT desiredSchedulerMs = 1; // Windows scheduler granularity
    bool sleepIsGranular = timeBeginPeriod( desiredSchedulerMs ) == TIMERR_NOERROR;

    WindowsLoadXInput();

    SetProcessDpiAwareness( PROCESS_SYSTEM_DPI_AWARE );

    WNDCLASS windowClass = {};

    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowsMainWindowCallback;
    windowClass.hInstance = instance;
    // windowClass.hIcon = ;
    windowClass.lpszClassName = "IsometricGameWindowClass";

    RegisterClass( &windowClass );

    //WS_EX_LAYERED - make transparent window
    HWND window = CreateWindowEx( 0, windowClass.lpszClassName, "Cute Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                  CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, 0, 0, instance, 0 );

    if ( window )
    {
        globalRunning = true;
        WindowsInitOpenGL( window );

        //@TODO: Figure out cursor hiding
        // RECT mouseBoundry;
        // GetWindowRect( window, &mouseBoundry );
        // ClipCursor( &mouseBoundry );
        // SetCursor( 0 );

        HDC windowDC = GetDC( window );
        int monitorRehreshHz = 60;
        HDC refreshContext = GetDC( window );
        int refreshRate = GetDeviceCaps( refreshContext, VREFRESH );
        ReleaseDC( window, refreshContext );
        if ( refreshRate > 1 )
        {
            monitorRehreshHz = refreshRate;
        }

        float32 gameRefreshHz = ( float32 ) monitorRehreshHz; // change this in case the software renderer can't hit 60 fps
        float32 targetSecondsPerFrame = 1.0f / ( float32 ) gameRefreshHz;

        Game_Memory gameMemory = {};
#ifdef INTERNAL
        LPVOID baseAddress = ( LPVOID ) Terabytes( 2 );
#else
        LPVOID baseAddress = 0;
#endif
        gameMemory.permanentStorageSize = Megabytes( 64 );
        gameMemory.transientStorageSize = Megabytes( 256 );
        state.totalSize = gameMemory.permanentStorageSize + gameMemory.transientStorageSize;
        state.gameMemoryBlock = VirtualAlloc( baseAddress, state.totalSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
        gameMemory.permanentStorage = state.gameMemoryBlock;
        gameMemory.transientStorage = ( u8 * ) gameMemory.permanentStorage + gameMemory.permanentStorageSize;
#ifdef INTERNAL
        platform.DebugFreeFileMemory = DebugPlatformFreeFileMemory;
        platform.DebugReadEntireFile = DebugPlatformReadEntireFile;
        platform.DebugWriteEntireFile = DebugPlatformWriteEntireFile;
#endif

        gameMemory.platformApi = platform;

        state.inputPlayingIndex = -1;
        state.inputRecordingIndex = -1;
        for ( int replayIndex = 0; replayIndex < ArrayCount( state.replayBuffers ); ++replayIndex )
        {
            Windows_Replay_Buffer *replayBuffer = &state.replayBuffers[ replayIndex ];
            WindowsGetInputRecordingFileLocation( &state, false, replayIndex, replayBuffer->filename, sizeof( replayBuffer->filename ) );
            replayBuffer->fileHandle = CreateFile( replayBuffer->filename, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0 );
            DWORD maxSizeHigh = state.totalSize >> 32;
            DWORD maxSizeLow = state.totalSize & 0xFFFFFFFF;
            replayBuffer->memoryMap = CreateFileMapping( replayBuffer->fileHandle, 0, PAGE_READWRITE,
                                                         maxSizeHigh, maxSizeLow, 0 );
            replayBuffer->memoryBlock = MapViewOfFile( replayBuffer->memoryMap, FILE_MAP_ALL_ACCESS, 0, 0, state.totalSize );
            Assert( replayBuffer->memoryBlock );
        }

        if ( gameMemory.permanentStorage && gameMemory.transientStorage )
        {
            LARGE_INTEGER lastCounter = WindowsGetWallClock();
            u64 lastCycleCount = __rdtsc();

            Game_Input input[ 2 ] = {};
            Game_Input *newInput = &input[ 0 ];
            Game_Input *oldInput = &input[ 1 ];

            Windows_Game_Code game = WindowsLoadGameCode( sourceGameCodeDLLFullPath, tempGameCodeDLLFullPath );
            if ( game.Init )
            {
                game.Init( &gameMemory, windowWidth, windowHeight );
            }
            while ( globalRunning )
            {
                newInput->dtForUpdate = targetSecondsPerFrame;
#ifdef INTERNAL
                FILETIME newDLLWtiteTime = WindowsGetLastWriteTime( sourceGameCodeDLLFullPath );
                if ( CompareFileTime( &newDLLWtiteTime, &game.dllLastWriteTime ) != 0 )
                {
                    WindowsUnloadGameCode( game );
                    game = WindowsLoadGameCode( sourceGameCodeDLLFullPath, tempGameCodeDLLFullPath );
                }
#endif

                Game_Controller_Input *oldKeyboardController = getController( oldInput, 0 );
                Game_Controller_Input *newKeyboardController = getController( newInput, 0 );
                *newKeyboardController = {};
                newKeyboardController->isConnected = true;

                for ( int buttonIndex = 0; buttonIndex < ArrayCount( newKeyboardController->buttons ); ++buttonIndex )
                {
                    newKeyboardController->buttons[ buttonIndex ].endedDown = oldKeyboardController->buttons[ buttonIndex ].endedDown;
                }

                WindowsProcessPendingMessages( &state, newKeyboardController );

                if ( globalPause )
                {
                    continue;
                }

                POINT mouse;
                GetCursorPos( &mouse );
                ScreenToClient( window, &mouse );
                newInput->mouseX = mouse.x;
                newInput->mouseY = mouse.y;
                newInput->mouseZ = 0;
                WindowsProcessKeyboardMessage( &newInput->mouseButtons[ 0 ], GetKeyState( VK_LBUTTON ) & ( 1 << 15 ) );
                WindowsProcessKeyboardMessage( &newInput->mouseButtons[ 1 ], GetKeyState( VK_RBUTTON ) & ( 1 << 15 ) );
                WindowsProcessKeyboardMessage( &newInput->mouseButtons[ 2 ], GetKeyState( VK_MBUTTON ) & ( 1 << 15 ) );
                WindowsProcessKeyboardMessage( &newInput->mouseButtons[ 3 ], GetKeyState( VK_XBUTTON1 ) & ( 1 << 15 ) );
                WindowsProcessKeyboardMessage( &newInput->mouseButtons[ 4 ], GetKeyState( VK_XBUTTON2 ) & ( 1 << 15 ) );

                DWORD maxControllerCount = XUSER_MAX_COUNT;
                if ( maxControllerCount > ArrayCount( newInput->controllers ) - 1 )
                {
                    maxControllerCount = ArrayCount( newInput->controllers ) - 1;
                }
                for ( DWORD controllerIndex = 0; controllerIndex < maxControllerCount; ++controllerIndex )
                {
                    DWORD ourControllerIndex = controllerIndex + 1;
                    Game_Controller_Input *oldController = getController( oldInput, ourControllerIndex );
                    Game_Controller_Input *newController = getController( newInput, ourControllerIndex );

                    XINPUT_STATE controllerState;
                    if ( XInputGetState( controllerIndex, &controllerState ) == ERROR_SUCCESS )
                    {
                        newController->isConnected = true;
                        newController->isAnalog = oldController->isAnalog;
                        XINPUT_GAMEPAD *pad = &controllerState.Gamepad;

                        newController->leftTriggerAverage = WindowsProcessXInputTriggerValue( pad->bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD );
                        newController->rightTriggerAverage = WindowsProcessXInputTriggerValue( pad->bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD );

                        newController->leftStickAverageX = WindowsProcessXInputStickValue( pad->sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
                        newController->leftStickAverageY = WindowsProcessXInputStickValue( pad->sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
                        newController->rightStickAverageX = WindowsProcessXInputStickValue( pad->sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
                        newController->rightStickAverageY = WindowsProcessXInputStickValue( pad->sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );

                        if ( newController->leftStickAverageY != 0.0f || newController->leftStickAverageX != 0.0f ||
                             newController->rightStickAverageY != 0.0f || newController->rightStickAverageX != 0.0f ||
                             newController->leftTriggerAverage != 0.0f || newController->rightTriggerAverage != 0.0f )
                        {
                            newController->isAnalog = true;
                        }

                        if ( pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT )
                        {
                            newController->leftStickAverageX = -1;
                            newController->isAnalog = false;
                        }

                        if ( pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT )
                        {
                            newController->leftStickAverageX = 1;
                            newController->isAnalog = false;
                        }

                        if ( pad->wButtons & XINPUT_GAMEPAD_DPAD_UP )
                        {
                            newController->leftStickAverageY = 1;
                            newController->isAnalog = false;
                        }

                        if ( pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN )
                        {
                            newController->leftStickAverageY = -1;
                            newController->isAnalog = false;
                        }

                        float32 threshold = 0.5f;
                        WindowsProcessXInputDigitalButton( newController->leftStickAverageX < -threshold ? 1 : 0,
                                                           &oldController->left, 1, &newController->left );
                        WindowsProcessXInputDigitalButton( newController->leftStickAverageX > threshold ? 1 : 0,
                                                           &oldController->right, 1, &newController->right );
                        WindowsProcessXInputDigitalButton( newController->leftStickAverageY < -threshold ? 1 : 0,
                                                           &oldController->down, 1, &newController->down );
                        WindowsProcessXInputDigitalButton( newController->leftStickAverageY > threshold ? 1 : 0,
                                                           &oldController->up, 1, &newController->up );

                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->down, XINPUT_GAMEPAD_DPAD_DOWN, &newController->down );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->up, XINPUT_GAMEPAD_DPAD_UP, &newController->up );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->left, XINPUT_GAMEPAD_DPAD_LEFT, &newController->left );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->right, XINPUT_GAMEPAD_DPAD_RIGHT, &newController->right );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->start, XINPUT_GAMEPAD_START, &newController->start );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->back, XINPUT_GAMEPAD_BACK, &newController->back );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->l3, XINPUT_GAMEPAD_LEFT_THUMB, &newController->l3 );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->r3, XINPUT_GAMEPAD_RIGHT_THUMB, &newController->r3 );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->l1, XINPUT_GAMEPAD_LEFT_SHOULDER, &newController->l1 );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->r1, XINPUT_GAMEPAD_RIGHT_SHOULDER, &newController->r1 );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->a, XINPUT_GAMEPAD_A, &newController->a );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->b, XINPUT_GAMEPAD_B, &newController->b );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->x, XINPUT_GAMEPAD_X, &newController->x );
                        WindowsProcessXInputDigitalButton( pad->wButtons, &oldController->y, XINPUT_GAMEPAD_Y, &newController->y );

                        // XINPUT_VIBRATION vibration;
                        // vibration.wLeftMotorSpeed = 60000;
                        // vibration.wRightMotorSpeed = 60000;
                        // XInputSetState( 0, &vibration );
                    }
                    else
                    {
                        newController->isConnected = false;
                    }
                }

                if ( state.inputRecordingIndex > -1 )
                {
                    WindowsRecordInput( &state, newInput );
                }

                if ( state.inputPlayingIndex > -1 )
                {
                    WindowsPlaybackInput( &state, newInput );
                }

                if ( game.WindowChanged && windowChanged )
                {
                    game.WindowChanged( &gameMemory, windowWidth, windowHeight );
                    windowChanged = false;
                }

                if ( game.UpdateAndRender )
                {
                    game.UpdateAndRender( &gameMemory, newInput );
                }

                SwapBuffers( windowDC );
                LARGE_INTEGER workCounter = WindowsGetWallClock();
                float32 secondsElapsedForFrame = WindowsGetSecondsElapsed( lastCounter, workCounter );

                printf( "Frame time: %f\n", secondsElapsedForFrame );
                LARGE_INTEGER endCounter = WindowsGetWallClock();
                lastCounter = endCounter;

                Game_Input *temp = newInput;
                newInput = oldInput;
                oldInput = temp;
            }
        }
        else
        {
            //@TODO: Logging
        }
    }
    else
    {
        //@TODO: Logging
    }
    return 0;
}
