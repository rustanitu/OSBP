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
  vert = (m * vec4(vertex, 1)).xyz;
  norm = (_m * vec4(normal, 1)).xyz;
}