#version 450

// IO declarations
in vec3 vert;

uniform int permutations[512];

out vec3 color;

// Auxiliar Functions
float interpolate(float t)
{
  return t * t * t * (t * (t * 6 - 15) + 10);
}

float grad(int hash, float x, float y, float z)
{
  switch(hash & 0xF)
  {
      case 0x0: return  x + y;
      case 0x1: return -x + y;
      case 0x2: return  x - y;
      case 0x3: return -x - y;
      case 0x4: return  x + z;
      case 0x5: return -x + z;
      case 0x6: return  x - z;
      case 0x7: return -x - z;
      case 0x8: return  y + z;
      case 0x9: return -y + z;
      case 0xA: return  y - z;
      case 0xB: return -y - z;
      case 0xC: return  y + x;
      case 0xD: return -y + z;
      case 0xE: return  y - x;
      case 0xF: return -y - z;
      default: return 0;
  }
}

float lerp(float a, float b, float x)
{
  return a + x * (b - a);
}

float perlin(vec3 pos)
{
  int xi = int(pos.x);
  int yi = int(pos.y);
  int zi = int(pos.z);
  float xf = fract(pos.x);
  float yf = fract(pos.y);
  float zf = fract(pos.z);

  float u = interpolate(xf);
  float v = interpolate(yf);
  float w = interpolate(zf);

  int aaa, aba, aab, abb, baa, bba, bab, bbb;
  aaa = permutations[ permutations[ permutations[xi] + yi] + zi];
  aba = permutations[ permutations[ permutations[xi] + yi + 1] + zi];
  aab = permutations[ permutations[ permutations[xi] + yi] + zi + 1];
  abb = permutations[ permutations[ permutations[xi] + yi + 1] + zi + 1];
  baa = permutations[ permutations[ permutations[xi + 1] + yi ] + zi];
  bba = permutations[ permutations[ permutations[xi + 1] + yi+1] + zi];
  bab = permutations[ permutations[ permutations[xi + 1] + yi] + zi + 1];
  bbb = permutations[ permutations[ permutations[xi + 1] + yi + 1] + zi + 1];

  float x1, x2, y1, y2;
  x1 = lerp(grad(aaa, xf, yf, zf), grad (baa, xf-1, yf, zf), u);
  x2 = lerp(grad(aba, xf, yf-1, zf), grad(bba, xf-1, yf-1, zf), u);
  y1 = lerp(x1, x2, v);

  x1 = lerp(grad(aab, xf, yf, zf-1), grad(bab, xf-1, yf, zf-1), u);
  x2 = lerp(grad(abb, xf, yf-1, zf-1), grad(bbb, xf-1, yf-1, zf-1), u);
  y2 = lerp(x1, x2, v);
  
  float rand = lerp (y1, y2, w);
  return (rand * 0.5) + 0.5;
}

float noise(vec3 pos, float frequency, float amplitude)
{
  return perlin(pos * frequency) * amplitude;
}

// Main Function
void main()
{
  float r = noise(vert.xyy, 4, 128);
  float g = noise(vert.xyy, 8, 64);
  float b = noise(vert.xyy, 16, 32);
  float a = noise(vert.xyy, 32, 16);
  color = vec3(r + g + b + a) / 300;
  vec3 blue = abs(color - vec3(1)) * vec3(0,0,0.6);
  color += blue;
}