#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Tex;
uniform sampler2D ContaminationTex;

void main()
{
    FragColor = texture(Tex, TexCoord);
}
