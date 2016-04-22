#version 450
 
in vec4 myVertex;
in vec4 color;
uniform mat4 mvp;

out vec4 fcolor;
void main (void)
{
  fcolor = color;
  gl_Position = mvp * myVertex;
}