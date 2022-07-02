#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

#ifndef UNITY_BUILD
    #include "shared.h"
#endif

using Microsoft::WRL::ComPtr;

#define padded_float32( name ) \
    float32 name;              \
    float32 padding_##name[ 3 ];

#define MAX_ADAPTERS 4

struct Constant_Buffer_VS
{
    Matrix4 modelViewProjection;
    Vector4 color;
};

struct Vertex
{
    Vector3 position;
    Vector3 normal;
    Vector2 textureCoords;

    Vertex() { }

    Vertex( float32 x, float32 y, float32 z, float32 nx, float32 ny, float32 nz, float32 u, float32 v )
    {
        position = Vector3( x, y, z );
        normal = Vector3( nx, ny, nz );
        textureCoords = Vector2( u, v );
    }
};

struct Graphics_Adapter
{
    IDXGIAdapter *adapter;
    DXGI_ADAPTER_DESC description;
};

struct Vertex_Shader
{
    ComPtr< ID3D11VertexShader > shader;
    ComPtr< ID3D10Blob > shaderBuffer;
    ComPtr< ID3D11InputLayout > inputLayout;
};

struct Pixel_Shader
{
    ComPtr< ID3D11PixelShader > shader;
    ComPtr< ID3D10Blob > shaderBuffer;
};

struct DirectX
{
    ComPtr< ID3D11Device > device;
    ComPtr< ID3D11DeviceContext > deviceContext;
    ComPtr< IDXGISwapChain > swapChain;
    ComPtr< ID3D11RenderTargetView > renderTargetView;
    ComPtr< ID3D11RasterizerState > rasterizerState;

    ComPtr< ID3D11DepthStencilView > depthStencilView;
    ComPtr< ID3D11Texture2D > depthStencilBuffer;
    ComPtr< ID3D11DepthStencilState > depthStencilState;

    ComPtr< ID3D11SamplerState > samplerState;
    ComPtr< ID3D11ShaderResourceView > texture;

    Vertex_Shader vertexShader;
    Pixel_Shader pixelShader;

    ComPtr< ID3D11Buffer > vertexBuffer;
    ComPtr< ID3D11Buffer > indexBuffer;
    ComPtr< ID3D11Buffer > constantBuffer;

    Graphics_Adapter availableAdapters[ MAX_ADAPTERS ];
    int availableAdaptersCount = 0;
    IDXGIAdapter *adapter;

    Matrix4 viewProjectionMatrix;
};

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
