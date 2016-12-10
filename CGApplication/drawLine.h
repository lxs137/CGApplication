#ifndef DRAWLINE_H
#define DRAWLINE_H

#include <iostream>
#include "textureManager.h"
#include "windowSetting.h"

#include <glm\glm.hpp>

namespace drawLine{
	GLuint myShaderProgram;
	GLuint myVAO;
	GLuint myVBO;
	GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	GLboolean drawing=GL_FALSE;
	GLboolean cliping=0;
	glm::ivec3 rotateCenter = glm::ivec3(0, 0, 0);
    std::vector<glm::ivec3> transBasisPoint;
	struct ClipWindow
	{
		glm::ivec3 clipWindowCenter;
		GLint windowHeightHalf;
		GLint windowWidthHalf;
	}myClipWindow;
	enum MenuOptions
	{
		EDIT,
		MOVE,
		ROTATE,
		ZOOM,
		CLIP,
		SAVEFILE,
		OPENFILE,
		EXIT
	};
	MenuOptions transformStatus=EDIT;
}
void lineRender2DSence();
void lineOnMouseClick(int button, int state, int x, int y);
void lineOnActiveMotion(int x, int y);
void lineOnMouseWheelScrollValid(int wheel, int direction, int x, int y);
void lineOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y);
void lineProcessMenuEvent(int options);
void lineOnReshape(int width, int height);
void lineInitGlutWindow();
void lineLoadImageAsTexture(unsigned int testTextureID, const char *filename);
void lineInitVAO(GLuint &VAO,GLuint &VBO);
void lineInitMenus();
void lineGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo = glm::ivec2(0, 0));
void lineSetTransBasisPoint();
#endif


