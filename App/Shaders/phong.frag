#version 330 core
in vec3 vNormal;
in vec2 vUV;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vFragPos;

uniform vec3 uViewPos;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Light uLight;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular1;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 lightDir = vec3(uLight.position - vFragPos);
    vec3 objectColor = texture2D(texture_diffuse1, vUV).xyz;
    
    float ambientStrength = 0.4f;
    vec3 ambient = ambientStrength * uLight.color;

    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * uLight.color;

    float specularStrength = 0.0f;
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    int shininess = 4;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * uLight.color;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0f);
}