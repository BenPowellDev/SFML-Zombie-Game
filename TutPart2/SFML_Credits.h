#pragma once
#include <SFML/Graphics.hpp>

class SFML_Credits :
	public sf::Drawable
{
public:
	SFML_Credits(int windowWidth, int windowHeight);
	void update(sf::Time elapsedTime);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Text m_creditText2;
	sf::Text m_creditText;
};

