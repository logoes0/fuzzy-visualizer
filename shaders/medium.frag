#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main() {
    // Pixelation effect
    float pixelSize = 0.01; // smaller = higher quality
    vec2 coord = floor(TexCoord / pixelSize) * pixelSize;
    vec4 texColor = texture(ourTexture, coord);

    FragColor = texColor; // show full image, slightly blocky
}
