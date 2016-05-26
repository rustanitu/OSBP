#version 450

in vec3 wrl_position;  
in vec3 wrl_normal;
in vec2 st;
in vec3 tangent;
in vec3 bitangent;

uniform vec3 light; 
uniform vec3 eye;
uniform vec3 amb;
uniform vec3 diff;
uniform vec3 spec;
uniform float shi;
uniform sampler2D normtexture;
uniform sampler2D difftexture;

vec3 expand (vec3 v)
{
  return 2 * (v - vec3(0.5f, 0.5f, 0.5f));
}

out vec3 color;
out vec3 secondColor;
void main()
{
  mat3 rot = transpose(mat3(tangent, bitangent, wrl_normal));
  vec3 wrl_vNormal = expand(texture(normtexture, st).rgb);
  vec3 wrl_vLight = normalize(rot * (light - wrl_position));
  vec3 wrl_vEye = normalize(rot * (eye - wrl_position));
  
  // Diffuse
  float ndotl = dot(wrl_vNormal, wrl_vLight);
  vec3 cdiff = diff * max(ndotl, 0.0f);
  
  // Specular
  vec3 cspec = vec3(0.0f);
  if (ndotl > 0.0f)
  {
    vec3 wrl_vReflection = normalize(reflect(-wrl_vLight, wrl_vNormal));
    cspec = spec * pow(max(dot(wrl_vReflection, wrl_vEye), 0.0f), shi);
  }
  
  // Final Color
  color = amb + mix(cdiff, texture(difftexture, st).rgb, 0.25) + cspec;
  secondColor = wrl_vLight;
} 