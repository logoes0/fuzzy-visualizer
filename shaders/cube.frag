#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;

out vec4 FragColor;

void main()
{
    // Use monochrome blue instead of vertex colors
    vec3 baseColor = vec3(0.2, 0.4, 0.8); // Blue color
    
    // Ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * baseColor;
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * baseColor;
    
    // Specular lighting
    float specularStrength = 0.4;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Attenuation based on distance
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    
    vec3 result = ambient + (diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);
}
