#include "renderer.h"

#ifndef UNITY_BUILD
    #include "windows_main.h"
#endif

DirectX directX;

internal void *RendererAllocate( u64 size );
internal bool RendererFree( void *memmory );
internal QOI_Image LoadQOI( char *filename );

Vertex cubeVertices[ 36 ] = {
    // positions         // normals         // texture coords
    Vertex( 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f ),
    Vertex( 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f ),
    Vertex( -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f ),

    Vertex( -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f ),
    Vertex( -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f ),
    Vertex( 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f ),

    Vertex( 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f ),
    Vertex( 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f ),
    Vertex( -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f ),

    Vertex( -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f ),
    Vertex( -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f ),
    Vertex( 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f ),

    Vertex( -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f ),
    Vertex( -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f ),
    Vertex( -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f ),

    Vertex( -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f ),
    Vertex( -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f ),
    Vertex( -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f ),

    Vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f ),
    Vertex( 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f ),
    Vertex( 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f ),

    Vertex( 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f ),
    Vertex( 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f ),
    Vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f ),

    Vertex( 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f ),
    Vertex( 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f ),
    Vertex( -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f ),

    Vertex( -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f ),
    Vertex( -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f ),
    Vertex( 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f ),

    Vertex( 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f ),
    Vertex( 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f ),
    Vertex( -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f ),

    Vertex( -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f ),
    Vertex( -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f ),
    Vertex( 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f ),
};

