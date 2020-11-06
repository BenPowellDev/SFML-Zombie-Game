#pragma once
#include "SFML_SpriteObject.h"
#include "SFML_SpriteAnimation.h"

class SFML_AnimatedSpriteObject :
	public SFML_SpriteObject
{
public:
	SFML_AnimatedSpriteObject();
	virtual ~SFML_AnimatedSpriteObject();

	//Member functions to add animation sequence
	int addAnimation(std::string SpriteSheetFilename,
		std::string spriteInfoFilename,
		float animationTime);

	//Member function that sets the current animation
	void setCurrentAnimation(int animationindex);

	//Member function that returns the address to the current animation
	SFML_SpriteAnimation* getCurrentAnimation();

	//Member function that updates object states and variables
	virtual void update(float elapsedTime);

	//Member functions to control playback
	void play();
	void pause();
	void stop();

private:
	std::vector<sf::Texture*> m_spriteTextureList;
	int m_currentAnimationID;
	float m_currentTime;
	bool m_isPaused;


protected:
	std::vector<SFML_SpriteAnimation*> m_animatedSpriteList;
	SFML_SpriteAnimation* m_currentAnimation;
};

