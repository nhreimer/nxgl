#version 330 core

layout( points ) in;
layout( line_strip, max_vertices = 11 ) out;

in vec4 vertexShaderColor[];  // color passed from the vertex shader
out vec4 geometryShaderColor;

in uint vertexEdges[];       // number of edges passed from the vertex shader

const float TAU = 6.2831853;

void main()
{
  geometryShaderColor = vertexShaderColor[ 0 ];

  for (int i = 0; i <= 10; i++)
  {
    // Angle between each side in radians
    float ang = TAU / 10.0 * i;

    vec4 offset = vec4( cos( ang ) * 0.3, -sin( ang ) * 0.4, 0.0, 0.0 );
    gl_Position = gl_in[ 0 ].gl_Position + offset;

    EmitVertex();
  }

  EndPrimitive();
}