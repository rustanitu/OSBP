#version 450

in vec3 vertex;
in vec2 texcoord;

out vec2 st;
void main()
{
  gl_Position = vec4(vertex, 1);
  st = texcoord;
}