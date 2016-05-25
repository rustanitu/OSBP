﻿#version 450

in vec3 vertex;

uniform mat4 mvp;

void main()
{
  gl_Position = mvp * vec4(vertex, 1.0f);
} 