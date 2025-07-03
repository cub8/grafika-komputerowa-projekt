#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D Tex;
uniform sampler2D ContaminationTex;

void main()
{
    vec3 baseColor = texture(Tex, TexCoord).rgb;              
    float intensity = texture(ContaminationTex, TexCoord).a; 

    vec3 colorLow = vec3(1.0, 1.0, 0.0);
    vec3 colorMid = vec3(1.0, 0.5, 0.0);
    vec3 colorHigh = vec3(1.0, 0.0, 0.0);
    vec3 contaminationColor;

    if (intensity > 0.7) {
        contaminationColor = colorHigh;
    }
    else if (intensity > 0.2) {
        contaminationColor = colorMid;
    } else if (intensity > 0.001 ) {
        contaminationColor = colorLow;
    }
    else {
        contaminationColor = baseColor;
    }

    vec3 contaminated = mix(baseColor, contaminationColor, 0.5);

    FragColor = vec4(contaminated, 1.0);
}