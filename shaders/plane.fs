#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D Tex;              // EU map
uniform sampler2D ContaminationTex; // contamination heatmap

void main()
{
    vec3 baseColor = texture(Tex, TexCoord).rgb;              
    float intensity = texture(ContaminationTex, TexCoord).a; 

    // Map intensity to color
    vec3 colorLow = vec3(1.0, 1.0, 0.0);   // yellow
    vec3 colorMid = vec3(1.0, 0.5, 0.0);   // orange
    vec3 colorHigh = vec3(1.0, 0.0, 0.0); // red
    vec3 contaminationColor;

    if (intensity > 0.7) {
        contaminationColor = colorHigh;
    }
    else if (intensity > 0.2) {
        contaminationColor = colorMid;
    } else if (intensity > 0.1 ) {
        contaminationColor = colorLow;
    }
    else {
        contaminationColor = baseColor;
    }

    
    // color mix
    vec3 contaminated = mix(baseColor, contaminationColor, 0.5);

    FragColor = vec4(contaminated, 1.0);
}