#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D texture1;
uniform int isUseTexture;

void main()
{
	if(isUseTexture >= 1)
	{
		color=texture(texture1,texCoord);
	}
	else
	{
		color=vertexColor;
	}
}