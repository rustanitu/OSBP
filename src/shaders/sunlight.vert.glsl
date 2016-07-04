#version 450

in vec3 vertex;

uniform mat4 mvp;

out vec3 vert;
void main()
{
  vert = vertex;
  gl_Position = mvp * vec4(vertex, 1.0f);
}