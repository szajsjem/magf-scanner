#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out vec3 fragmentNormal;
out vec3 fragmentColor;

uniform mat4 MVP;
uniform mat4 V;
uniform float jitterAmount = 0.05; // Adjust as needed

// Hash function to generate a pseudo-random value from a vec3
float hash(vec3 p) {
    return fract(sin(dot(p ,vec3(12.9898,78.233, 37.719))) * 43758.5453);
}

vec3 jitter(vec3 pos) {
    return vec3(
        hash(pos + vec3(1.0, 0.0, 0.0)),
        hash(pos + vec3(0.0, 1.0, 0.0)),
        hash(pos + vec3(0.0, 0.0, 1.0))
    ) * 2.0 - 1.0; // Range [-1, 1]
}

void main() {
    vec3 offset = jitter(position) * jitterAmount;
    vec3 perturbedPosition = position + offset;
  gl_Position = MVP * vec4(perturbedPosition, 1.0);
  fragmentNormal =  normal;
  fragmentColor=color;
}