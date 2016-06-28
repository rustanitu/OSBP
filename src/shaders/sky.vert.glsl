#version 450

in vec3 vertex;

uniform float size;
uniform mat4 mvp;

out vec3 vert;
void main()
{
  vert = vertex / 5;
  vert += vec3(0.5);
  gl_Position = mvp * vec4(vertex, 1.0f);
}