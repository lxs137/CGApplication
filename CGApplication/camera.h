#ifndef CAMREA_H
#define CAMERA_H

#include "windowSetting.h"
#define GLEW_STATIC
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

enum CameraMovementStatus
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW = -90.0f;//Æ«º½½Ç
const GLfloat PITCH = 0.0f;//¸©Ñö½Ç
const GLfloat SPEED = 0.005f;
const GLfloat SENSITIVITY = 0.15f;
const GLfloat ZOOM = glm::radians(45.0f);//Í¶Ó°¾ØÕóÖÐµÄFOV
const GLfloat ZOOM_MAX = glm::radians(45.0f);
const GLfloat ZOOM_MIN = glm::radians(1.0f);

class Camera
{
private:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;
	void updateCameraVector()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat yaw = YAW, GLfloat pitch = PITCH)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
		this->MovementSpeed = SPEED;
		this->MouseSensitivity = SENSITIVITY;
		this->Zoom = ZOOM;
		updateCameraVector();
	}
	GLfloat getZoom()
	{
		return this->Zoom;
	}
	glm::mat4 getViewMartix()
	{
		glm::mat4 martix = glm::lookAt(this->Position, this->Position + this->Front, this->Up);
		return martix;
	}
	void processKeyBoard(CameraMovementStatus status)
	{
		if (status == FORWARD)
		{
			this->Position+=this->Front*this->MovementSpeed;
		}
		if (status == BACKWARD)
		{
			this->Position -= this->Front*this->MovementSpeed;
		}
		if (status == RIGHT)
		{
			this->Position += this->Right*this->MovementSpeed;
		}
		if (status == LEFT)
		{
			this->Position -= this->Right*this->MovementSpeed;
		}
		//cout << "(" << this->Position.x << "," << this->Position.y << "," << this->Position.z << ")"<<endl;
	}
	void processMouseMovement(GLfloat xMove,GLfloat yMove)
	{
		xMove *= this->MouseSensitivity;
		yMove *= this->MouseSensitivity;
		this->Yaw += xMove;
		this->Pitch += yMove;
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
		updateCameraVector();
	}
	void processMouseScroll(int direction)
	{
		GLfloat zoomMove = glm::radians((GLfloat)direction)*3.0f;
		if (this->Zoom >= ZOOM_MIN&&this->Zoom <= ZOOM_MAX)
			this->Zoom += zoomMove;
		if (this->Zoom < ZOOM_MIN)
			this->Zoom = ZOOM_MIN;
		if (this->Zoom > ZOOM_MAX)
			this->Zoom = ZOOM_MAX;
	}


};

#endif