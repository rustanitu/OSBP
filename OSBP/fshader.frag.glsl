#version 450

in vec3 wrl_position;  
in vec3 wrl_normal;
in vec2 st;
in vec3 tangent;
in vec3 binormal;

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
void main()
{
  mat3 rot = transpose(mat3(tangent, binormal, wrl_normal));
  vec3 wrl_vNormal = expand(texture(normtexture, st).rgb);
  vec3 wrl_vLight = normalize(rot * (light - wrl_position));
  vec3 wrl_vEye = normalize(rot * (eye - wrl_position));
  vec3 wrl_vReflection = normalize(reflect(-wrl_vLight, wrl_vNormal));
  
  // Diffuse
  vec3 cdiff = diff * max(dot(wrl_vNormal, wrl_vLight), 0.0f);
  cdiff = clamp(cdiff, 0.0f, 1.0f);
  
  // Specular
  vec3 cspec = spec * pow(max(dot(wrl_vReflection, wrl_vEye), 0.0f), shi);
  cspec = clamp(cspec, 0.0f, 1.0f);
  
  // Final Color
  //color = amb + cdiff + cspec;
  color = amb + texture(difftexture, st).rgb + cspec;
} 