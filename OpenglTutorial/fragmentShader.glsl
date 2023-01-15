#version 330 core

in vec2 TexCoord;
//in vec3 colorPer;

out vec4 color;

//uniform vec4 ourColor;
uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

// control variables
uniform float texture_mix_control;


void main()
{
    //color = ourColor;
    //color = vec4(colorPer.rgb, 1.0f);
    color = mix(texture(ourTexture0, TexCoord), texture(ourTexture1, vec2(TexCoord.x, 1.0f - TexCoord.y)), texture_mix_control);
}