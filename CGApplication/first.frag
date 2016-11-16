#version 330 core

in vec4 vertexColor;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	color=vertexColor;
}