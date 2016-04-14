in vec4 myVertex;
in vec4 color;

out vec4 fcolor;
void main (void)
{
  fcolor = color;
  gl_Position = myVertex;
}