RENDERER_DRAW_CUBE(RendererDrawCube)
{
    directX.deviceContext->VSSetShader( directX.vertexShader.shader.Get(), NULL, 0 );
    directX.deviceContext->PSSetShader( directX.pixelShader.shader.Get(), NULL, 0 );
    directX.deviceContext->PSSetSamplers( 0, 1, directX.samplerState.GetAddressOf() );
    directX.deviceContext->PSSetShaderResources( 0, 1, directX.texture.GetAddressOf() );

    Constant_Buffer_VS constantBuffer = {};
    constantBuffer.modelViewProjection = directX.viewProjectionMatrix * CreateModelMatrix(transform);
    constantBuffer.color = color;
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HRESULT result = directX.deviceContext->Map( directX.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
    if ( SUCCEEDED( result ) )
    {
        CopyMemory( mappedResource.pData, &constantBuffer, sizeof( Constant_Buffer_VS ) );
        directX.deviceContext->Unmap( directX.constantBuffer.Get(), 0 );

        directX.deviceContext->VSSetConstantBuffers( 0, 1, directX.constantBuffer.GetAddressOf() );
    }
    else
    {
        LogResult( result, "Failed to map constant buffer!" );
    }

    u32 stride = sizeof( Vertex );
    u32 offset = 0;

    directX.deviceContext->IASetVertexBuffers( 0, 1, directX.vertexBuffer.GetAddressOf(), &stride, &offset );
    directX.deviceContext->Draw(ArrayCount(cubeVertices), 0);
    // directX.deviceContext->IASetIndexBuffer( directX.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0 );
    // directX.deviceContext->DrawIndexed( 6, 0, 0 );
}

RENDERER_BEGIN_FRAME( RendererBeginFrame )
{
    float32 background[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    directX.deviceContext->ClearRenderTargetView( directX.renderTargetView.Get(), background );
    directX.deviceContext->ClearDepthStencilView( directX.depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

    directX.deviceContext->IASetInputLayout( directX.vertexShader.inputLayout.Get() );
    directX.deviceContext->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    directX.deviceContext->RSSetState( directX.rasterizerState.Get() );
    directX.deviceContext->OMSetDepthStencilState( directX.depthStencilState.Get(), 0 );

    directX.viewProjectionMatrix = viewProjectionMatrix;
}

RENDERER_END_FRAME( RendererEndFrame )
{
    directX.swapChain->Present( 1, NULL );
}

RENDERER_INIT( RendererInit )
{
    D3D11_BUFFER_DESC vertexBufferDescription = {};
    vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDescription.ByteWidth = sizeof( Vertex ) * ArrayCount( cubeVertices );
    vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDescription.CPUAccessFlags = 0;
    vertexBufferDescription.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData = {};
    vertexBufferData.pSysMem = cubeVertices;

    HRESULT result = directX.device->CreateBuffer( &vertexBufferDescription, &vertexBufferData, directX.vertexBuffer.GetAddressOf() );
    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create vertex buffer!" );
        return false;
    }

//     D3D11_BUFFER_DESC indexBufferDescription = {};
//     indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
//     indexBufferDescription.ByteWidth = sizeof( int ) * ArrayCount( indices );
//     indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
//     indexBufferDescription.CPUAccessFlags = 0;
//     indexBufferDescription.MiscFlags = 0;

//     D3D11_SUBRESOURCE_DATA indexBufferData = {};
//     indexBufferData.pSysMem = indices;

//     result = directX.device->CreateBuffer( &indexBufferDescription, &indexBufferData, directX.indexBuffer.GetAddressOf() );
//     if ( FAILED( result ) )
//     {
//         LogResult( result, "Failed to create index buffer!" );
//         return false;
//     }

    QOI_Image image = LoadQOI( "container.qoi" );
    D3D11_TEXTURE2D_DESC textureDescription = {};
    textureDescription.Width = image.width;
    textureDescription.Height = image.height;
    textureDescription.MipLevels = 1;
    textureDescription.ArraySize = 1;
    textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDescription.SampleDesc.Count = 1;
    textureDescription.SampleDesc.Quality = 0;
    textureDescription.Usage = D3D11_USAGE_DEFAULT;
    textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDescription.CPUAccessFlags = 0;
    textureDescription.MiscFlags = 0;

    ComPtr< ID3D11Texture2D > texture;

    D3D11_SUBRESOURCE_DATA textureData = {};
    textureData.pSysMem = image.pixels;
    textureData.SysMemPitch = image.width * image.channels * sizeof( u8 );
    textureData.SysMemSlicePitch = image.width * image.height * image.channels * sizeof( u8 );

    result = directX.device->CreateTexture2D( &textureDescription, &textureData, texture.GetAddressOf() );
    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create texture!" );
        RendererFree( image.pixels );
        return false;
    }

    RendererFree( image.pixels );
    D3D11_SHADER_RESOURCE_VIEW_DESC textureResource = {};
    textureResource.Format = textureDescription.Format;
    textureResource.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    textureResource.Texture2D.MostDetailedMip = 0;
    textureResource.Texture2D.MipLevels = 1;

    result = directX.device->CreateShaderResourceView( texture.Get(), &textureResource, directX.texture.GetAddressOf() );
    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create shader resource view!" );
        return false;
    }

    D3D11_BUFFER_DESC constantBufferDescription = {};
    constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufferDescription.MiscFlags = 0;
    // needs to be aligned to 16 bytes
    constantBufferDescription.ByteWidth = sizeof( Constant_Buffer_VS ) + ( 16 - ( sizeof( Constant_Buffer_VS ) % 16 ) );
    constantBufferDescription.StructureByteStride = 0;

    result = directX.device->CreateBuffer( &constantBufferDescription, 0, directX.constantBuffer.GetAddressOf() );

    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create constant buffer!" );
        return false;
    }

    return true;
}

internal bool InitializePixelShader( Pixel_Shader *shader, ComPtr< ID3D11Device > &device, char *path )
{
    wchar_t widePath[ 512 ];
    MultiByteToWideChar( CP_ACP, 0, path, -1, widePath, 512 );
    HRESULT result = D3DReadFileToBlob( widePath, shader->shaderBuffer.GetAddressOf() );

    if ( FAILED( result ) )
    {
        LogResultF( result, "Failed to load shader: %s", path );
        return false;
    }

    result = device->CreatePixelShader( shader->shaderBuffer->GetBufferPointer(), shader->shaderBuffer->GetBufferSize(),
                                        NULL, shader->shader.GetAddressOf() );

    if ( FAILED( result ) )
    {
        LogResultF( result, "Failed to create shader: %s", path );
        return false;
    }

    return true;
}

