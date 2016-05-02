#version 450
 
in vec3 vertex;
in vec3 normal;
in vec2 texcoord;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 tinv_model;

out vec3 wrl_normal;
out vec3 wrl_position;
out vec2 st;
void main()
{
    gl_Position = mvp * vec4(vertex, 1.0f);
    wrl_position = vec3(model * vec4(vertex, 1.0f));
    wrl_normal = vec3(tinv_model * vec4(normal, 1.0f));
    st = texcoord;
} 