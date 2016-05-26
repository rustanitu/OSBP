#version 450

in vec2 st;

uniform sampler2D tex;
uniform sampler2D tex2;

out vec3 color;
void main()
{
  color = texture2D(tex, st).xyz;// * texture2D(tex2, st).xyz;
}