#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 overrideColor1;

void main()
{    
    vec4 texColor = texture(texture_diffuse1, TexCoords);

    if (overrideColor1.r >= 0.f) {
        FragColor = mix(texColor, vec4(overrideColor1, 1.0), 0.5);
    } else {
        FragColor = texColor;
    }
}