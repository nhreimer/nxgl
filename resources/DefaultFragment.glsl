#version 330 core

// window-space will be the upper-left of the screen, rather than the usual lower-left.
layout(origin_upper_left) in vec4 gl_FragCoord;

in vec4 vertexShaderColor;  // pull from the previous shader in the pipeline
out vec4 fragmentShaderColor; // pass to the next shader in the pipeline

void main()
{
  fragmentShaderColor = vertexShaderColor;
}