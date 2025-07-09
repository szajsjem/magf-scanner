#version 430 core

// The position of the corner (0,0,0) of the cube in model space.
layout(location = 0) in vec3 vertexPosition_modelspace;

layout(location = 1) in vec3 color;
// The 8 scalar values for the cube's corners.
// We use two vec4s as it's a common and compatible way to pass 8 floats.
// cornerValues1.xyzw corresponds to corners 0, 1, 2, 3
layout(location = 2) in vec4 cornerValues1;
// cornerValues2.xyzw corresponds to corners 4, 5, 6, 7
layout(location = 3) in vec4 cornerValues2;

// Data to be passed to the geometry shader
out VStoGS {
  vec4 cornerValues1;
  vec4 cornerValues2;
  vec3 color;
}
vs_out;

void main() {
  // The gl_Position is the cube's origin. The geometry shader will use this
  // as a base to construct the triangles.
  gl_Position = vec4(vertexPosition_modelspace, 1.0);

  // Pass the corner values through to the geometry shader.
  vs_out.cornerValues1 = cornerValues1;
  vs_out.cornerValues2 = cornerValues2;
  vs_out.color=color;
}