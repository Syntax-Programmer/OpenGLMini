#version 460 core

in vec3 frag_rgb;
out vec4 frag_color;

void main() {
    frag_color = vec4(frag_rgb, 1.0f);
}