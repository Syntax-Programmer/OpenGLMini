#version 460 core

layout (location = 0) in ivec3 vert_pos;
layout (location = 1) in ivec3 color;

out vec3 frag_rgb;

void main() {
    frag_rgb = vec3((color.x / 255),
                    (color.y / 255),
                    (color.z / 255));

    gl_Position = vec4(((vert_pos.x / 1000) * 2.0) - 1.0,
                       ((vert_pos.y / 800) * 2.0) - 1.0,
                       (vert_pos.z * 2.0) - 1.0,
                       1.0);

}
