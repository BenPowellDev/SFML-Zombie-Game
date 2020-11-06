#include "SFML_Credits.h"

SFML_Credits::SFML_Credits(int windowWidth, int windowHeight)
{
	const float scale = 0.8;
	const float invScale = 1;
	
	sf::Font myFont;
	sf::View myView;
	myView.setSize(windowWidth, windowHeight);
	myView.setCenter(windowWidth / 2, windowHeight / 2);
	myView.setViewport(sf::FloatRect(0, (1 - scale) / 2, scale, scale));
	
	myFont.loadFromFile("../Media/Fonts/SegoeMarker.ttf");
	
	sf::Text m_creditText("Creator and Lead Developer\nSantiago Aboy Solanes\nLead Artist\nThat other guy\nThis\nis\nto\nshow\nhow\nit\nappears", myFont, windowWidth / 16);
	m_creditText.setPosition((windowWidth * invScale - m_creditText.getGlobalBounds().width) / 2, windowHeight);
	m_creditText.setFillColor(sf::Color::White);

	//Such as this one
	sf::Text m_creditText2("I am centered", myFont, windowWidth / 16);
	m_creditText2.setPosition((windowWidth * invScale - m_creditText2.getGlobalBounds().width) / 2, m_creditText.getGlobalBounds().top + m_creditText.getGlobalBounds().height);
	m_creditText2.setFillColor(sf::Color::Red);

	//Move the credits upward. You may want this to be tied to a sf::Clock/sf::Time and move it taking into account the delta time
	myView.move(0, windowHeight * 0.0001);
	//Reset the view if they go "too far"
	if (myView.getCenter().y > windowHeight * 2 + m_creditText.getGlobalBounds().height + m_creditText2.getGlobalBounds().height) {
			myView.setCenter(windowWidth / 2, windowHeight / 2);
		}
}

void SFML_Credits::update(sf::Time elapsedTime)
{

}

void SFML_Credits::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	target.draw(m_creditText);
	target.draw(m_creditText2);

}