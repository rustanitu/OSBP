#version 450

in vec3 vert;
uniform float size;
out vec4 color;

void main()
{
  float radius = length(vert);
  radius /= size;
  if (radius <= 1)
  {
    radius += 1;
    color = vec4(1, radius - 0.8, 0, (2 - radius)*(2 - radius));
  }
  else
    color = vec4(0);
}