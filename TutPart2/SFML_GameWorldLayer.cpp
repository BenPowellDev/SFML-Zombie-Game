#include "SFML_GameWorldLayer.h"

SFML_GameWorldLayer::SFML_GameWorldLayer(std::string textureFileName,
	float parallaxFactor, int viewportWidth, int viewportHeight)
	: m_layerTexture()
	, m_layerVertices(sf::Quads, 4)
	, m_parallaxFactor(parallaxFactor)
{
	if (m_layerTexture.loadFromFile(textureFileName))
	{
		//Set texture to repeat
		m_layerTexture.setRepeated(true);

		//Set the vertices of the quad
		//0 -------- 1
		//|          |
		//|          |
		//|          |
		//3 -------- 2

		m_layerVertices[0].position = sf::Vector2f(0.f, 0.f);
		m_layerVertices[1].position = sf::Vector2f(0.f, viewportHeight);
		m_layerVertices[2].position = sf::Vector2f(viewportWidth, viewportHeight);
		m_layerVertices[3].position = sf::Vector2f(viewportWidth, 0.f);
	}
}

void SFML_GameWorldLayer::followCamera(SFML_2DCamera* camera)
{
	//Find out the visible area rectangle of the camera
	sf::FloatRect cameraWindows = camera->getCameraWindow(m_parallaxFactor);

	//Calculate the left, right, top and bottom values
	int left = cameraWindows.left;
	int right = cameraWindows.left + cameraWindows.width;
	int top = cameraWindows.top;
	int bottom = cameraWindows.top + cameraWindows.height;

	//Set the texture coordinates using those values
	m_layerVertices[0].texCoords = sf::Vector2f(left, top);
	m_layerVertices[1].texCoords = sf::Vector2f(left, bottom);
	m_layerVertices[2].texCoords = sf::Vector2f(right, bottom);
	m_layerVertices[3].texCoords = sf::Vector2f(right, top);
}

void SFML_GameWorldLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Assign the m_layerTexture to the renderstate
	states.texture = &m_layerTexture;

	//Draw the quad using the renderstate
	target.draw(m_layerVertices, states);

}

void SFML_GameWorldLayer::setparallaxFactor(float parallaxFactor)
{
	m_parallaxFactor = parallaxFactor;
}

float SFML_GameWorldLayer::getparallaxFactor()
{
	return m_parallaxFactor;
}