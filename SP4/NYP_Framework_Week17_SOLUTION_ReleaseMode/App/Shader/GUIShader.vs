#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 Color;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	Color = aColor;
	TexCoord = aTexCoord;
}