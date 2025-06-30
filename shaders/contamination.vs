#version 330 core

layout (location = 0) in vec2 quadPos;      
layout (location = 1) in vec3 instancePos;  
layout (location = 2) in float intensity;    
layout (location = 3) in float scale;        

uniform mat4 view;      
uniform mat4 projection;

out float intens;

void main() {
    
    vec2 scaledQuad = quadPos * scale;

    vec3 worldPos = vec3(instancePos.x + scaledQuad.x, instancePos.z + scaledQuad.y, 0.0);

    gl_Position = projection * view * vec4(worldPos, 1.0);

    intens = intensity;

}
