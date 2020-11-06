#include "SFML_TitleScreen.h"

SFML_TitleScreen::SFML_TitleScreen(std::string texturefilename, int windowWidth, int windowHeight)
{
	//Load Title Screen Textures
	m_titleScreenTexture.loadFromFile("../Media/Textures/title-screen.png");
	m_titleScreenSprite.setTexture(m_titleScreenTexture);

	sf::Vector2u bkTextureSize = m_titleScreenTexture.getSize();
	m_titleScreenSprite.setTextureRect(sf::IntRect(0, 0, bkTextureSize.x, bkTextureSize.y));

	m_titleScreenSprite.setPosition(0, 0);

	//Load Message Textures
	m_messageTexture.loadFromFile("../Media/Textures/continue.png");
	m_messageSprite.setTexture(m_messageTexture);

	m_messageSprite.setPosition(windowWidth * 0.5f, windowHeight * 0.75f);

	sf::Vector2u msgTextureSize = m_messageTexture.getSize();
	m_messageSprite.setOrigin(msgTextureSize.x / 2, msgTextureSize.y / 2);

	m_messageSprite.setTextureRect(sf::IntRect(0, 0, msgTextureSize.x, msgTextureSize.y));

	//Sets Message Color
	m_messageSprite.setColor(sf::Color(255, 0, 0));

	//Message Flash
	m_timeSinceLastFlash = sf::Time::Zero;
	m_flashingTime = sf::seconds(0.2f);
}

void SFML_TitleScreen::update(sf::Time elapsedTime)
{

	m_timeSinceLastFlash += elapsedTime;

	if (m_timeSinceLastFlash > m_flashingTime)
	{
		m_flashon = !m_flashon;
		m_timeSinceLastFlash -= m_flashingTime;
		if (m_flashon)
			m_messageSprite.setColor(sf::Color(255, 255, 255));
		else
			m_messageSprite.setColor(sf::Color(255, 0, 0));
	}
}

void SFML_TitleScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	
	target.draw(m_titleScreenSprite);
	target.draw(m_messageSprite);

}