#version 450

in vec3 vertex;
in vec2 texcoord;

uniform mat4 mvp;

out vec2 st;
void main()
{
  gl_Position = mvp * vec4(vertex, 1.0f);
  st = texcoord;
}