#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define GLEW_STATIC
#include <gl/glew.h>

class shader
{
public:
	GLuint shaderProgram;

	shader(char * vertexSourcePath, char *fragmentSourcePath)
	{
		initVertexShader(vertexSourcePath);
		initFragmentShader(fragmentSourcePath);
		initShaderProgram();
	}
private:
	GLuint vertexShader;
	GLuint fragmentShader;
	void initVertexShader(char *vertexSourcePath)
	{
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		string sourceCodeStr = readFile(vertexSourcePath);
		const char *sourceCode = sourceCodeStr.c_str();
		glShaderSource(this->vertexShader, 1, &sourceCode, NULL);
		glCompileShader(this->vertexShader);
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(this->vertexShader, 512, NULL, infoLog);
		}
	}
	void initFragmentShader(char *fragmentSourcePath)
	{
		this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		string sourceCodeStr = readFile(fragmentSourcePath);
		const char* sourceCode = sourceCodeStr.c_str();
		glShaderSource(this->fragmentShader, 1, &sourceCode, NULL);
		glCompileShader(this->fragmentShader);
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(this->fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(this->fragmentShader, 512, NULL, infoLog);
		}
	}
	void initShaderProgram()
	{
		this->shaderProgram = glCreateProgram();
		glAttachShader(this->shaderProgram, this->vertexShader);
		glAttachShader(this->shaderProgram, this->fragmentShader);
		glLinkProgram(this->shaderProgram);
		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->shaderProgram, 512, NULL, infoLog);
		}
	}
	string readFile(char *filePath)
	{
		ifstream infile;
		string sourceCode;
		infile.open(filePath);
		if (!infile)
			cout << filePath << " open error" << endl;
		sourceCode.assign(istreambuf_iterator<char>(infile),
			istreambuf_iterator<char>());
		return sourceCode;
	}

};
#endif // SHADER_H