#version 450

in vec3 vertex;

uniform int size;
uniform int slice;

out vec3 vert;
void main()
{
  gl_Position = vec4(vertex, 1.0f);
  vert = vertex * 0.5;
  vert += 0.5;
  vert.z = slice / (size-1);
}