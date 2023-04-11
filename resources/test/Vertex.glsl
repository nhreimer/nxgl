#version 330 core

// pull in the data from the vertex attribute
layout (location = 0) in vec2 attribPos;   // the position variable has attribute position 0
layout (location = 1) in vec4 attribColor; // the color variable has attribute position 1

// create an output variable that will get passed to the next shader
out vec4 vertexShaderColor;

uniform mat4 uMVP; // MVP performed on the CPU side

void main()
{
  gl_Position = uMVP * vec4( attribPos, 0.0, 1.0 );
  vertexShaderColor = attribColor; // copy the data for the fragment shader
}
