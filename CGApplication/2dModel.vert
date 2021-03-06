#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

out vec4 vertexColor;
out vec2 texCoord;

uniform mat4 transform;

void main()
{
	gl_Position=vec4(position,1.0f);
	vertexColor=vec4(color,1.0f);
	texCoord=vec2(position.x,position.y);
}