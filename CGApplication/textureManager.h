#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "windowSetting.h"
#include <map>
#include <SOIL.h>

class TextureManager
{
public:
	~TextureManager()
	{		
		unloadAllTexture();
	}
	bool loadTexture(const char *filename, const unsigned int textureID, GLenum image_format = GL_RGB,
		GLint internal_format = GL_RGB, GLint level = 0, GLint border = 0)
	{
		GLuint glTexture;

		if (myTextureMap.find(textureID) != myTextureMap.end())
			glDeleteTextures(1, &(myTextureMap[textureID]));

		glTexture = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS |SOIL_FLAG_POWER_OF_TWO);
		myTextureMap[textureID] = glTexture;
		myTexturePathMap[textureID] = (std::string)filename;

		//unsigned char *imageBits(0);
		//int width = 0, height = 0;
		//imageBits=SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
		//if (imageBits == 0 || width == 0 || height == 0)
		//	return false;

		//glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, image_format, GL_UNSIGNED_BYTE, imageBits);
		//glGenerateMipmap(GL_TEXTURE_2D);
		//SOIL_free_image_data(imageBits);

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
	bool unloadTexture(const unsigned int textureID)
	{
		if (myTextureMap.find(textureID) != myTextureMap.end())
		{
			glDeleteTextures(1, &(myTextureMap[textureID]));
			myTextureMap.erase(textureID);
			myTexturePathMap.erase(textureID);
			return true;
		}
		else
			return false;
	}
	bool bindTexture(const unsigned int textureID)
	{
		if (myTextureMap.find(textureID) != myTextureMap.end())
		{
			glBindTexture(GL_TEXTURE_2D, myTextureMap[textureID]);
			return true;
		}
		else
			return false;
	}
	void unloadAllTexture()
	{
		std::map<unsigned int, GLuint>::iterator mapi;
		for (mapi = myTextureMap.begin(); mapi != myTextureMap.end(); )
			unloadTexture(mapi->first);
		myTextureMap.clear();
		myTexturePathMap.clear();
	}
	void saveScreenshot(const char *filePath)
	{
		if (SOIL_save_screenshot(filePath, SOIL_SAVE_TYPE_BMP, 0, 0, WIDTH, HEIGHT) == 1)
			std::cout << "Í¼Æ¬±£´æ³É¹¦"<<std::endl;
		else
			std::cout << "Í¼Æ¬±£´æÊ§°Ü"<<std::endl;
	}
	std::string getTexturePath(const unsigned int textureID)
	{
		if (myTexturePathMap.find(textureID) != myTexturePathMap.end())
		{
			return myTexturePathMap[textureID];
		}
		else
			return "";
	}
	void changeTexturePath(const unsigned int textureID, std::string newFilePath)
	{
		if (myTexturePathMap.find(textureID) != myTexturePathMap.end())
		{
			myTexturePathMap[textureID] = newFilePath;
			glDeleteTextures(1, &(myTextureMap[textureID]));
			myTextureMap[textureID] = SOIL_load_OGL_texture(newFilePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_POWER_OF_TWO);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			myTextureMap[textureID] = SOIL_load_OGL_texture(newFilePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_POWER_OF_TWO);
			myTexturePathMap[textureID] = (std::string)newFilePath;
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
private:
	std::map<unsigned int, GLuint> myTextureMap;
	std::map<unsigned int, std::string> myTexturePathMap;
};


#endif 
