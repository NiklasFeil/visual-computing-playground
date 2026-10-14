#version 330 core
in vec3 vNormal;
in vec2 vUV;
in vec3 vTangent;
in vec3 vBitangent;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular1;

out vec4 FragColor;

void main()
{
    FragColor = texture2D(texture_diffuse1, vUV);
}