#version 330 core
in vec3 Color;

out vec4 FragColor;

void main()
{
    // Simple flat shading - no lighting calculations
    vec3 baseColor = vec3(0.2, 0.4, 0.8); // Blue color
    FragColor = vec4(baseColor * 0.8, 1.0); // Slightly darker for low quality
}
