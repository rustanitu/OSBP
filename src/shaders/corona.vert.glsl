#version 450

in vec3 vertex;

uniform mat4 turb_rot;
uniform mat4 mvp;

out vec3 texpos;
out vec3 vert;
void main()
{
  vert = vertex;
  texpos = (turb_rot * vec4(vertex, 1)).xyz;
  texpos = texpos * 0.25;
  texpos += vec3(0.5, 0.5, 0);
  gl_Position = mvp * vec4(vertex, 1.0f);
}