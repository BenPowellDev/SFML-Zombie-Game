#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class SFML_SpriteObject :
	public sf::Drawable, public sf::Transformable
{
public:
	SFML_SpriteObject();
	virtual ~SFML_SpriteObject();

	//Member functions to load and set the default texture to use
	bool setDefaultTexture(std::string textureFilename);
	bool setDefaultTexture(std::string textureFilename, std::string spriteInfoFilename);

	//Member function to set tint colour
	void setColor(const sf::Color& color);

	//Member function to get the object's world position
	sf::Vector2f getWorldPosition();

	//Member function to get the opbject's screen position
	sf::Vector2f getScreenPosition(sf::Transform cameraScreenTransform);

	//Member function to get the object's global and local bounding rectangles
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;

	//Member function to update the object's states
	virtual void update(float elapsedTime);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//Member attribute for default texture
	sf::Texture* m_defaultTexture;

protected:
	//Member attribute to start the SFML sprite
	sf::Sprite m_sprite;

	//Member attribute that points to the current used texture
	sf::Texture* m_spriteTexture;

	//Member attribute to store texture rectangle
	sf::IntRect m_textureRectangle;

	//Member attribute to store origin point
	sf::Vector2f m_spriteOrigin;

	//To help us keep track if the object has been successfully initialised
	bool m_initialised;

};

