#version 330 core

in vec4 vertexShaderColor;    // pull from the previous shader in the pipeline
out vec4 fragmentShaderColor; // pass to the next shader in the pipeline

void main()
{
  //color = vec4( 1.f, 0.f, 0.f, 1.f );
  fragmentShaderColor = vertexShaderColor;
}