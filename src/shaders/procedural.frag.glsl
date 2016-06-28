#version 450

in vec3 wrl_pos;
in vec2 st;

uniform sampler3D noise_tex;

out vec3 color;
void main()
{
  vec4 noise = texture(noise_tex, wrl_pos);
  color = noise.xyz;
}