internal bool InitializeVertexShader( Vertex_Shader *shader, ComPtr< ID3D11Device > &device, char *path,
                             D3D11_INPUT_ELEMENT_DESC *layoutDescription, int descriptionCount )
{
    wchar_t widePath[ 512 ];
    MultiByteToWideChar( CP_ACP, 0, path, -1, widePath, 512 );
    HRESULT result = D3DReadFileToBlob( widePath, shader->shaderBuffer.GetAddressOf() );

    if ( FAILED( result ) )
    {
        LogResultF( result, "Failed to load shader: %s", path );
        return false;
    }

    result = device->CreateVertexShader( shader->shaderBuffer->GetBufferPointer(), shader->shaderBuffer->GetBufferSize(),
                                         NULL, shader->shader.GetAddressOf() );

    if ( FAILED( result ) )
    {
        LogResultF( result, "Failed to create shader: %s", path );
        return false;
    }

    result = device->CreateInputLayout( layoutDescription, descriptionCount,
                                        shader->shaderBuffer->GetBufferPointer(),
                                        shader->shaderBuffer->GetBufferSize(),
                                        shader->inputLayout.GetAddressOf() );
    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create input layout!" );
        return false;
    }
    return true;
}

internal bool InitializeShaders()
{
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXTURE_COORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    if ( !InitializeVertexShader( &directX.vertexShader, directX.device, "shaders/vertex.fxc", layout, ArrayCount( layout ) ) )
    {
        return false;
    }

    if ( !InitializePixelShader( &directX.pixelShader, directX.device, "shaders/pixel.fxc" ) )
    {
        return false;
    }
    return true;
}

internal bool InitializeDirectX( HWND window, int windowWidth, int windowHeight )
{
    ComPtr< IDXGIFactory > factory;

    HRESULT result = CreateDXGIFactory( __uuidof( IDXGIFactory ), ( void ** ) factory.GetAddressOf() );
    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create DXGI Factory!" );
        return false;
    }

    IDXGIAdapter *adapter;
    int adapterIndex = 0;
    while ( SUCCEEDED( factory->EnumAdapters( adapterIndex, &adapter ) ) && adapterIndex < MAX_ADAPTERS )
    {
        Graphics_Adapter graphicsAdapter = {};
        graphicsAdapter.adapter = adapter;
        result = adapter->GetDesc( &graphicsAdapter.description );
        if ( FAILED( result ) )
        {
            LogResult( result, "Failed to get adapter description" );
        }
        directX.availableAdapters[ adapterIndex ] = graphicsAdapter;
        adapterIndex += 1;
    }
    directX.availableAdaptersCount = adapterIndex;

    if ( adapterIndex == 0 )
    {
        Log( "No DXGI Adapter found!" );
        return false;
    }

    u64 maxMemory = 0;
    for ( int i = 0; i < directX.availableAdaptersCount; ++i )
    {
        Graphics_Adapter *graphicsAdapter = &directX.availableAdapters[ i ];
        if ( graphicsAdapter->description.DedicatedVideoMemory > maxMemory )
        {
            maxMemory = graphicsAdapter->description.DedicatedVideoMemory;
            directX.adapter = graphicsAdapter->adapter;
        }
    }

    DXGI_SWAP_CHAIN_DESC swapChainDescription = {};
    swapChainDescription.BufferDesc.Width = windowWidth;
    swapChainDescription.BufferDesc.Height = windowHeight;
    swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // antialiasing
    swapChainDescription.SampleDesc.Count = 1;
    swapChainDescription.SampleDesc.Quality = 0;

    swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescription.BufferCount = 1;
    swapChainDescription.OutputWindow = window;
    swapChainDescription.Windowed = true;
    swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

#ifdef INTERNAL
    int deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
    int deviceFlags = 0;
