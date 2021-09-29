#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

out vec3 theColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 out_position = projection * view * model * v;
	gl_Position = out_position;	
	theColor = vertexColor;
}