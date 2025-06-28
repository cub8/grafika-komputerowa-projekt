#version 330 core
layout(location = 0) in vec2 quadPos;     
layout(location = 1) in vec3 worldPos;     
layout(location = 2) in float intensity;   

uniform mat4 view;
uniform mat4 projection;
uniform float baseSize;                    // np. quad size

out float fIntensity;

void main() {
    vec3 pos = worldPos;
    vec2 scaled = quadPos * baseSize;
    
    vec3 p = vec3(pos.x + scaled.x, 0.0, pos.z + scaled.y);
    gl_Position = projection * view * vec4(p, 1.0);
    fIntensity = intensity;
}
