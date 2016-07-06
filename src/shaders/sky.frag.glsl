#version 450

in vec3 vert;

uniform sampler2D noise_tex;

out vec4 color;

vec3 colorStar(vec2 v)
{
  vec4 tex = texture(noise_tex, v);
  vec3 blueStars = vec3(tex.a);
  blueStars = clamp(blueStars - 0.5, 0, 1);
  blueStars *= vec3(1,1,2);
  return blueStars;
}
void main()
{
  vec4 tex = texture(noise_tex, vert.xy);

  vec3 star = colorStar(vert.xy);
  vec3 n0 = colorStar(vert.xy + dFdx(vert.xy));
  vec3 n1 = colorStar(vert.xy - dFdx(vert.xy));
  vec3 n2 = colorStar(vert.xy + dFdy(vert.xy));
  vec3 n3 = colorStar(vert.xy - dFdy(vert.xy));
  vec3 n4 = colorStar(vert.xy + dFdx(vert.xy) + dFdy(vert.xy));
  vec3 n5 = colorStar(vert.xy + dFdx(vert.xy) - dFdy(vert.xy));
  vec3 n6 = colorStar(vert.xy - dFdx(vert.xy) + dFdy(vert.xy));
  vec3 n7 = colorStar(vert.xy - dFdx(vert.xy) - dFdy(vert.xy));

  float lstar = length(star);
  float ln0 = length(n0);
  float ln1 = length(n1);
  float ln2 = length(n2);
  float ln3 = length(n3);
  float ln4 = length(n4);
  float ln5 = length(n5);
  float ln6 = length(n6);
  float ln7 = length(n7);
  
  if (lstar > ln0 && ln0 > 0.25 &&
      lstar > ln1 && ln1 > 0.25 &&
      lstar > ln2 && ln2 > 0.25 &&
      lstar > ln3 && ln3 > 0.25 &&
      lstar > ln4 && ln4 > 0.25 &&
      lstar > ln5 && ln5 > 0.25 &&
      lstar > ln6 && ln6 > 0.25 &&
      lstar > ln7 && ln7 > 0.25 )
    star = vec3(1);
  
  color = vec4(tex.r, 0, tex.g * 0.5 + 0.25, 0);
  color *= 0.2;
  color.a = 1;
  color += vec4(star, 1);
}