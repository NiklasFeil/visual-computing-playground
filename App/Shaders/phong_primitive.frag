#version 330 core
in vec3 vNormal;
in vec3 vFragPos;

uniform vec3 uObjectColor;
uniform vec3 uViewPos;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Light uLight;

out vec4 FragColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLight.color;
  	
    // diffuse 
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLight.position - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLight.color;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    const float shininess = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * uLight.color;  
        
    vec3 result = (ambient + diffuse + specular) * uObjectColor;

    FragColor = vec4(result, 1.0);
}