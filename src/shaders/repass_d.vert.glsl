#version 450

in vec3 vertex;
in vec3 normal;

uniform mat4 m;
uniform mat4 _m;

uniform mat4 mvp;

out vec3 vert;
out vec3 norm;
void main()
{
  gl_Position = mvp * vec4(vertex, 1);
  vert = mat3(m) * vertex;
  norm = mat3(_m) * normal;
}