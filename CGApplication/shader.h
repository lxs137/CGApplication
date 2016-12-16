#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "windowSetting.h"
using namespace std;

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
		{
			sourceCode=readGLSLFromMemory(filePath);
			return sourceCode;
		}
		sourceCode.assign(istreambuf_iterator<char>(infile),
			istreambuf_iterator<char>());
		return sourceCode;
	}
	string readGLSLFromMemory(string filePath)
	{
		HRSRC hRsrc = NULL;
		if (filePath == "2dModel.frag")
		{
			hRsrc = FindResource(NULL, MAKEINTRESOURCE(103), TEXT("GLSLText"));
		}
		else if (filePath == "2dModel.vert")
		{
			hRsrc = FindResource(NULL, MAKEINTRESOURCE(104), TEXT("GLSLText"));
		}
		else if (filePath == "3dModel.frag")
		{
			hRsrc = FindResource(NULL, MAKEINTRESOURCE(105), TEXT("GLSLText"));
		}
		else if (filePath == "3dModel.vert")
		{
			hRsrc = FindResource(NULL, MAKEINTRESOURCE(106), TEXT("GLSLText"));
		}
		 
		if (NULL == hRsrc)
			return NULL;
		DWORD dwSize = SizeofResource(NULL, hRsrc);
		if (0 == dwSize)
			return NULL;
		HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
		if (NULL == hGlobal)
			return NULL;
		LPVOID pBuffer = LockResource(hGlobal);
		if (NULL == pBuffer)
			return NULL;
		string codeStr = string((const char*)pBuffer);
		return codeStr;
	}

};
#endif // SHADER_H