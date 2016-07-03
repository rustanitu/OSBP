#version 450

in vec3 vert;
in vec3 texpos;

uniform float time;
uniform sampler3D turb_tex;

out vec4 colorout;

vec4 corona_color(float r, float intens)
{
  float d = (0.9 - r)*(0.9 - r);
  vec4 color = vec4(1, d, 0, d) * intens;
  return color;
}

vec4 corona(vec3 v, vec3 r)
{
  float radius = length(r);
  if (radius > 2 || radius < 0.9)
    return vec4(0);
  radius -= 1.0;
  vec4 tex = texture(turb_tex, v);
  float dr = (tex.r + tex.g * 2 + tex.b + tex.a) / 80;
  return corona_color(radius, dr);
}

void main()
{
  colorout = corona(texpos, vert);
}