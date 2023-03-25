#version 330 core

in vec4 geometryShaderColor;  // pull from the previous shader in the pipeline
out vec4 fragmentShaderColor; // pass to the next shader in the pipeline

void main()
{
  fragmentShaderColor = geometryShaderColor;
}