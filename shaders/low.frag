#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main() {
    // Strong pixelation + color dimming
    float pixelSize = 0.05; // bigger = lower quality
    vec2 coord = floor(TexCoord / pixelSize) * pixelSize;
    vec4 texColor = texture(ourTexture, coord);

    FragColor = texColor * 0.7; // dim colors a bit
}
