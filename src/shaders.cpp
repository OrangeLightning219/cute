#pragma once
#ifndef UNITY_BUILD
    #include "game.h"
    #include "utils/utils.h"
    #include "math.cpp"
#endif

struct Shader
{
    u32 id;
};

internal Shader CreateShader( char *vertexPath, char *fragmentPath, char *geometryPath )
{
    int success;
    char infoLog[ 512 ];

    Debug_Read_File_Result vertexFile = platform.DebugReadEntireFile( vertexPath );
    defer { platform.DebugFreeFileMemory( vertexFile.content ); };

    Debug_Read_File_Result fragmentFile = platform.DebugReadEntireFile( fragmentPath );
    defer { platform.DebugFreeFileMemory( fragmentFile.content ); };

    u32 vertexShader = platform.glCreateShader( GL_VERTEX_SHADER );
    platform.glShaderSource( vertexShader, 1, ( char ** ) &vertexFile.content, ( int * ) &vertexFile.contentSize );
    platform.glCompileShader( vertexShader );
    platform.glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );

    if ( !success )
    {
        platform.glGetShaderInfoLog( vertexShader, 512, 0, infoLog );
        printf( "Error during vertex shader compilation:\n%s", infoLog );
    }

    u32 fragmentShader = platform.glCreateShader( GL_FRAGMENT_SHADER );
    platform.glShaderSource( fragmentShader, 1, ( char ** ) &fragmentFile.content, ( int * ) &fragmentFile.contentSize );
    platform.glCompileShader( fragmentShader );
    platform.glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );

    if ( !success )
    {
        platform.glGetShaderInfoLog( fragmentShader, 512, 0, infoLog );
        printf( "Error during fragment shader compilation:\n%s", infoLog );
    }

    u32 id = platform.glCreateProgram();
    platform.glAttachShader( id, vertexShader );
    platform.glAttachShader( id, fragmentShader );

    if ( geometryPath )
    {
        Debug_Read_File_Result geometryFile = platform.DebugReadEntireFile( geometryPath );
        defer { platform.DebugFreeFileMemory( geometryFile.content ); };

        u32 geometryShader = platform.glCreateShader( GL_GEOMETRY_SHADER );
        platform.glShaderSource( geometryShader, 1, ( char ** ) &geometryFile.content, ( int * ) &geometryFile.contentSize );
        platform.glCompileShader( geometryShader );
        platform.glGetShaderiv( geometryShader, GL_COMPILE_STATUS, &success );

        if ( !success )
        {
            platform.glGetShaderInfoLog( geometryShader, 512, 0, infoLog );
            printf( "Error during geometry shader compilation:\n%s", infoLog );
        }

        platform.glAttachShader( id, geometryShader );
    }

    platform.glLinkProgram( id );
    platform.glGetProgramiv( id, GL_LINK_STATUS, &success );

    if ( !success )
    {
        platform.glGetProgramInfoLog( id, 512, 0, infoLog );
        printf( "Error during shader linking:\n%s", infoLog );
    }

    platform.glDeleteShader( vertexShader );
    platform.glDeleteShader( fragmentShader );

    return Shader{ id };
}

internal inline Shader CreateShader( char *vertexPath, char *fragmentPath )
{
    return CreateShader( vertexPath, fragmentPath, 0 );
}

inline void UseShader( Shader shader )
{
    platform.glUseProgram( shader.id );
}

inline void BindUniformBlock( Shader shader, char *blockName, int bindingPoint )
{
    u32 blockIndex = platform.glGetUniformBlockIndex( shader.id, blockName );
    platform.glUniformBlockBinding( shader.id, blockIndex, bindingPoint );
}

internal int GetUniformLocation( int shader, char *name )
{
    int location = platform.glGetUniformLocation( shader, name );
    if ( location < 0 )
    {
        printf( "Could not find shader location: %s\n", name );
    }
    return location;
}

inline void ShaderSetBool( Shader shader, char *name, bool value )
{
    platform.glUniform1i( GetUniformLocation( shader.id, name ), ( int ) value );
}

inline void ShaderSetInt( Shader shader, char *name, int value )
{
    platform.glUniform1i( GetUniformLocation( shader.id, name ), value );
}

inline void ShaderSetFloat32( Shader shader, char *name, float32 value )
{
    platform.glUniform1f( GetUniformLocation( shader.id, name ), value );
}

inline void ShaderSetVec4( Shader shader, char *name, float32 x, float32 y, float32 z, float32 w )
{
    platform.glUniform4f( GetUniformLocation( shader.id, name ), x, y, z, w );
}

inline void ShaderSetVec3( Shader shader, char *name, float32 x, float32 y, float32 z )
{
    platform.glUniform3f( GetUniformLocation( shader.id, name ), x, y, z );
}

inline void ShaderSetVec3( Shader shader, char *name, Vector3 v )
{
    platform.glUniform3f( GetUniformLocation( shader.id, name ), v.x, v.y, v.z );
}

inline void ShaderSetMat4( Shader shader, char *name, float32 *mat4 )
{
    platform.glUniformMatrix4fv( GetUniformLocation( shader.id, name ), 1, GL_FALSE, mat4 );
}
