#version 450

in vec3 wrl_position;  
in vec3 wrl_normal;

uniform vec3 light; 
uniform vec3 eye;
uniform vec3 amb;
uniform vec3 diff;
uniform vec3 spec;
uniform float shi;

out vec3 color;
void main()
{
  vec3 wrl_vLight = normalize(light - wrl_position);
  vec3 wrl_vEye = normalize(eye - wrl_position);
  
  // Diffuse
  float ndotl = dot(wrl_normal, wrl_vLight);
  vec3 cdiff = diff * max(ndotl, 0.0f);
  
  // Specular
  vec3 cspec = vec3(0.0f);
  if (ndotl > 0.0f)
  {
    vec3 wrl_vReflection = normalize(reflect(-wrl_vLight, wrl_normal));
    cspec = spec * pow(max(dot(wrl_vReflection, wrl_vEye), 0.0f), shi);
  }
  
  // Final Color
  color = amb;// + cdiff + cspec;
} 