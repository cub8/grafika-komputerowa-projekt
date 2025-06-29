#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D Tex;              // EU map
uniform sampler2D ContaminationTex; // contamination heatmap

void main()
{
    vec3 baseColor = texture(Tex, TexCoord).rgb;              
    float level = texture(ContaminationTex, TexCoord).a; 

    // color mix
    vec3 contaminated = mix(baseColor, vec3(0.5, 0.0, 0.0), clamp(level, 0.0, 1.0));

    FragColor = vec4(contaminated, 1.0);
}