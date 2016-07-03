#version 330

layout (location = 0) in vec3 a_vertex;

out vec4 clipSpace;
out vec2 texCoords;
out vec3 vertCamDir;
out vec4 vertex_view;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 camera_position;

uniform vec3 lightPosition;

const float tileFactor = 5.0;

void main(void) {
    vec4 worldPos = model * vec4(a_vertex, 1.0);
    vertex_view = view * worldPos;
    clipSpace = projection * vertex_view; 
    gl_Position = clipSpace;
    texCoords = vec2((a_vertex.x/2.0 + 0.5)*2, a_vertex.z/2.0 + 0.5) * tileFactor;
    vertCamDir = camera_position - worldPos.xyz;
}