#version 450

in vec3 vert;
in vec3 texpos;

uniform float time;
uniform sampler3D turb_tex;
uniform sampler2D noise_tex;

out vec4 colorout;

vec4 corona_color(float r, float intens, float shi)
{
  float a = 2 * (r - 0.5) * (r - 0.5) * (r - 0.5) * (r - 0.5);
  float yel = 0.5 + r;
  vec4 color = vec4(1, yel, yel * 0.02, a ) * intens;
  return color;
}

vec4 corona(vec3 v, float radius)
{
  vec4 tex = texture(turb_tex, v);
  vec4 shi = texture(noise_tex, v.xy);
  float dr = (tex.r + tex.g + tex.b) * 5 + tex.a * 3;
  dr *= dr;
  return corona_color(radius + 0.15, dr, abs(shi.g));
}

void main()
{
  colorout = vec4(0, 0, 0, 0);
  float radius = length(vert);
  if (radius <= 1.4 && radius > 0.9)
  {
    colorout = corona(texpos, radius-1);
  }
}