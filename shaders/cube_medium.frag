#version 330 core
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    vec3 baseColor = vec3(0.2, 0.4, 0.8); // Blue color
    
    // Ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * baseColor;

    // Diffuse lighting only (no specular for medium quality)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * baseColor;

    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}
