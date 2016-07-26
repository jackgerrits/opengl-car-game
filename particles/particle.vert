#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 st;

void main(void){
    gl_Position = projection * view * model * vec4(position, 1.0);
    st = vec2(tex_coord.x, 1.0 - tex_coord.y);
}