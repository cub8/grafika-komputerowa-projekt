#version 330 core
in float fIntensity;
out vec4 FragColor;

// gradient
vec3 getColor(float t) {
    if (t < 0.5)
        return mix(vec3(1.0,1.0,0.0), vec3(1.0,0.5,0.0), t*2.0); // yellow → orange
    else
        return mix(vec3(1.0,0.5,0.0), vec3(1.0,0.0,0.0), (t-0.5)*2.0); // orange → red
}

void main() {
    vec3 col = getColor(clamp(fIntensity,0.0,1.0));

    FragColor = vec4(col, fIntensity * 0.5);
}
