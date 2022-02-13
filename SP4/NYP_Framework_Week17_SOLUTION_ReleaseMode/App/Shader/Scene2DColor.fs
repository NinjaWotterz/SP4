#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;

// texture samplers
uniform sampler2D texture1;
uniform vec4 runtime_color;

void main()
{
	FragColor = texture(texture1, TexCoord);
	FragColor *= Color;
	FragColor *= runtime_color;
}