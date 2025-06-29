#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Tex;
uniform sampler2D ContaminationTex;

void main()
{
    vec4 mapColor = texture(Tex, TexCoord);
    vec4 contamination = texture(ContaminationTex, TexCoord);

    FragColor = mix(mapColor, contamination, contamination.a);
}
