#ifndef DRAWELLIPSE_H
#define DRAWELLIPSE_H
#include <iostream>
#include <vector>
#include "textureManager.h"
#include "windowSetting.h"

#include <glm\glm.hpp>

namespace drawEllipse{
	TextureManager *myTextureManager;
	GLuint myShaderProgram;
	GLuint myVAO;
	GLuint myVBO;
	GLboolean drawing = GL_FALSE;
	std::vector<glm::ivec3> controlPoints;
};
void ellipseRender2DSence();
void ellipseOnMouseClick(int button, int state, int x, int y);
void ellipseOnActiveMotion(int x, int y);
void ellipseOnReshape(int width, int height);
void ellipseInitGlutWindow();
void ellipseLoadImageAsTexture(unsigned int testTextureID, const char *filename);
void ellipseInitVAO(GLuint &VAO,GLuint &VBO);
glm::mat4 ellipseGetTransformMatrix();
#endif // !DRAWELLIPSE_H
