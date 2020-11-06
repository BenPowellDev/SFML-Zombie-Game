#include "SFML_AnimatedSpriteObject.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

SFML_AnimatedSpriteObject::SFML_AnimatedSpriteObject()
	: m_currentAnimation(NULL)
	, m_isPaused(false)
	, m_currentAnimationID(-1)
{

}

SFML_AnimatedSpriteObject::~SFML_AnimatedSpriteObject()
{
	//Deleting texture objects in the lists
	for (std::vector<sf::Texture*>::iterator it = m_spriteTextureList.begin();
		it != m_spriteTextureList.end();
		++it)
	{
		delete (*it);
	}

	m_spriteTextureList.clear();

	//Deleting animation objects in the list
	for (std::vector<SFML_SpriteAnimation*>::iterator it = m_animatedSpriteList.begin();
		it != m_animatedSpriteList.end();
		++it)
	{
		delete (*it);
	}

	m_animatedSpriteList.clear();
}

int SFML_AnimatedSpriteObject::addAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	//Create a new texture object
	sf::Texture* spriteTexture = new sf::Texture();

	//Load the texture file
	if (!spriteTexture->loadFromFile(spriteSheetFilename))
	{
		return -1;
	}

	//Store the texture to the list
	m_spriteTextureList.push_back(spriteTexture);

	//Create a new animation
	SFML_SpriteAnimation* animation = new SFML_SpriteAnimation();

	//Store the address of the texture object in the animation
	animation->setSpriteSheet(spriteTexture);

	//Loading sequences of frames (rectangles and centres)
	//by reading the spriteinfo file line by line

	std::string left;
	std::string top;
	std::string width;
	std::string height;
	std::string xcenter;
	std::string ycenter;

	int ileft, itop, iwidth, iheight, ixcenter, iycenter;

	int frameCounter = 0;

	//Open the file
	std::ifstream inputFile(spriteInfoFilename);

	if (inputFile.is_open())
	{
		do {
			std::string stringLine;
			std::getline(inputFile, stringLine);


			if (inputFile.good())
			{
				//Line read successfully - split the result into 6 (whitespace separated)
				std::istringstream stringStream(stringLine);
				stringStream >> left;
				stringStream >> top;
				stringStream >> width;
				stringStream >> height;
				stringStream >> xcenter;
				stringStream >> ycenter;

				//Converts them from strings to integers
				ileft = std::stoi(left);
				itop = std::stoi(top);
				iwidth = std::stoi(width);
				iheight = std::stoi(height);
				ixcenter = std::stoi(xcenter);
				iycenter = std::stoi(ycenter);

				//Use them as animation frame
				animation->addFrame(sf::IntRect(ileft, itop, iwidth, iheight), sf::Vector2f(ixcenter - ileft, iycenter - itop));

				//Keep track of the counter - how many frames
				frameCounter++;
			}
			else
				break;
		} while (true);

		inputFile.close();

	}

		if (frameCounter > 0)
		{
			animation->setFrameTime(animationTime / frameCounter);
			m_animatedSpriteList.push_back(animation);
			m_initialised = true;
			return m_animatedSpriteList.size() - 1;
		}
		else
			return -1;
	
}

void SFML_AnimatedSpriteObject::setCurrentAnimation(int animationindex)
{
	SFML_SpriteAnimation* newAnimation = m_animatedSpriteList[animationindex];

	if (newAnimation)
	{
		if (m_currentAnimation != newAnimation)
		{
			m_currentAnimation = newAnimation;
			m_spriteTexture = m_currentAnimation->getSpriteSheet();
			m_currentAnimation->resetCurrentAnimation();
			m_currentAnimation->getCurrentFrame(&m_textureRectangle, &m_spriteOrigin);
			m_currentTime = 0;
		}
	}
}

SFML_SpriteAnimation* SFML_AnimatedSpriteObject::getCurrentAnimation()
{
	return m_currentAnimation;
}

void SFML_AnimatedSpriteObject::play()
{
	m_isPaused = false;
}

void SFML_AnimatedSpriteObject::pause()
{
	m_isPaused = true;
}

void SFML_AnimatedSpriteObject::stop()
{
	m_isPaused = true;
	m_currentTime = 0;
}

void SFML_AnimatedSpriteObject::update(float elapsedTime)
{
	//if not paused and we have a current valid animation
	if (!m_isPaused && m_currentAnimation)
	{
		float frameTime = m_currentAnimation->getTimePerFrame();

		//add elapsed time
		m_currentTime += elapsedTime;

		//if current time is bigger than the frame time advance one frame
		if (m_currentTime >= frameTime)
		{
			m_currentAnimation->incrementFrameNumber();
			m_currentAnimation->getCurrentFrame(&m_textureRectangle, &m_spriteOrigin);
			m_currentTime = m_currentTime - frameTime;
		}
	}

	SFML_SpriteObject::update(elapsedTime);
}


