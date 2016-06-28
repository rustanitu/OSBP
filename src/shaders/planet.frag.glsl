#version 450

in vec3 vert;

uniform sampler3D noise_tex;

out vec3 color;
void main()
{
  vec4 tex = texture(noise_tex, vert);
  color = vec3((tex.r + tex.g + tex.b + tex.a) / 300);
  vec3 blue = abs(color - vec3(1)) * vec3(0,0,0.6);
  color += blue;
}