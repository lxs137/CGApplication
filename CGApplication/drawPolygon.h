#ifndef DRAWPOLYGON_H
#define DRAWPOLYGON_H

#include <iostream>
#include <vector>
#include "textureManager.h"
#include "windowSetting.h"

#include <glm\glm.hpp>

namespace drawPolygon
{
	TextureManager *myTextureManager;
	GLuint myShaderProgram;
	GLuint myVAO;
	GLuint myVBO;
	GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	GLint drawStatus = 0;//0代表未处于绘制状态；1代表正在初始化顶点；2表示正在修改顶点
	GLint filling = 0;//0代表未填充，1代表用颜色填充，2代表用图像填充
	std::vector<glm::ivec3> vertexPoints;//多边形的各个顶点
	std::vector<glm::ivec3> transBasisPoint;
	glm::ivec3 rotateCenter = glm::ivec3(0, 0, 0);
	GLint drawingPointIndex = 1;//多边形的各个顶点（从1开始）,表示正在初始化或者修改的顶点
	unsigned int textureID = 2;
	enum MenuOptions
	{
		EDIT,
		MOVE,
		ROTATE,
		ZOOM,
		FILLCOLOR,
		FILLPICTURE,
		EXIT
	};
	MenuOptions transformStatus = EDIT;
}
void polygonRender2DSence();
void polygonOnMouseClick(int button, int state, int x, int y);
void polygonOnActiveMotion(int x, int y);
void polygonOnMouseWheelScrollValid(int wheel, int direction, int x, int y);
void polygonOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y);
void polygonProcessMenuEvent(int options);
void polygonOnReshape(int width, int height);
void polygonInitGlutWindow();
void polygonInitMenus();
void polygonInitVAO(GLuint &VAO, GLuint &VBO);
void polygonGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo = glm::ivec2(0, 0));
void polygonSetTransBasisPoint();

#endif // !DRAWPOLYGON_H
