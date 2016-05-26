#version 450

in vec3 vert;
in vec3 norm;

uniform vec3 mamb;
uniform vec3 mdiff;
uniform vec3 mspec;
uniform float mshi;

out vec3 position;
out vec3 normals;
out vec3 ambcolor;
out vec3 diffcolor;
out vec4 specolor;

void main ()
{
  position = vert;
  normals = norm;
  ambcolor = mamb;
  diffcolor = mdiff;
  specolor = vec4(mspec, mshi);
}