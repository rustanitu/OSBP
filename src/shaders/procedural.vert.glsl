#version 450

in vec3 vertex;

uniform mat4 mv;
uniform mat4 mvp;

out vec2 st;
out vec3 wrl_pos;
void main()
{
  vec4 vert = vec4(vertex, 1.0f);
  gl_Position = mvp * vert;
  wrl_pos = vertex * 0.5;
  wrl_pos += vec3(0.5);
}