#version 330 core
in vec3 vNormal;
in vec2 vUV;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vFragPos;

#define NUM_POINT_LIGHTS 1
#define NUM_SPOTLIGHTS 1

uniform vec3 uViewPos;
uniform bool uUseTextures; // true -> use textures; false -> use hand-defined simple material without textures

struct PointLight {
    vec3 position;
    vec3 color;
    vec3 intensity; // ambient, diffuse, specular
    float radius;
    float falloff;
};

struct Spotlight {
    vec3 position;
    vec3 color;
    vec3 intensity; // ambient, diffuse, specular
    vec3 direction;
    float radius;
    float falloff;
    float inner_cutoff;
    float outer_cutoff;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    vec3 intensity; // ambient, diffuse, specular
};

uniform PointLight[NUM_POINT_LIGHTS] uPointLights;
uniform Spotlight[NUM_SPOTLIGHTS] uSpotlights;
uniform DirectionalLight uDirectionalLight;

// Material using textures
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
};

uniform Material uMaterial;

// Simple Material without textures
struct MaterialSimple {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform MaterialSimple uMaterialSimple;

out vec4 FragColor;

// (x, y, z) = (ambient_att, diffuse_att, spec_att)
vec3 attenuation_soft(float distance, vec3 intensity, float radius, float falloff) {
    float s = distance / radius;

    if (s >= 1.0)
        return vec3(0.0);

    float s2 = s * s;

    return intensity * (1 - s2) * (1 - s2) / (1 + falloff * s2);
}

vec3 attenuation_hard(float distance, vec3 intensity, float radius, float falloff) {
    float s = distance / radius;

    if (s >= 1.0)
        return vec3(0.0);

    float s2 = s * s;

    return intensity * (1 - s2) * (1 - s2) / (1 + falloff * s);
}

vec3 phong_directional_light(Material mat, DirectionalLight light, vec3 viewDir, vec3 normal) {
    vec3 ambient, diffuse, specular;

    vec3 lightDir = normalize(-light.direction);

    vec3 objectColor = texture2D(mat.texture_diffuse1, vUV).xyz;

    ambient = light.color * objectColor;

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    diffuse = diffuseStrength * light.color * objectColor;

    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 4.0f;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    specular = spec * light.color * objectColor;

    return light.intensity.x * ambient + light.intensity.y * diffuse + light.intensity.z * specular;
}

vec3 phong_point_light(Material mat, PointLight light, vec3 viewDir, vec3 normal) {
    vec3 ambient, diffuse, specular;

    vec3 lightDir = normalize(light.position - vFragPos);

    vec3 objectColor = texture2D(mat.texture_diffuse1, vUV).xyz;

    ambient = light.color * objectColor;

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    diffuse = diffuseStrength * light.color * objectColor;

    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 4.0f;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    specular = spec * light.color * objectColor;

    vec3 att = attenuation_soft(distance(vFragPos, light.position), light.intensity, light.radius, light.falloff);

    return att.x * ambient + att.y * diffuse + att.z * specular;
}

vec3 phong_spotlight(Material mat, Spotlight light, vec3 viewDir, vec3 normal) {
    vec3 ambient = vec3(0.0f), diffuse = vec3(0.0f), specular = vec3(0.0f);

    vec3 fragToLightDir = normalize(light.position - vFragPos);

    vec3 objectColor = texture2D(mat.texture_diffuse1, vUV).xyz;

    ambient = light.color * objectColor;

    float theta = dot(-fragToLightDir, normalize(light.direction));

    // we are comparing cosines, so if the angle is smaller, the value is higher
    if (theta > light.inner_cutoff) {

        float diffuseStrength = max(dot(normal, fragToLightDir), 0.0);
        diffuse = diffuseStrength * light.color * objectColor;

        vec3 reflectDir = reflect(-fragToLightDir, normal);
        float shininess = 4.0f;
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular = spec * light.color * objectColor;
    }
    else if(theta > light.outer_cutoff) {
        float eps = mix(0.0f, 1.0f, (theta - light.outer_cutoff) / abs(light.inner_cutoff - light.outer_cutoff));

        float diffuseStrength = max(dot(normal, fragToLightDir), 0.0);
        diffuse = diffuseStrength * light.color * objectColor * eps;

        vec3 reflectDir = reflect(-fragToLightDir, normal);
        float shininess = 4.0f;
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular = spec * light.color * objectColor * eps;
    }
    
    vec3 att = attenuation_soft(distance(vFragPos, light.position), light.intensity, light.radius, light.falloff);

    return att.x * ambient + att.y * diffuse + att.z * specular;
}

vec3 phong_directional_light(MaterialSimple mat, DirectionalLight light, vec3 viewDir, vec3 normal) {
    vec3 ambient, diffuse, specular;

    vec3 lightDir = normalize(-light.direction);

    ambient = light.color * mat.ambient;

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    diffuse = diffuseStrength * light.color * mat.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    specular = spec * light.color * mat.specular;

    return light.intensity.x * ambient + light.intensity.y * diffuse + light.intensity.z * specular;
}

vec3 phong_point_light(MaterialSimple mat, PointLight light, vec3 viewDir, vec3 normal) {
    vec3 ambient, diffuse, specular;

    vec3 lightDir = normalize(light.position - vFragPos);

    ambient = light.color * mat.ambient;

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    diffuse = diffuseStrength * light.color * mat.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    specular = spec * light.color * mat.specular;

    vec3 att = attenuation_soft(distance(vFragPos, light.position), light.intensity, light.radius, light.falloff);

    return att.x * ambient + att.y * diffuse + att.z * specular;
}

vec3 phong_spotlight(MaterialSimple mat, Spotlight light, vec3 viewDir, vec3 normal) {
    vec3 ambient = vec3(0.0f), diffuse = vec3(0.0f), specular = vec3(0.0f);

    vec3 fragToLightDir = normalize(light.position - vFragPos);

    ambient = light.color * mat.ambient;

    float theta = dot(-fragToLightDir, normalize(light.direction));

    if (theta > light.inner_cutoff) {

        float diffuseStrength = max(dot(normal, fragToLightDir), 0.0);
        diffuse = diffuseStrength * light.color * mat.diffuse;

        vec3 reflectDir = reflect(-fragToLightDir, normal);
        float shininess = 4.0f;
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular = spec * light.color * mat.specular;
    }
    else if(theta > light.outer_cutoff) {
        float eps = mix(0.0f, 1.0f, (theta - light.outer_cutoff) / abs(light.inner_cutoff - light.outer_cutoff));

        float diffuseStrength = max(dot(normal, fragToLightDir), 0.0);
        diffuse = diffuseStrength * light.color * mat.diffuse * eps;

        vec3 reflectDir = reflect(-fragToLightDir, normal);
        float shininess = 4.0f;
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular = spec * light.color * mat.specular * eps;
    }
    
    vec3 att = attenuation_soft(distance(vFragPos, light.position), light.intensity, light.radius, light.falloff);

    return att.x * ambient + att.y * diffuse + att.z * specular;
}

void main()
{
    vec3 result = vec3(0.0f);
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 normal = normalize(vNormal);

    if (uUseTextures) {

        //result += phong_directional_light(uMaterial, uDirectionalLight, viewDir, normal);

        for(int i = 0; i < NUM_POINT_LIGHTS; i++) {
            result += phong_point_light(uMaterial, uPointLights[i], viewDir, normal);
        }

        for(int i = 0; i < NUM_SPOTLIGHTS; i++) {
            result += phong_spotlight(uMaterial, uSpotlights[i], viewDir, normal);
        }

    }
    else {
        //result += phong_directional_light(uMaterialSimple, uDirectionalLight, viewDir, normal);

        for(int i = 0; i < NUM_POINT_LIGHTS; i++) {
            result += phong_point_light(uMaterialSimple, uPointLights[i], viewDir, normal);
        }

        for(int i = 0; i < NUM_SPOTLIGHTS; i++) {
            result += phong_spotlight(uMaterialSimple, uSpotlights[i], viewDir, normal);
        }
    }

    FragColor = vec4(result, 1.0f);   
    
}