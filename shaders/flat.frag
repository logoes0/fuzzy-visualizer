#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * Color;
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * Color;
    
    // Simple flat shading - no specular
    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}
