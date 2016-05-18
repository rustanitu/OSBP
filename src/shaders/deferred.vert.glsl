#version 450
 
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

out vec3 gPosition;
out vec3 gNormal;
out vec4 gAlbedoSpec;

void main()
{    
  // Store the fragment position vector in the first gbuffer texture
  gPosition = FragPos;
  
  // Also store the per-fragment normals into the gbuffer
  gNormal = normalize(Normal);
  
  // And the diffuse per-fragment color
  gAlbedoSpec.rgb = texture(texture_diffuse, TexCoords).rgb;
  
  // Store specular intensity in gAlbedoSpec's alpha component
  gAlbedoSpec.a = texture(texture_specular, TexCoords).r;
}  