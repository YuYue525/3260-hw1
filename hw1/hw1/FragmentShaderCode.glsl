#version 330 core

out vec4 Color;
in vec3 theColor;

void main()
{
	Color = vec4(theColor, 1.0);
}
