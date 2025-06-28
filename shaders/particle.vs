#version 330 core

layout (location = 0) in vec2 quadPos;       
layout (location = 1) in vec3 instancePos;   
layout (location = 2) in float intensity;    

out float fIntensity;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform float scale = 1.0;

void main() {
    // billboard: wyciągamy kierunki kamery (osie)
    vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp    = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 worldPos = instancePos
                  + cameraRight * quadPos.x * scale
                  + cameraUp    * quadPos.y * scale;

    gl_Position = projection * view * vec4(worldPos, 1.0);
    fIntensity = intensity;

    texCoord = quadPos + vec2(0.5);
}
