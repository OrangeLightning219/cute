struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 textureCoords : TEXTURE_COORDS;
    float4 color : COLOR;
};

Texture2D texture0 : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 pixelColor = input.color;
    /* float3 pixelColor = texture0.Sample(samplerState, input.textureCoords).xyz; */
    return pixelColor;
}

