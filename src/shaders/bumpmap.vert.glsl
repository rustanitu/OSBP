#version 450
 
in vec3 vertex;
in vec3 normal;
in vec2 texcoord;
in vec3 tan;
in vec3 bitan;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 tinv_model;

out vec3 wrl_normal;
out vec3 wrl_position;
out vec2 st;
out vec3 tangent;
out vec3 bitangent;
void main()
{
  gl_Position = mvp * vec4(vertex, 1.0f);
  wrl_position = mat3(model) * vertex;
  wrl_normal = mat3(tinv_model) * normal;
  st = texcoord;
  tangent = mat3(tinv_model) * normalize(tan);
  bitangent = mat3(tinv_model) * normalize(bitan);
} 