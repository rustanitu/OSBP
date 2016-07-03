#version 450

in vec3 vert;

uniform sampler2D noise_tex;

out vec4 color;
void main()
{
  vec4 tex = texture(noise_tex, vert.xy);
  vec3 blueStars = vec3(tex.a);
  blueStars = clamp(blueStars - 0.5, 0, 1);
  blueStars *= vec3(1.5, 1.5, 2);
  //blueStars *= blueStars;
  //blueStars -= vec3(1);
  //blueStars.z *= 1.5;
  if (blueStars.b > 1)
    blueStars = vec3(1,1,1);

  //blueStars = tex.a * tex.a * vec3(0.25, 0.25, 0.4);

  vec3 planets = vec3(tex.b * tex.b) - vec3(1.5);
  planets *= vec3(5,3,1);
  planets = clamp(planets, 0, 1);

  color = vec4(blueStars, 1);
}