#version 450
 
in vec3 vertex;
in vec3 normal;
in vec2 texcoord;
in vec3 tan;
in vec3 bitan;

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 _m;

out vec3 wrl_normal;
out vec3 wrl_position;
out vec2 st;
out vec3 tangent;
out vec3 bitangent;
void main()
{
  gl_Position = mvp * vec4(vertex, 1.0f);
  wrl_position = mat3(m) * vertex;
  wrl_normal = mat3(_m) * normal;
  st = texcoord;
  tangent = mat3(_m) * normalize(tan);
  bitangent = mat3(_m) * normalize(bitan);
} 