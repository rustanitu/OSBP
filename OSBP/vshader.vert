layout(location = 0) in vec4 myVertex;
layout(location = 1) in vec4 color;

out vec4 fcolor;
void main (void)
{
  fcolor = color;
  gl_Position = myVertex;
}