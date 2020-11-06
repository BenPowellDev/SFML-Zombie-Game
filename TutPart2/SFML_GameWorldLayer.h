#pragma once
#include <SFML/Graphics.hpp>
#include "SFML_2DCamera.h"

class SFML_GameWorldLayer :
	public sf::Drawable
{
public:
	SFML_GameWorldLayer(std::string textureFileName,
		float parallaxFactor,
		int viewportWidth,
		int viewportHeight);

	void followCamera(SFML_2DCamera* camera);

	//Member Function to set parallax factor after object creation
	void setparallaxFactor(float parallaxFactor);

	//Member function to get the layers current parallax factor
	float getparallaxFactor();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//Member attribute to store the texture object
	sf::Texture m_layerTexture;

	//Member attribute to store the window vertices
	sf::VertexArray m_layerVertices;

	//Member attribute to store the layers parallax factor
	float m_parallaxFactor;
};

