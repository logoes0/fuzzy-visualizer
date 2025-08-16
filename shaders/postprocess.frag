#version 330 core
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float pixelSize;

out vec4 FragColor;

void main()
{
    // Simply sample the texture - the pixelation comes from the low resolution rendering
    FragColor = texture(screenTexture, TexCoord);
}
