#include "PerlinNoise.h"

const int PerlinNoise::size = 512;
const int PerlinNoise::permutations[512] = 
{
  151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53,
  194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37,
  240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26,
  197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
  88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68,
  175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158,
  231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92,
  41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
  1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200,
  196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198,
  173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202,
  38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227,
  47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213,
  119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155,
  167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79,
  113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
  251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
  81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31,
  181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45,
  127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29,
  24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
  151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53,
  194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37,
  240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26,
  197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
  88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68,
  175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158,
  231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92,
  41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
  1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200,
  196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198,
  173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202,
  38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227,
  47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213,
  119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155,
  167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79,
  113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
  251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
  81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31,
  181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45,
  127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29,
  24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

float* PerlinNoise::Generate3DTexture(int dimension)
{
  int size = dimension * dimension * dimension * 4;
  float* noise = new float[size];

  float dim = float(dimension);

  int count = 0;
  for (int k = 0; k < dimension; ++k)
  {
    float z = k / dim;
    for (int j = 0; j < dimension; ++j)
    {
      float y = j / dim;
      for (int i = 0; i < dimension; ++i)
      {
        float x = i / dim;
        glm::vec3 pos(x, y, z);
        float r = Noise(pos, 4, 128);
        float g = Noise(pos, 8, 64);
        float b = Noise(pos, 16, 32);
        float a = Noise(pos, 32, 16);

        noise[count++] = r;
        noise[count++] = g;
        noise[count++] = b;
        noise[count++] = a;
      }
    }
  }

  return noise;
}

float* PerlinNoise::Generate2DTexture(int dimension)
{
  int size = dimension * dimension * 4;
  float* noise = new float[size];

  float dim = float(dimension);

  int count = 0;
  for (int j = 0; j < dimension; ++j) {
    float y = j / dim;
    for (int i = 0; i < dimension; ++i) {
      float x = i / dim;
      glm::vec3 pos(x, y, 0);
      float r = Noise(pos, 4, 128);
      float g = Noise(pos, 8, 64);
      float b = Noise(pos, 224, 2);
      float a = Noise(pos, 255, 2);

      noise[count++] = r;
      noise[count++] = g;
      noise[count++] = b;
      noise[count++] = a;
    }
  }

  return noise;
}

float PerlinNoise::Fade(float t)
{
  return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::Grad(int hash, float x, float y, float z)
{
  switch (hash & 0xF) {
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

float PerlinNoise::Lerp(float a, float b, float x)
{
  return a + x * (b - a);
}

float PerlinNoise::Perlin(glm::vec3 pos)
{
  int xi = int(pos.x);
  int yi = int(pos.y);
  int zi = int(pos.z);
  float xf = pos.x - xi;
  float yf = pos.y - yi;
  float zf = pos.z - zi;

  float u = Fade(xf);
  float v = Fade(yf);
  float w = Fade(zf);

  int aaa, aba, aab, abb, baa, bba, bab, bbb;
  aaa = permutations[permutations[permutations[xi] + yi] + zi];
  aba = permutations[permutations[permutations[xi] + yi + 1] + zi];
  aab = permutations[permutations[permutations[xi] + yi] + zi + 1];
  abb = permutations[permutations[permutations[xi] + yi + 1] + zi + 1];
  baa = permutations[permutations[permutations[xi + 1] + yi] + zi];
  bba = permutations[permutations[permutations[xi + 1] + yi + 1] + zi];
  bab = permutations[permutations[permutations[xi + 1] + yi] + zi + 1];
  bbb = permutations[permutations[permutations[xi + 1] + yi + 1] + zi + 1];

  float x1, x2, y1, y2;
  x1 = Lerp(Grad(aaa, xf, yf, zf), Grad(baa, xf - 1, yf, zf), u);
  x2 = Lerp(Grad(aba, xf, yf - 1, zf), Grad(bba, xf - 1, yf - 1, zf), u);
  y1 = Lerp(x1, x2, v);

  x1 = Lerp(Grad(aab, xf, yf, zf - 1), Grad(bab, xf - 1, yf, zf - 1), u);
  x2 = Lerp(Grad(abb, xf, yf - 1, zf - 1), Grad(bbb, xf - 1, yf - 1, zf - 1), u);
  y2 = Lerp(x1, x2, v);

  float rand = Lerp(y1, y2, w);
  return rand;// (rand * 0.5) + 0.5;
}

float PerlinNoise::Noise(glm::vec3 pos, float frequency, float amplitude)
{
  return Perlin(pos * frequency) * amplitude;
}