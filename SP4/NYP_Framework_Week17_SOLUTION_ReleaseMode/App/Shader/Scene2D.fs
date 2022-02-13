#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;

// texture samplers
uniform sampler2D texture1;


void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

	FragColor = texture(texture1, TexCoord);
	FragColor *= Color;
}