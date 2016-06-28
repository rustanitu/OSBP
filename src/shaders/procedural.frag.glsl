#version 450

in vec3 wrl_pos;
in vec2 st;

uniform sampler3D noise_tex;

out vec3 color;
void main()
{
  vec4 tex = texture(noise_tex, wrl_pos);
  color = vec3((tex.r + tex.g + tex.b + tex.a) / 300);
}