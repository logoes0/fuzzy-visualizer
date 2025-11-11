#version 330 core
in vec3 Color;

out vec4 FragColor;

void main()
{
    // Use vertex colors to distinguish faces (no lighting, but color variation)
    // This makes the cube structure visible without smooth shading
    FragColor = vec4(Color * 0.7, 1.0); // Use vertex colors, slightly dimmed
}
