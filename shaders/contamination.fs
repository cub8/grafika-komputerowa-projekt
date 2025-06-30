#version 330 core

in float intens;

out vec4 FragColor;

void main() {

    FragColor = vec4(0.0, 0.0 ,0.0 ,intens);
}