#endif
    result = D3D11CreateDeviceAndSwapChain( directX.adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, deviceFlags, NULL, 0,
                                            D3D11_SDK_VERSION, &swapChainDescription, directX.swapChain.GetAddressOf(),
                                            directX.device.GetAddressOf(), NULL, directX.deviceContext.GetAddressOf() );

    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create device and swap chain!" );
        return false;
    }

    ComPtr< ID3D11Texture2D > backBuffer;
    result = directX.swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( void ** ) backBuffer.GetAddressOf() );
    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to get the back buffer!" );
        return false;
    }

    result = directX.device->CreateRenderTargetView( backBuffer.Get(), NULL, directX.renderTargetView.GetAddressOf() );
    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create render target view!" );
        return false;
    }

    D3D11_TEXTURE2D_DESC depthStencilDescription = {};

    depthStencilDescription.Width = windowWidth;
    depthStencilDescription.Height = windowHeight;
    depthStencilDescription.MipLevels = 1;
    depthStencilDescription.ArraySize = 1;
    depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDescription.SampleDesc.Count = 1;
    depthStencilDescription.SampleDesc.Quality = 0;
    depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDescription.CPUAccessFlags = 0;
    depthStencilDescription.MiscFlags = 0;

    result = directX.device->CreateTexture2D( &depthStencilDescription, NULL, directX.depthStencilBuffer.GetAddressOf() );

    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create depth stencil buffer!" );
        return false;
    }

    result = directX.device->CreateDepthStencilView( directX.depthStencilBuffer.Get(), NULL, directX.depthStencilView.GetAddressOf() );

    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create depth stencil view!" );
        return false;
    }

    directX.deviceContext->OMSetRenderTargets( 1, directX.renderTargetView.GetAddressOf(), directX.depthStencilView.Get() );

    D3D11_DEPTH_STENCIL_DESC depthStencilStateDescription = {};

    depthStencilStateDescription.DepthEnable = true;
    depthStencilStateDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    result = directX.device->CreateDepthStencilState( &depthStencilStateDescription, directX.depthStencilState.GetAddressOf() );

    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create depth stencil state!" );
        return false;
    }

    bool shadersInitialized = InitializeShaders();
    if ( !shadersInitialized )
    {
        Log( "Failed to initialize shaders!" );
        return false;
    }

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = ( float32 ) windowWidth;
    viewport.Height = ( float32 ) windowHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    directX.deviceContext->RSSetViewports( 1, &viewport );

    D3D11_RASTERIZER_DESC rasterizerDescription = {};

    rasterizerDescription.FillMode = D3D11_FILL_SOLID;
    // rasterizerDescription.FillMode = D3D11_FILL_WIREFRAME;
    // rasterizerDescription.CullMode = D3D11_CULL_BACK;
    rasterizerDescription.CullMode = D3D11_CULL_NONE; //@TODO: Switch back to BACK

    result = directX.device->CreateRasterizerState( &rasterizerDescription, directX.rasterizerState.GetAddressOf() );
    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create rasterizer state!" );
        return false;
    }

    D3D11_SAMPLER_DESC samplerDescription = {};

    samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDescription.MinLOD = 0;
    samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

    result = directX.device->CreateSamplerState( &samplerDescription, directX.samplerState.GetAddressOf() );

    if ( FAILED( result ) )
    {
        LogResult( result, "Failed to create sampler state!" );
        return false;
    }

    return true;
}

internal void *RendererAllocate( u64 size )
{
    return VirtualAlloc( 0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
}

internal bool RendererFree( void *memory )
{
    return VirtualFree( memory, 0, MEM_RELEASE );
}

internal inline u32 ToLittleEndian( u32 value )
{
    u8 *bytes = ( u8 * ) &value;
    return bytes[ 3 ] << 0 | bytes[ 2 ] << 8 | bytes[ 1 ] << 16 | bytes[ 0 ] << 24;
}

//@TODO: Add option to flip
internal QOI_Image LoadQOI( char *filename )
{
    QOI_Image result = {};

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

    Debug_Read_File_Result file = DebugPlatformReadEntireFile( filename );
    if ( file.contentSize > 0 )
    {
        QOI_Header *header = ( QOI_Header * ) file.content;
        header->width = ToLittleEndian( header->width );
        header->height = ToLittleEndian( header->height );

        result.width = header->width;
        result.height = header->height;
        result.channels = header->channels;
        result.colorspace = header->colorspace;
        result.pixels = ( u8 * ) RendererAllocate( result.width * result.height * result.channels * sizeof( u8 ) );

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
        DebugPlatformFreeFileMemory( file.content );
    }

    return result;
}
