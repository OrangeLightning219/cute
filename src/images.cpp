#pragma once
#ifndef UNITY_BUILD
    #include <stdio.h>
    #include <stdlib.h>
    #include "utils/utils.h"
    #include "game.h"
    #include "math.cpp"
#endif

struct QOI_Image
{
    u32 width;
    u32 height;
    u8 channels;
    u8 colorspace;
    u8 *pixels;
};

#pragma pack( push, 1 )
struct QOI_Header
{
    char magic[ 4 ]; // "qoif"
    u32 width;
    u32 height;
    u8 channels;   // 3 = RGB, 4 = RGBA
    u8 colorspace; // 0 = sRGB with linear alpha, 1 = all channels linear
};
#pragma pack( pop )

struct Color
{
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 255;

    Color() { }

    Color( u8 r, u8 g, u8 b, u8 a )
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
};

internal inline u32 ToLittleEndian( u32 value )
{
    u8 *bytes = ( u8 * ) &value;
    return bytes[ 3 ] << 0 | bytes[ 2 ] << 8 | bytes[ 1 ] << 16 | bytes[ 0 ] << 24;
}

//@TODO: Add option to flip
internal QOI_Image LoadQOI( char *filename )
{
    QOI_Image result = {};

    Debug_Read_File_Result file = platform.DebugReadEntireFile( filename );
    if ( file.contentSize > 0 )
    {
        QOI_Header *header = ( QOI_Header * ) file.content;
        header->width = ToLittleEndian( header->width );
        header->height = ToLittleEndian( header->height );

        result.width = header->width;
        result.height = header->height;
        result.channels = header->channels;
        result.colorspace = header->colorspace;
        result.pixels = ( u8 * ) malloc( result.width * result.height * result.channels * sizeof( u8 ) );

        u8 OP_RGB = 0b11111110;
        u8 OP_RGBA = 0b11111111;
        u8 OP_INDEX = 0b00;
        u8 OP_DIFF = 0b01;
        u8 OP_LUMA = 0b10;
        u8 OP_RUN = 0b11;

        u8 *data = ( u8 * ) file.content + sizeof( QOI_Header );
        u32 dataSize = file.contentSize - sizeof( QOI_Header ) - 8;
        u32 bytesProcessed = 0;
        u8 *pixels = result.pixels;
        Color seenPixels[ 64 ] = {};
        Color previousPixel = Color( 0, 0, 0, 255 );

        while ( bytesProcessed < dataSize )
        {
            bool wasRun = false;

            if ( *data == OP_RGB )
            {
                previousPixel.r = data[ 1 ];
                previousPixel.g = data[ 2 ];
                previousPixel.b = data[ 3 ];
                bytesProcessed += 4;
                data += 4;
            }
            else if ( *data == OP_RGBA )
            {
                previousPixel.r = data[ 1 ];
                previousPixel.g = data[ 2 ];
                previousPixel.b = data[ 3 ];
                previousPixel.a = data[ 4 ];
                bytesProcessed += 5;
                data += 5;
            }
            else if ( *data >> 6 == OP_INDEX )
            {
                u8 index = *data;
                previousPixel = seenPixels[ index ];
                bytesProcessed += 1;
                data += 1;
            }
            else if ( *data >> 6 == OP_DIFF )
            {
                u8 byte = *data;
                s8 db = ( byte & 0b00000011 ) - 2;
                s8 dg = ( ( byte >> 2 ) & 0b00000011 ) - 2;
                s8 dr = ( ( byte >> 4 ) & 0b00000011 ) - 2;
                previousPixel.r += dr;
                previousPixel.g += dg;
                previousPixel.b += db;
                bytesProcessed += 1;
                data += 1;
            }
            else if ( *data >> 6 == OP_LUMA )
            {
                u8 byte = *data;
                s8 dg = ( ( byte ) &0b00111111 ) - 32;
                byte = data[ 1 ];
                s8 db_dg = ( byte & 0b00001111 ) - 8;
                s8 dr_dg = ( ( byte >> 4 ) & 0b00001111 ) - 8;
                previousPixel.r += ( u8 ) ( dr_dg + dg );
                previousPixel.g += dg;
                previousPixel.b += ( u8 ) ( db_dg + dg );
                bytesProcessed += 2;
                data += 2;
            }
            else if ( *data >> 6 == OP_RUN )
            {
                u8 byte = *data;
                u8 runLength = ( byte & 0b00111111 ) + 1;
                for ( u8 i = 0; i < runLength; ++i )
                {
                    pixels[ i * 4 + 0 ] = previousPixel.r;
                    pixels[ i * 4 + 1 ] = previousPixel.g;
                    pixels[ i * 4 + 2 ] = previousPixel.b;
                    if ( result.channels == 4 )
                    {
                        pixels[ i * 4 + 3 ] = previousPixel.a;
                    }
                }
                wasRun = true;
                bytesProcessed += 1;
                data += 1;
                pixels += result.channels * runLength;
            }

            if ( !wasRun )
            {
                pixels[ 0 ] = previousPixel.r;
                pixels[ 1 ] = previousPixel.g;
                pixels[ 2 ] = previousPixel.b;
                if ( result.channels == 4 )
                {
                    pixels[ 3 ] = previousPixel.a;
                }
                pixels += result.channels;
            }
            int pixelIndex = ( previousPixel.r * 3 + previousPixel.g * 5 + previousPixel.b * 7 + previousPixel.a * 11 ) % 64;
            seenPixels[ pixelIndex ] = previousPixel;
        }
    }

    return result;
}
