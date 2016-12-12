#ifndef ENTERMENU_H
#define ENTERMENU_H
#include <vector>
#include "windowSetting.h"
#include <glm/glm.hpp>
namespace EnterMenu{
	std::vector<std::vector<glm::ivec3>> buttonRect={
		{ glm::ivec3(-335,150,0),glm::ivec3(-210,150,0),glm::ivec3(-210,50,0),glm::ivec3(-335,50,0)},  //line button
		{ glm::ivec3(-160,150,0),glm::ivec3(-35,150,0),glm::ivec3(-35,50,0),glm::ivec3(-160,50,0)},    //circle button
		{ glm::ivec3(25,150,0), glm::ivec3(150,150,0), glm::ivec3(150,50,0),glm::ivec3(25,50,0)},      //ellipse button
		{ glm::ivec3(200,150,0), glm::ivec3(325,150,0), glm::ivec3(325,50,0),glm::ivec3(200,50,0) },   //bezier button
		{ glm::ivec3(-335, -50, 0), glm::ivec3(-210, -50, 0), glm::ivec3(-210, -150, 0), glm::ivec3(-335, -150, 0) },  //spline button
		{ glm::ivec3(-160, -50, 0), glm::ivec3(-35, -50, 0), glm::ivec3(-35, -150, 0), glm::ivec3(-160, -150, 0) },     //polygon button
		{ glm::ivec3(25, -50, 0), glm::ivec3(150, -50, 0), glm::ivec3(150, -150, 0), glm::ivec3(25, -150, 0) },        //3d button
		{ glm::ivec3(200, -50, 0), glm::ivec3(325, -50, 0), glm::ivec3(325, -150, 0), glm::ivec3(200, -150, 0) }       //exit button
	};
	glm::vec3 selectedColor = glm::vec3(0.0f, 0.50f, 0.55f), unSelectedColor = glm::vec3(0.0f, 0.737f, 0.831f);
	GLuint myShaderProgram;
	std::vector<GLuint> myVAOs;
	std::vector<GLuint> myVBOs;
	GLint framePixelsNum;
	GLboolean init = GL_FALSE; // «∑Ò≥ı ºªØ
	DRAWINGMODEL selectedButton = DRAWENTERMENU;
	GLint windowIdentify = 0;
	struct Button 
	{
		std::vector<GLfloat> buttonPixels;
		GLint pixelsNum;
	};
	std::vector<const unsigned char*> buttonText = {
		(unsigned char*)"Draw \n Line",
		(unsigned char*)" Draw \n Circle",
		(unsigned char*)" Draw \n Ellipse",
		(unsigned char*)" Draw \n Bezier",
		(unsigned char*)"    Draw \n B-Spline",
		(unsigned char*)"   Draw \n Polygon",
		(unsigned char*)"  3D \n View",
		(unsigned char*)"     Exit \n Application"
	};
};
void entranceRender2DSence();
void entranceOnMouseClick(int button, int state, int x, int y);
void entranceOnMouseMove(int x, int y);
GLint entranceInitGlutWindow();
EnterMenu::Button entranceInitButton(std::vector<glm::ivec3> vertics,glm::vec3 fillColor);
#endif