#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
};

out vec4 fragmentColor;  

in vec2 textureCoords;
in vec3 normal;
in vec3 fragmentPosition;

uniform Material material;
uniform vec4 color;

void main()
{
    fragmentColor = color;
    /* fragmentColor = texture(material.texture_diffuse1, textureCoords); */
    /* fragmentColor = vec4(255.0, 0.0, 255.0, 1.0); */
}

