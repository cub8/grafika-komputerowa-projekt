#version 330 core

in vec2 fragUV;

uniform float intensity;  // from particle
uniform float scale;      // from particle

out vec4 FragColor;

void main() {
    float dist = length(fragUV - vec2(0.5));  // center of quad
    float alpha = exp(-dist * 8.0) * intensity;  

    // red 
    FragColor = vec4(1.0, 0.0, 0.0, 0.5);
}
