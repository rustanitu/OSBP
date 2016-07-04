#version 450

in vec3 vert;
in vec3 texpos;

uniform float time;
uniform sampler3D turb_tex;

out vec4 colorout;

vec4 corona_color(float r, float intens)
{
  float a = 1 - r;
  vec4 color = vec4(1, r, 0, 1) * intens;
  return clamp(color, 0, 1);
}

vec4 corona(vec3 v, float radius)
{
  vec4 tex = texture(turb_tex, v);
  float dr = (8*tex.r + 4*tex.g + 4*tex.b);
  return corona_color(radius, 1);
}

void main()
{
  colorout = vec4(0);
  float radius = length(vert);
  if (radius <= 1.5 && radius >= 1)
  {
    colorout = corona(texpos, radius-1);
  }
  
  radius /= 5;
  radius += 1;
  colorout += vec4(1, radius - 0.8, 0, (2 - radius)*(2 - radius));
}