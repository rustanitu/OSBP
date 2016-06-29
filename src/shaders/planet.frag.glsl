#version 450

in vec3 vert;

uniform float time;
uniform sampler3D noise_tex;

out vec3 color;
void main()
{
  vec4 tex = texture(noise_tex, vert * time);
  color = vec3((tex.r + tex.g + tex.b + tex.a) / 200);
  color *= 0.5;
  color += 0.5;

  vec3 light = abs(color) - vec3(0.5);
  vec3 shi = light * vec3(1) * 2;
  light = clamp(light, 0, 1);
  light += shi;
  
  color = clamp(color, 0, 1);

  vec3 red = abs(color - vec3(1)) * vec3(1,0,0);
  vec3 yellow = color * vec3(1,1,0);
  
  //vec3 comp = clamp(red + yellow, 0, 1);
  
  color = red + yellow + light;
  //color *= 0.5;
}