#version 330 core
in vec3 vNormal;
in vec2 vUV;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vFragPos;

uniform vec3 uViewPos;
uniform vec3 uObjectColor;
uniform bool uUseColor; // true -> use color; false -> use textures

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
    float falloff;
};

uniform PointLight uPointLight;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
};

uniform Material uMaterial;


out vec4 FragColor;

float attenuation_soft(float distance, float radius, float max_intensity, float falloff) {
    float s = distance / radius;

    if (s >= 1.0)
        return 0.0;

    float s2 = s * s;

    return max_intensity * (1 - s2) * (1 - s2) / (1 + falloff * s2);
}

float attenuation_hard(float distance, float radius, float max_intensity, float falloff) {
    float s = distance / radius;

    if (s >= 1.0)
        return 0.0;

    float s2 = s * s;

    return max_intensity * (1 - s2) * (1 - s2) / (1 + falloff * s);
}

void phong(out vec3 ambient, out vec3 diffuse, out vec3 specular) {
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uPointLight.position - vFragPos);
    
    float ambientStrength = 0.1f;
    ambient = ambientStrength * uPointLight.color;

    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    diffuse = diffuseStrength * uPointLight.color;

    float specularStrength = 0.3f;
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    int shininess = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    specular = specularStrength * spec * uPointLight.color;
}

void main()
{
    vec3 objectColor;
    if (!uUseColor) {
        objectColor = texture2D(uMaterial.texture_diffuse1, vUV).xyz;
    }
    else {
        objectColor = uObjectColor;
    }
    
    vec3 ambient, diffuse, specular;
    phong(ambient, diffuse, specular);
    vec3 sky_ambient = 0.1f * vec3(1.0f);
    
    float att = attenuation_soft(distance(vFragPos, uPointLight.position), uPointLight.radius, uPointLight.intensity, uPointLight.falloff);

    vec3 result = (att * (ambient + diffuse + specular)) * objectColor;
    //vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
    
}