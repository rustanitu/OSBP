#version 450

in vec3 vert;

uniform sampler2D noise_tex;

out vec3 color;
void main()
{
  vec4 tex = texture(noise_tex, vert.xy);
  //color = vec3(tex.a / 32);
  //return;
  //color = vec3((tex.r + tex.g + tex.b + tex.a) / 236);
  vec3 blueStars = vec3(tex.a) - vec3(1);
  blueStars *= vec3(1,1,1.5);
  blueStars = clamp(blueStars, 0, 1);
  vec3 planets = vec3(tex.b) - vec3(1.5);
  planets *= vec3(5,3,1);
  planets = clamp(planets, 0, 1);
  
  vec3 clouds = vec3(tex.r + tex.g) / 192;
  clouds *= 0.5;
  clouds += 0.5;
  clouds *= 0.4;
  clouds -= 0.1;
  clouds = clamp(clouds, 0, 1);
  vec3 blue = abs(clouds - vec3(1)) * vec3(0,0,0.1);
  blue -= vec3(0.1);

  color = blueStars + planets + clouds + blue;
}