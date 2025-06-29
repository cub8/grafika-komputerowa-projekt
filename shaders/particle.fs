#version 330 core

in float fIntensity;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D particleTexture;

void main() {

    vec4 tex = texture(particleTexture, texCoord);
    vec3 color = mix(vec3(0.2), vec3(1.0, 0.0, 0.0), fIntensity);  // from gray to red
    float alpha = tex.a * fIntensity;

    FragColor = vec4(color, alpha);

}
