#version 330 core

in vec4 vertexColor;
in vec2 texCoord;
out vec4 color;
uniform sampler2D texture1;

void main()
{
	color=texture(texture1,texCoord);

}