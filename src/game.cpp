#include "game.h"
#include "cute.cpp"

void APIENTRY glDebugOutput( GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                             const char *message, const void *userParam );
struct Game_State
{
    Memory_Arena arena;
    Camera camera;
    u32 uboMatrices;
    int windowWidth;
    int windowHeight;

    bool firstMouse;
    float32 lastMouseX;
    float32 lastMouseY;
};

internal inline Game_State *GetGameState( Game_Memory *memory )
{
    return ( Game_State * ) memory->permanentStorage;
}

Platform_Api platform;

extern "C" __declspec( dllexport )
GAME_INIT( GameInit )
{
    platform = memory->platformApi;
    Game_State *gameState = GetGameState( memory );
    gameState->windowWidth = windowWidth;
    gameState->windowHeight = windowHeight;

    gameState->firstMouse = true;
    int flags;
    platform.glGetIntegerv( GL_CONTEXT_FLAGS, &flags );
    if ( flags & GL_CONTEXT_FLAG_DEBUG_BIT )
    {
        platform.glEnable( GL_DEBUG_OUTPUT );
        platform.glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
        platform.glDebugMessageCallback( glDebugOutput, 0 );
        platform.glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE );
    }

    CuteInit();
    gameState->camera = CreateCamera( Vector3( 0.0f, 0.0f, 6.0f ), defaultYaw, 0.0f );
    // camera = CreateCamera( Vector3( 0.0f, 3.0f, 6.0f ), defaultYaw, -20.0f );

    u32 uboMatrices;
    platform.glGenBuffers( 1, &uboMatrices );
    platform.glBindBuffer( GL_UNIFORM_BUFFER, uboMatrices );
    platform.glBufferData( GL_UNIFORM_BUFFER, 2 * sizeof( Matrix4 ), 0, GL_STATIC_DRAW );
    platform.glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    platform.glBindBufferRange( GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof( Matrix4 ) );
    gameState->uboMatrices = uboMatrices;

    Matrix4 projection = GetProjectionMatrix( 45.0f, ( float32 ) windowWidth / ( float32 ) windowHeight, 0.1f, 500.0f );
    platform.glBindBuffer( GL_UNIFORM_BUFFER, uboMatrices );
    platform.glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( Matrix4 ), projection.data );
    platform.glBindBuffer( GL_UNIFORM_BUFFER, 0 );
    platform.glEnable( GL_DEPTH_TEST );
    // platform.glEnable( GL_STENCIL_TEST );
    platform.glEnable( GL_BLEND );
    // platform.glEnable( GL_CULL_FACE );
    platform.glEnable( GL_MULTISAMPLE );

    platform.glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // platform.glCullFace( GL_FRONT ); //GL_BACK is default
    // platform.glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //Wireframe
    platform.glViewport( 0, 0, windowWidth, windowHeight );
}

extern "C" __declspec( dllexport )
GAME_UPDATE_AND_RENDER( GameUpdateAndRender )
{
    Game_State *gameState = GetGameState( memory );
    platform.glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
    platform.glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // | GL_STENCIL_BUFFER_BIT );

    float32 dt = input->dtForUpdate;
    for ( int controllerIndex = 0; controllerIndex < ArrayCount( input->controllers ); ++controllerIndex )
    {
        Game_Controller_Input *controller = getController( input, controllerIndex );
        if ( !controller->isConnected ) { continue; }
        if ( controller->isAnalog )
        {
        }
        else
        {
            Camera *camera = &gameState->camera;
            float32 velocity = camera->movementSpeed * dt;
            if ( controller->up.endedDown ) { camera->position += camera->front * velocity; }
            if ( controller->down.endedDown ) { camera->position -= camera->front * velocity; }
            if ( controller->left.endedDown ) { camera->position -= camera->right * velocity; }
            if ( controller->right.endedDown ) { camera->position += camera->right * velocity; }

            if ( controller->a.endedDown ) { camera->movementSpeed = 7.0f; }
            else { camera->movementSpeed = defaultSpeed; }
        }
    }

    if ( gameState->firstMouse )
    {
        gameState->lastMouseX = ( float32 ) input->mouseX;
        gameState->lastMouseY = ( float32 ) input->mouseY;
        gameState->firstMouse = false;
    }

    float32 xOffset = ( float32 ) input->mouseX - gameState->lastMouseX;
    float32 yOffset = gameState->lastMouseY - ( float32 ) input->mouseY;
    gameState->lastMouseX = ( float32 ) input->mouseX;
    gameState->lastMouseY = ( float32 ) input->mouseY;
    CameraProcessMouse( &gameState->camera, xOffset, yOffset );

    Matrix4 view = GetViewMatrix( &gameState->camera );
    platform.glBindBuffer( GL_UNIFORM_BUFFER, gameState->uboMatrices );
    platform.glBufferSubData( GL_UNIFORM_BUFFER, sizeof( Matrix4 ), sizeof( Matrix4 ), view.data );
    platform.glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    Transform cubeTransform = Transform();
    // Translate( cubeTransform, Vector3( 2.5f, 0.0f, 0.0f ) );
    // Rotate( cubeTransform, Vector3( 0.0f, 0.0f, 1.0f ), 45.0f );
    Scale( cubeTransform, Vector3( 1.5f, 0.5f, 0.5f ) );
    // Rotate( cubeTransform, Vector3( 1.0f, 0.0f, 0.0f ), -90.0f );
    // RotateAroundPoint( cubeTransform, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), 90.0f );
    // RotateAroundPoint( cubeTransform, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), 90.0f );
    DrawCube( cubeTransform );
}

extern "C" __declspec( dllexport )
GAME_WINDOW_CHANGED( GameWindowChanged )
{
    Game_State *gameState = GetGameState( memory );
    gameState->windowWidth = windowWidth;
    gameState->windowHeight = windowHeight;
    platform.glViewport( 0, 0, windowWidth, windowHeight );
}

void APIENTRY glDebugOutput( GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                             const char *message, const void *userParam )
{
    // ignore non-significant error/warning codes
    if ( id == 131169 || id == 131185 || id == 131218 || id == 131204 ) return;

    char *sourceString = 0;
    switch ( source )
    {
        case GL_DEBUG_SOURCE_API: sourceString = ( char * ) "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceString = ( char * ) "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceString = ( char * ) "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: sourceString = ( char * ) "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION: sourceString = ( char * ) "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER: sourceString = ( char * ) "Source: Other"; break;
    }

    char *typeString = 0;
    switch ( type )
    {
        case GL_DEBUG_TYPE_ERROR: typeString = ( char * ) "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeString = ( char * ) "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeString = ( char * ) "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY: typeString = ( char * ) "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: typeString = ( char * ) "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER: typeString = ( char * ) "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP: typeString = ( char * ) "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP: typeString = ( char * ) "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER: typeString = ( char * ) "Type: Other"; break;
    }

    char *severityString = 0;
    switch ( severity )
    {
        case GL_DEBUG_SEVERITY_HIGH: severityString = ( char * ) "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: severityString = ( char * ) "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW: severityString = ( char * ) "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: severityString = ( char * ) "Severity: notification"; break;
    }
    printf( "Debug message (%d): %s\n%s | %s | %s\n\n", id, message, sourceString, typeString, severityString );
}
