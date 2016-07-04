#version 450

in vec3 vert;

uniform sampler3D noise_tex;

out vec4 color;

void main()
{
  vec4 tex = texture(noise_tex, vert);
  color = vec4((tex.r + tex.g + tex.b + tex.a) / 4);
  color *= 0.5;
  color += 0.5;

  vec4 light = abs(color) - vec4(0.5);
  vec4 shi = light * vec4(1) * 2;
  light = clamp(light, 0, 1);
  light += shi;
  
  color = clamp(color, 0, 1);

  vec4 red = abs(color - vec4(1)) * vec4(0.5,0,0,0);
  vec4 yellow = color * vec4(1,1,0, 0);
  
  
  color = red + yellow + light;
  color.a = 1;
}