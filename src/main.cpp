#include "glad.c"
#include <stdio.h>
#include <stdlib.h>
#include <glfw3.h>
#include "utils/utils.h"
// #define STB_IMAGE_IMPLEMENTATION
#include "math.cpp"
#include "shaders.cpp"
#include "shapes.cpp"
#include "camera.cpp"

global_variable float32 deltaTime = 0.0f;
global_variable float32 lastFrame = 0.0f;
global_variable int windowWidth = 1280;
global_variable int windowHeight = 1280;

global_variable Camera camera;
global_variable bool firstMouse = true;
global_variable float32 lastMouseX = 0.0f;
global_variable float32 lastMouseY = 0.0f;

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

void FramebufferSizeCallback( GLFWwindow *window, int width, int height )
{
    windowWidth = width;
    windowHeight = height;
    glViewport( 0, 0, width, height );
}

void ProcessInput( GLFWwindow *window )
{
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }

    if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
    {
        CameraProcessKeyboard( &camera, FORWARD, deltaTime );
    }

    if ( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
    {
        CameraProcessKeyboard( &camera, BACKWARD, deltaTime );
    }

    if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
    {
        CameraProcessKeyboard( &camera, LEFT, deltaTime );
    }

    if ( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
    {
        CameraProcessKeyboard( &camera, RIGHT, deltaTime );
    }

    if ( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
    {
        camera.movementSpeed = 7.0f;
    }
    else
    {
        camera.movementSpeed = defaultSpeed;
    }
}

void MouseCallback( GLFWwindow *window, float64 mouseX, float64 mouseY )
{
    if ( firstMouse )
    {
        lastMouseX = ( float32 ) mouseX;
        lastMouseY = ( float32 ) mouseY;
        firstMouse = false;
    }

    float32 xOffset = ( float32 ) mouseX - lastMouseX;
    float32 yOffset = lastMouseY - ( float32 ) mouseY;
    lastMouseX = ( float32 ) mouseX;
    lastMouseY = ( float32 ) mouseY;
    CameraProcessMouse( &camera, xOffset, yOffset );
}
int main()
{
    glfwInit();

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );
    glfwWindowHint( GLFW_SAMPLES, 4 );

    GLFWwindow *window = glfwCreateWindow( windowWidth, windowHeight, "OpenGL Engine", 0, 0 );
    if ( !window )
    {
        printf( "Failed to create GLFW window!\n" );
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    glfwSetCursorPosCallback( window, MouseCallback );
    glfwMakeContextCurrent( window );

    if ( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
    {
        printf( "Failed to initialize GLAD!\n" );
        return -1;
    }

    int flags;
    glGetIntegerv( GL_CONTEXT_FLAGS, &flags );
    if ( flags & GL_CONTEXT_FLAG_DEBUG_BIT )
    {
        glEnable( GL_DEBUG_OUTPUT );
        glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
        glDebugMessageCallback( glDebugOutput, 0 );
        glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE );
    }

    InitShapes();
    camera = CreateCamera( Vector3( 0.0f, 0.0f, 6.0f ), defaultYaw, 0.0f );
    // camera = CreateCamera( Vector3( 0.0f, 3.0f, 6.0f ), defaultYaw, -20.0f );

    u32 uboMatrices;
    glGenBuffers( 1, &uboMatrices );
    glBindBuffer( GL_UNIFORM_BUFFER, uboMatrices );
    glBufferData( GL_UNIFORM_BUFFER, 2 * sizeof( Matrix4 ), 0, GL_STATIC_DRAW );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    glBindBufferRange( GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof( Matrix4 ) );

    Matrix4 projection = GetProjectionMatrix( 45.0f, ( float32 ) windowWidth / ( float32 ) windowHeight, 0.1f, 500.0f );
    glBindBuffer( GL_UNIFORM_BUFFER, uboMatrices );
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( Matrix4 ), projection.data );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    glViewport( 0, 0, windowWidth, windowHeight );
    glfwSetFramebufferSizeCallback( window, FramebufferSizeCallback );

    glEnable( GL_DEPTH_TEST );
    // glEnable( GL_STENCIL_TEST );
    glEnable( GL_BLEND );
    // glEnable( GL_CULL_FACE );
    glEnable( GL_MULTISAMPLE );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // glCullFace( GL_FRONT ); //GL_BACK is default
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //Wireframe

    while ( !glfwWindowShouldClose( window ) )
    {
        float32 currentFrame = ( float32 ) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        ProcessInput( window );

        glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

        Matrix4 view = GetViewMatrix( &camera );
        glBindBuffer( GL_UNIFORM_BUFFER, uboMatrices );
        glBufferSubData( GL_UNIFORM_BUFFER, sizeof( Matrix4 ), sizeof( Matrix4 ), view.data );
        glBindBuffer( GL_UNIFORM_BUFFER, 0 );

        Transform cubeTransform = Transform();
        // Translate( cubeTransform, Vector3( 2.5f, 0.0f, 0.0f ) );
        // Rotate( cubeTransform, Vector3( 0.0f, 0.0f, 1.0f ), 45.0f );
        // Scale( cubeTransform, Vector3( 1.5f, 0.5f, 0.5f ) );
        // Rotate( cubeTransform, Vector3( 1.0f, 0.0f, 0.0f ), -90.0f );
        // RotateAroundPoint( cubeTransform, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), 90.0f );
        // RotateAroundPoint( cubeTransform, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), 90.0f );
        DrawCube( cubeTransform );

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
