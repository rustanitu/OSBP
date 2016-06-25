#version 450

in vec2 st;

out vec3 color;
void main()
{
  float sawtooth = fract(st.x * 16);
  float triangle = abs(2.0 * sawtooth - 1.0);
  float s = smoothstep(0.2, 0.5, triangle);
  float width = length(vec2 (dFdx(st.x), dFdy(st.x)));
  float edge = width * 16 * 2.0; // 2-pixel large
  float square = smoothstep(0.5 - edge, 0.5 + edge, triangle);
  color = vec3 (square);
}