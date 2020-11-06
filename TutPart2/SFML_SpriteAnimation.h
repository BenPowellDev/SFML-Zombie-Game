#pragma once
#include <SFML/Graphics.hpp>

class SFML_SpriteAnimation
{
public:
	SFML_SpriteAnimation();
	~SFML_SpriteAnimation();

	//Member function to store the memory address of the texture to use
	void setSpriteSheet(sf::Texture* spriteTexture);

	//Member function to add a frame (Rectangle and Centre)
	void addFrame(sf::IntRect textureRect, sf::Vector2f centre);

	//Member function that returns current frame information (rectangle and centre)
	bool getCurrentFrame(sf::IntRect* rect, sf::Vector2f* centre);

	//Member function to set the delay between shwoing consecutive frames
	void setFrameTime(float timeperFrame);
	float getFrameTime();

	//Member function that returns the delay between showing consecutive frames
	float getTimePerFrame();

	//Member function that returns the address of the texture used
	sf::Texture* getSpriteSheet();

	//Member function that returns the nuimber of frames in this animation
	int getNumberofFrames();
	void getTextureInfo(int newFrame, sf::IntRect* rect, sf::Vector2f* centre);

	//Member functions to set and get loop flag
	void setLoop(bool loop);
	bool isLooped() const;

	//Member functions to set and get interruptible flag
	void setCanBeInterrupted(bool trueorfalse);
	bool canbeinterrupted() const;

	//Member functions to increment and get frame number, and reset animation
	void incrementFrameNumber();
	int getCurrentFrameNumber();
	void resetCurrentAnimation();

	//Member Function to check if the animation is completed.
	//This will never be true if the animation is looping
	bool isCompleted();

private:
	//Frames information (Rectangles and Centres)
	std::vector<sf::IntRect> m_frameRectangleList;
	std::vector<sf::Vector2f> m_frameCentreList;

	//Pointer to the texture object
	sf::Texture* m_spritesheet;

	//Current frame numbers and time delay
	int m_currentFrameNumber;
	float m_timeperFrame;

	//Animation controls
	bool m_isLoop;
	bool m_canbeInterrupted;
	
};

