cbuffer buffer : register(b0)
{
    float4x4 modelViewProjection;
    //float4x4 model;
    float4 color;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 textureCoords : TEXTURE_COORDS;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 textureCoords : TEXTURE_COORDS;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(modelViewProjection, float4(input.position, 1.0f) );
    /* output.position = mul(float4(input.position, 1.0f), modelViewProjection); */
    //output.normal = mul(float3x3(transpose(inverse(model))), input.normal);
    output.normal = input.normal;
    output.textureCoords = input.textureCoords;
    output.color = color;
    return output;
}
