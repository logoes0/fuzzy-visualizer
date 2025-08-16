#version 330 core
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float pixelSize;

out vec4 FragColor;

void main()
{
    // Snap UVs to pixel grid to create pixelation effect
    // Using the simplified formula: floor(TexCoord * pixelSize) / pixelSize
    vec2 pixelatedCoord = floor(TexCoord * pixelSize) / pixelSize;
    
    // Sample the texture at the pixelated coordinates
    FragColor = texture(screenTexture, pixelatedCoord);
}
