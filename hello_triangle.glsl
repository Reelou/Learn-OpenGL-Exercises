#version 120

layout (location = 0) in vec3 position;

void main() {
    gl_position = vec4(position.x, position.y, position.z, 1.0);
}