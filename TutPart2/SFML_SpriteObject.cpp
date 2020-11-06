#include "SFML_SpriteObject.h"

SFML_SpriteObject::SFML_SpriteObject()
	:m_initialised(false)
{
	m_defaultTexture = NULL;
}

SFML_SpriteObject::~SFML_SpriteObject()
{
	m_defaultTexture = NULL;
	delete m_defaultTexture;
}

bool SFML_SpriteObject::setDefaultTexture(std::string textureFilename)
{

	if (m_defaultTexture != NULL)
		delete m_defaultTexture;

	m_defaultTexture = new sf::Texture();

	if (!m_defaultTexture->loadFromFile(textureFilename))
	{
		//Failed loading the texture file
		m_initialised = false;
		delete m_defaultTexture;
		m_defaultTexture = NULL;
		return false;
	}

	//Texture file is loaded successfully
	m_spriteTexture = m_defaultTexture;

	//Temporary variables to read string of characters form the sprite info file
	std::string s_left, s_top, s_width, s_height, s_xcenter, s_ycenter;

	//We will convert the above into integers and store them here
	int i_left, i_top, i_width, i_height, i_xcenter, i_ycenter;

	//open the sprite info file
	std::ifstream inputFile("../Media/SpriteInfo/soldier.txt");

	if (inputFile.is_open())
	{
		//Read a line
		std::string stringLine;
		std::getline(inputFile, stringLine);

		if (inputFile.good())
		{
			//Seperate the line into 6 strings
			std::istringstream stringStream(stringLine);
			stringStream >> s_left;
			stringStream >> s_top;
			stringStream >> s_width;
			stringStream >> s_height;
			stringStream >> s_xcenter;
			stringStream >> s_ycenter;

			//Convert each into integers
			i_left = std::stoi(s_left);
			i_top = std::stoi(s_top);
			i_width = std::stoi(s_width);
			i_height = std::stoi(s_height);
			i_xcenter = std::stoi(s_xcenter);
			i_ycenter = std::stoi(s_ycenter);
		}
		
		//Setting default origin and rectangle
		m_spriteOrigin = sf::Vector2f(i_xcenter - i_left, i_ycenter - i_top);
		m_textureRectangle = sf::IntRect(i_left, i_top, i_width, i_height);

		inputFile.close();
	}

	m_initialised = true;
	return true;
}

void SFML_SpriteObject::setColor(const sf::Color& colour)
{
	m_sprite.setColor(colour);
}

sf::FloatRect SFML_SpriteObject::getLocalBounds() const
{
	float left = static_cast<float>(-m_spriteOrigin.x);
	float top = static_cast<float>(-m_spriteOrigin.y);
	float width = static_cast<float>(m_textureRectangle.width);
	float height = static_cast<float>(m_textureRectangle.height);

	return sf::FloatRect(left, top, width, height);
}

sf::FloatRect SFML_SpriteObject::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

sf::Vector2f SFML_SpriteObject::getWorldPosition()
{
	return getTransform().transformPoint(0, 0);
}

sf::Vector2f SFML_SpriteObject::getScreenPosition(sf::Transform ViewProjTransform)
{
	sf::Transform WorldViewProjTransform = ViewProjTransform * getTransform();
	return WorldViewProjTransform * sf::Vector2f();
}

void SFML_SpriteObject::update(float elapsedTime)
{
	if (!m_initialised)
		return;

	//Assign the texture to the sprite
	if (m_spriteTexture != NULL)
		m_sprite.setTexture(*m_spriteTexture);

	//Set the sprite's texture rectangle
	m_sprite.setTextureRect(m_textureRectangle);

	//Set the sprite's origin (pivot)
	m_sprite.setOrigin(m_spriteOrigin);
}

void SFML_SpriteObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}