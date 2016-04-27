#version 450
 
in vec4 myVertex;
in vec4 color;
uniform mat4 mvp;

out vec3 fcolor;
void main (void)
{
  vec3 normal = vec3(myVertex.xyz);
  vec3 light = vec3(0.0, 2.0, 0.0);
  vec3 view = vec3(0.0, 0.0, 2.0);

  vec3 amb = vec3(0.2, 0.2, 0.8);
  vec3 dif = vec3(0.2, 0.2, 0.8);
  vec3 spc = vec3(0.1, 0.1, 0.1);
  float shi = 30;

  float ndotl = dot(normalize(normal), normalize(light));
  fcolor = 0.2 * amb;
  if (ndotl > 0) {
    fcolor += 0.60 * ndotl * dif;
    vec3 refl = normalize(reflect(-light,normal));
    fcolor += 1.0*pow(max(0,dot(refl,normalize(view))),shi);
  }

  gl_Position = mvp * myVertex;
}