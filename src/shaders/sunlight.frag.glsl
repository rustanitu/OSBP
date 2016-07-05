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
    float y = 0.5 * (radius - 1) * (radius - 1) + 0.5;
    color = vec4(1, y, 0, (2 - radius)*(2 - radius));
  }
  else
    color = vec4(0);
}