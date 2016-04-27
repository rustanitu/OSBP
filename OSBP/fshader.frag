#version 450

in vec3 normal;
in vec3 light; 
in vec3 view;

out vec3 fragcolor;
void main (void) 
{
  vec3 amb = vec3(0.2, 0.2, 0.8);
  vec3 dif = vec3(0.2, 0.2, 0.8);
  vec3 spc = vec3(0.1, 0.1, 0.1);
  float shi = 30;

  float ndotl = dot(normalize(normal), normalize(light));
  fragcolor = 0.2 * amb;
  if (ndotl > 0) {
    fragcolor += 0.60 * ndotl * dif;
    vec3 refl = normalize(reflect(-light,normal));
    fragcolor += 1.0*pow(max(0,dot(refl,normalize(view))),shi);
  }
}