#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

out vec3 theColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 out_position = projection * view * model * transform * v;
	gl_Position = out_position;	
	theColor = vertexColor;
}
