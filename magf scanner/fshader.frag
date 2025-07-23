#version 430 core

// Input from the Geometry Shader
in vec3 fragmentNormal;
in vec3 fragmentPosition;
in vec3 fragmentColor;

// The final color output
out vec3 color;

// Uniforms for lighting
uniform vec3 lightPos = vec3(10.0, 60.0, 10.0);
uniform vec3 viewPos; // Camera position

void main() {
  // Ambient lighting
  float ambientStrength = 0.2;
  vec3 ambient = ambientStrength * vec3(1.0);

  // Diffuse lighting
  vec3 norm = normalize(fragmentNormal);
  vec3 lightDir = normalize(lightPos - fragmentPosition);
  float diff = abs(dot(norm, lightDir));
  vec3 diffuse = diff * vec3(1.0);

  // Specular lighting
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - fragmentPosition);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(abs(dot(viewDir, reflectDir)), 32);
  vec3 specular = specularStrength * spec * vec3(1.0);

  vec3 result = (ambient + diffuse + specular) * fragmentColor;
  color = result;
}