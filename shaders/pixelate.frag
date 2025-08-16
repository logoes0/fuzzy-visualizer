#version 330 core
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float pixelSize;

out vec4 FragColor;

void main()
{
    // Snap UVs to pixel grid to create pixelation effect
    vec2 texSize = textureSize(screenTexture, 0);
    vec2 pixelatedCoord = floor(TexCoord * texSize / pixelSize) * pixelSize / texSize;
    
    // Sample the texture at the pixelated coordinates
    FragColor = texture(screenTexture, pixelatedCoord);
}
