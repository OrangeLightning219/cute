#pragma once

// #include "stb_image.h"
#include "images.cpp"
#ifndef UNITY_BUILD
    #include "glad.c"
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
    glGenTextures( 1, &shapes.cubeTexture );
    glBindTexture( GL_TEXTURE_2D, shapes.cubeTexture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

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
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels );
        glGenerateMipmap( GL_TEXTURE_2D );
    }
    else
    {
        printf( "Failed to load texture 'container.png'!\n" );
    }

    shapes.baseShader = CreateShader( "../src/shaders/base.vert", "../src/shaders/base.frag" );
    BindUniformBlock( shapes.baseShader, "Matrices", 0 );
    glGenVertexArrays( 1, &shapes.cubeVAO );
    glGenBuffers( 1, &shapes.cubeVBO );
    glBindVertexArray( shapes.cubeVAO );
    glBindBuffer( GL_ARRAY_BUFFER, shapes.cubeVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( shapes.cubeVertices ), &shapes.cubeVertices, GL_STATIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float32 ), ( void * ) 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float32 ), ( void * ) ( 3 * sizeof( float32 ) ) );
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float32 ), ( void * ) ( 6 * sizeof( float32 ) ) );

    UseShader( shapes.baseShader );
    ShaderSetInt( shapes.baseShader, "material.texture_diffuse1", 0 );
}

void DrawCube( Transform &transform )
{
    UseShader( shapes.baseShader );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, shapes.cubeTexture );
    MatrixTransform modelMatrix = CreateModelMatrix( transform );
    ShaderSetMat4( shapes.baseShader, "model", &modelMatrix.data[ 0 ][ 0 ] );
    glBindVertexArray( shapes.cubeVAO );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glBindVertexArray( 0 );
}
