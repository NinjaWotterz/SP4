#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
     vec4 texel = texture(texture_diffuse1, TexCoords);
    if(texel.a < 0.5)
        discard;
    FragColor = texel;

}