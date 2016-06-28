#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <glm\vec3.hpp>

class PerlinNoise
{
public:
  static float* Generate3DTexture(int dimension);
  static float* Generate2DTexture(int dimension);

private:
  static float Fade(float t);

  static float Grad(int hash, float x, float y, float z);

  static float Lerp(float a, float b, float x);

  static float Perlin(glm::vec3 pos);

  static float Noise(glm::vec3 pos, float frequency, float amplitude);

private:
  static const int size;
  static const int permutations[512];
};

#endif