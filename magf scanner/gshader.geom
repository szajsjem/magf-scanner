#version 460 core


layout(points) in;
layout(triangle_strip, max_vertices = 15) out;

layout(std140, binding = 0) uniform TriangulationTable {
  int table[256][16];
}
triangulation_ubo;


// Input from the Vertex Shader
in VStoGS {
  vec4 cornerValues1;
  vec4 cornerValues2;
  vec3 color;
}
gs_in[];

// Uniforms from your application
uniform mat4 MVP;       // Model-View-Projection matrix
const float isoLevel=0; // The threshold value for the surface
const float cubeSize=1; // The size of one cube

// Data sent to the Fragment Shader
out vec3 fragmentNormal;
out vec3 fragmentColor;
out vec3 fragmentPosition;

// --- Marching Cubes Lookup Tables ---

// This table maps an edge index to the two corner indices that it connects.
// There are 12 edges in a cube.

// Helper function to interpolate between two vertices to find the point where
// the surface crosses the edge.
vec3 interpolateVertex(vec3 p1, vec3 p2, float val1, float val2) {
  if (abs(isoLevel - val1) < 0.00001) {
    return p1;
  }
  if (abs(isoLevel - val2) < 0.00001) {
    return p2;
  }
  if (abs(val1 - val2) < 0.00001) {
    return p1;
  }
  float t = (isoLevel - val1) / (val2 - val1);
  return mix(p1, p2, t);
}

void main() {
  // The base position of the cube (corner 0)
  vec3 cubeOrigin = gl_in[0].gl_Position.xyz;

  fragmentColor=gs_in[0].color;

  // Unpack the 8 scalar values for the corners
  float cornerValues[8];
  cornerValues[0] = gs_in[0].cornerValues1.x;
  cornerValues[1] = gs_in[0].cornerValues1.y;
  cornerValues[2] = gs_in[0].cornerValues1.z;
  cornerValues[3] = gs_in[0].cornerValues1.w;
  cornerValues[4] = gs_in[0].cornerValues2.x;
  cornerValues[5] = gs_in[0].cornerValues2.y;
  cornerValues[6] = gs_in[0].cornerValues2.z;
  cornerValues[7] = gs_in[0].cornerValues2.w;

  // These are the offsets from the cube origin to each of its 8 corners.
  // The order must match the order of the cornerValues.
  // This is the Z-major order used in your example.
  const vec3 cornerOffsets[8] = vec3[8](
    vec3(0.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 1.0),
    vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 0.0),
    vec3(1.0, 1.0, 1.0), vec3(0.0, 1.0, 1.0)
  );

  // Calculate the cube index: each bit corresponds to a corner.
  // If corner value < isoLevel, the bit is 1, otherwise 0.
  int cubeIndex = 0;
  if (cornerValues[0] < isoLevel)
    cubeIndex |= 1;
  if (cornerValues[1] < isoLevel)
    cubeIndex |= 2;
  if (cornerValues[2] < isoLevel)
    cubeIndex |= 4;
  if (cornerValues[3] < isoLevel)
    cubeIndex |= 8;
  if (cornerValues[4] < isoLevel)
    cubeIndex |= 16;
  if (cornerValues[5] < isoLevel)
    cubeIndex |= 32;
  if (cornerValues[6] < isoLevel)
    cubeIndex |= 64;
  if (cornerValues[7] < isoLevel)
    cubeIndex |= 128;

  // If the cube is entirely inside or outside the surface, there are no
  // triangles to generate.
  if (cubeIndex == 0 || cubeIndex == 255) {
    return;
  }

  // This array will hold the calculated positions of the vertices on the edges.
  vec3 edgeVertices[12];

  // Find the vertices for the edges that are intersected by the surface.
  for (int i = 0; i < 12; i++) {
    // Get the indices of the two corners for this edge
    ivec2 cornerIndices = edgeConnections[i];
    int c1_idx = cornerIndices.x;
    int c2_idx = cornerIndices.y;

    // Get their scalar values
    float val1 = cornerValues[c1_idx];
    float val2 = cornerValues[c2_idx];

    // Get their positions in model space
    vec3 p1 = cubeOrigin + cornerOffsets[c1_idx] * cubeSize;
    vec3 p2 = cubeOrigin + cornerOffsets[c2_idx] * cubeSize;

    // Interpolate to find the vertex position on the edge
    edgeVertices[i] = interpolateVertex(p1, p2, val1, val2);
  }

  // Generate the triangles for this cube configuration.
  for (int i = 0; triangulation_ubo.table[cubeIndex][i] != -1; i += 3) {
    // Get the three vertices for this triangle from the edge vertex list
    vec3 v0 = edgeVertices[triangulation_ubo.table[cubeIndex][i]];
    vec3 v1 = edgeVertices[triangulation_ubo.table[cubeIndex][i + 1]];
    vec3 v2 = edgeVertices[triangulation_ubo.table[cubeIndex][i + 2]];

    // Calculate the normal of the triangle for lighting
    vec3 normal = normalize(cross(v1 - v0, v2 - v0));

    // --- Emit the first vertex ---
    fragmentPosition = v0;
    fragmentNormal = normal;
    gl_Position = MVP * vec4(v0, 1.0);
    EmitVertex();

    // --- Emit the second vertex ---
    fragmentPosition = v1;
    fragmentNormal = normal;
    gl_Position = MVP * vec4(v1, 1.0);
    EmitVertex();

    // --- Emit the third vertex ---
    fragmentPosition = v2;
    fragmentNormal = normal;
    gl_Position = MVP * vec4(v2, 1.0);
    EmitVertex();

    // Finish the triangle primitive
    EndPrimitive();
  }
}