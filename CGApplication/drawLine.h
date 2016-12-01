#ifndef DRAWLINE_H
#define DRAWLINE_H

#include <iostream>
#include "textureManager.h"
#include "windowSetting.h"

#include <glm\glm.hpp>

namespace drawLine{
	TextureManager *myTextureManager;
	GLuint myShaderProgram;
	GLuint myVAO;
	GLuint myVBO;
	GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	GLboolean drawing=GL_FALSE;
}
void lineRender2DSence();
void lineOnMouseClick(int button, int state, int x, int y);
void lineOnActiveMotion(int x, int y);
void lineOnReshape(int width, int height);
void lineInitGlutWindow();
void lineLoadImageAsTexture(unsigned int testTextureID, const char *filename);
void lineInitVAO(GLuint &VAO,GLuint &VBO);
glm::mat4 lineGetTransformMatrix();


#endif


