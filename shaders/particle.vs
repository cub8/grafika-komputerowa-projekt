#version 330 core

layout (location = 0) in vec2 quadPos;       
layout (location = 1) in vec3 instancePos;   
layout (location = 2) in float intensity;    
layout (location = 3) in float scale;   

out float fIntensity;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;

void main() {

    // billboard
    vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp    = vec3(view[0][1], view[1][1], view[2][1]);

    vec2 scaledQuad = quadPos * scale;

    vec3 worldPos = instancePos
                  + cameraRight * scaledQuad.x
                  + cameraUp    * scaledQuad.y;

    gl_Position = projection * view * vec4(worldPos, 1.0);
    fIntensity = intensity;

    texCoord = quadPos + vec2(0.5);
}
