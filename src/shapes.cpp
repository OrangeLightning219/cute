#pragma once

// #include "stb_image.h"
#include "images.cpp"
#ifndef UNITY_BUILD
    #include "game.h"
    #include "math.cpp"
    #include "shaders.cpp"
#endif

struct Shapes
{
    Shader baseShader;
    u32 cubeVBO;
    u32 cubeVAO;
    u32 cubeTexture;
    float32 cubeVertices[ 288 ] = {
        // positions         // normals         // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };
};

global_variable Shapes shapes;

void InitShapes()
{
    shapes = {};
    platform.glGenTextures( 1, &shapes.cubeTexture );
    platform.glBindTexture( GL_TEXTURE_2D, shapes.cubeTexture );
    platform.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    platform.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    platform.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    platform.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    //     int width, height, nrChannels;
    //     stbi_set_flip_vertically_on_load( true ); // tell stb_image.h to flip loaded texture's on the y-axis.
    //     u8 *data = stbi_load( "container.png", &width, &height, &nrChannels, 0 );

    //     if ( data )
    //     {
    //         glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    //         glGenerateMipmap( GL_TEXTURE_2D );
    //     }
    //     else
    //     {
    //         printf( "Failed to load texture 'container.png'!\n" );
    //     }
    //     stbi_image_free( data );

    QOI_Image image = LoadQOI( "container.qoi" );
    if ( image.pixels )
    {
        platform.glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels );
        platform.glGenerateMipmap( GL_TEXTURE_2D );
    }
    else
    {
        printf( "Failed to load texture 'container.png'!\n" );
    }

    shapes.baseShader = CreateShader( "../src/shaders/base.vert", "../src/shaders/base.frag" );
    BindUniformBlock( shapes.baseShader, "Matrices", 0 );
    platform.glGenVertexArrays( 1, &shapes.cubeVAO );
    platform.glGenBuffers( 1, &shapes.cubeVBO );
    platform.glBindVertexArray( shapes.cubeVAO );
    platform.glBindBuffer( GL_ARRAY_BUFFER, shapes.cubeVBO );
    platform.glBufferData( GL_ARRAY_BUFFER, sizeof( shapes.cubeVertices ), &shapes.cubeVertices, GL_STATIC_DRAW );

    platform.glEnableVertexAttribArray( 0 );
    platform.glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float32 ), ( void * ) 0 );
    platform.glEnableVertexAttribArray( 1 );
    platform.glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float32 ), ( void * ) ( 3 * sizeof( float32 ) ) );
    platform.glEnableVertexAttribArray( 2 );
    platform.glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float32 ), ( void * ) ( 6 * sizeof( float32 ) ) );

    UseShader( shapes.baseShader );
    ShaderSetInt( shapes.baseShader, "material.texture_diffuse1", 0 );
}

void DrawCube( Transform &transform )
{
    UseShader( shapes.baseShader );
    platform.glActiveTexture( GL_TEXTURE0 );
    platform.glBindTexture( GL_TEXTURE_2D, shapes.cubeTexture );
    MatrixTransform modelMatrix = CreateModelMatrix( transform );
    ShaderSetMat4( shapes.baseShader, "model", &modelMatrix.data[ 0 ][ 0 ] );
    platform.glBindVertexArray( shapes.cubeVAO );
    platform.glDrawArrays( GL_TRIANGLES, 0, 36 );
    platform.glBindVertexArray( 0 );
}
