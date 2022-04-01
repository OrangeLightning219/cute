#include <stdio.h>
#include <glad/glad.h>
#include <glfw3.h>
#include "utils/utils.h"
#include "math.cpp"

global_variable float32 deltaTime = 0.0f;
global_variable float32 lastFrame = 0.0f;
global_variable int windowWidth = 1280;
global_variable int windowHeight = 1280;

void APIENTRY glDebugOutput( GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                             const char *message, const void *userParam )
{
    // ignore non-significant error/warning codes
    if ( id == 131169 || id == 131185 || id == 131218 || id == 131204 ) return;

    char *sourceString = 0;
    switch ( source )
    {
        case GL_DEBUG_SOURCE_API: sourceString = "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceString = "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceString = "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: sourceString = "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION: sourceString = "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER: sourceString = "Source: Other"; break;
    }

    char *typeString = 0;
    switch ( type )
    {
        case GL_DEBUG_TYPE_ERROR: typeString = "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeString = "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeString = "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY: typeString = "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: typeString = "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER: typeString = "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP: typeString = "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP: typeString = "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER: typeString = "Type: Other"; break;
    }

    char *severityString = 0;
    switch ( severity )
    {
        case GL_DEBUG_SEVERITY_HIGH: severityString = "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: severityString = "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW: severityString = "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: severityString = "Severity: notification"; break;
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
}

int main()
{
    Vector3 a = Vector3( 1.0f, 2.0f, 3.0f );
    Vector3 b = Vector3( 2.0f, 3.0f, 4.0f );
    Vector3 c = a + b;
    printf( "x: %f, y: %f, z: %f\n", c.x, c.y, c.z );

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

    // glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
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

    glViewport( 0, 0, windowWidth, windowHeight );
    glfwSetFramebufferSizeCallback( window, FramebufferSizeCallback );

    glEnable( GL_DEPTH_TEST );
    // glEnable( GL_STENCIL_TEST );
    glEnable( GL_BLEND );
    glEnable( GL_CULL_FACE );
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

        // glBindFramebuffer( GL_FRAMEBUFFER, fbo );
        glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
