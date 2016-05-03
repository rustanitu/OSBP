#version 450

in vec3 wrl_position;  
in vec3 wrl_normal;
in vec2 st;

uniform vec3 light; 
uniform vec3 eye;
uniform vec3 amb;
uniform vec3 diff;
uniform vec3 spec;
uniform float shi;
uniform sampler2D difftexture;

out vec3 color;
void main()
{
   vec3 wrl_vNormal = normalize(wrl_normal);
   vec3 wrl_vLight = normalize(light - wrl_position);
   vec3 wrl_vEye = normalize(eye - wrl_position);
   vec3 wrl_vReflection = normalize(reflect(-wrl_vLight, wrl_vNormal));
 
   // Diffuse
   vec3 cdiff = diff * max(dot(wrl_vNormal, wrl_vLight), 0.0f);
   cdiff = clamp(cdiff, 0.0f, 1.0f);
   
   // Specular
   vec3 cspec = spec * pow(max(dot(wrl_vReflection, wrl_vEye), 0.0f), shi);
   cspec = clamp(cspec, 0.0f, 1.0f);
   
   // Final Color
   color = amb + cdiff + cspec;
   color = texture2D(difftexture, st).rgb;
} 