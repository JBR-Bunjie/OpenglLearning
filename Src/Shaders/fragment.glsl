#version 330 core
in vec2 Texcoord;

out vec4 FragColor;

uniform sampler2D baseTex0;
uniform sampler2D baseTex1;
uniform float mixValue;

void main() {
    vec4 baseColor0 = texture(baseTex0, Texcoord);
    vec4 baseColor1 = texture(baseTex1, Texcoord);
    vec4 baseColor = mix(baseColor0, baseColor1, 0.2);
    FragColor = baseColor;
}
