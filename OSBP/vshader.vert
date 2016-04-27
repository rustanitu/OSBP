#version 450
 
in vec4 myVertex;
in vec4 color;
uniform mat4 mvp;

out vec3 normal;
out vec3 light; 
out vec3 view;
void main (void)
{
  normal = (mvp * myVertex).xyz;
  light = (mvp * vec4(0.0, 2.0, 0.0, 1)).xyz;
  view = (mvp * vec4(0.0, 0.0, 2.0, 1)).xyz;
  
  gl_Position = mvp * myVertex;
}