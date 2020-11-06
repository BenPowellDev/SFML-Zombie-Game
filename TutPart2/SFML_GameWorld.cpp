#include "SFML_GameWorld.h"

SFML_GameWorld::SFML_GameWorld(int windowWidth, int windowHeight, sf::RenderWindow* parentWindow)
	: m_windowWidth(windowWidth)
	, m_windowHeight(windowHeight)
	, m_camera(windowWidth, windowHeight)
{

	//Set default texutre  
	m_spriteObject.setDefaultTexture("../Media/Textures/light.png");

	//set Sprite position
	m_spriteObject.setPosition(0, 0);

	//Store the address of the parent render window in m_parentwindow
	m_parentWindow = parentWindow;

	//Sort the game world in descending order
	std::sort(m_gameWorldLayerList.begin(), m_gameWorldLayerList.end(), layerSortFunction);

	//Initialise Animation sprites
	m_animatedObject = new SFML_AnimatedSpriteObject();

	//Animation Textures
	m_idleAnimationID = m_animatedObject->addAnimation("../Media/Textures/blood.png", "../Media/SpriteInfo/blood.txt", 1.0f);

	//Set sprite position
	m_animatedObject->setPosition(0, 0);

	//Creates new Gameworld Layer
	SFML_GameWorldLayer* background = new SFML_GameWorldLayer("../Media/Textures/wall.png", 1.0f, windowWidth, windowHeight);

	//Inster it to the array
	m_gameWorldLayerList.push_back(background);

	//Creates new gameworld layer
	SFML_GameWorldLayer* foreground = new SFML_GameWorldLayer("../Media/Textures/wire.png", 0.3f,
		windowWidth, windowHeight);

	//Insert it to the array
	m_gameWorldLayerList.push_back(foreground);

	//Creates new gameworld layer
	SFML_GameWorldLayer* night = new SFML_GameWorldLayer("../Media/Textures/night.png", 0.3f,
		windowWidth, windowHeight);

	//Insert it to the array
	m_gameWorldLayerList.push_back(night);

	m_cameraPosition = sf::Vector2f(0, 0);

	//set object animation
	m_animatedObject->setCurrentAnimation(m_idleAnimationID);

	//Number of zombies
	m_numberofZombies = 3;

	for (int i = 0; i < m_numberofZombies; i++)
	{
		SFML_NPCSpriteObject* npcSprite = new SFML_NPCSpriteObject();
		npcSprite->setIdleAnimation("../Media/Textures/zombie-idle.png", "../Media/SpriteInfo/zombie-idle.txt", 1.0f);
		npcSprite->setWalkingAnimation("../Media/Textures/zombie-move.png", "../Media/SpriteInfo/zombie-move.txt", 1.0f);
		npcSprite->setAttackingAnimation("../Media/Textures/zombie-attack.png", "../Media/SpriteInfo/zombie-attack.txt", 1.0f);

		npcSprite->setPosition(rand() % 4000 - 2000, rand() % 4000 - 2000);
		npcSprite->setTargetLocation(sf::Vector2f(rand() % 4000 - 2000, rand() % 4000 - 2000));
		m_zombieCharacterList.push_back(npcSprite);
	}


	//Initialise Playable Character Object
	m_playerCharacter = new SFML_PlayableSpriteObject();
	m_playerCharacter->setKnifeIdleAnimation("../Media/Textures/hero_knife_idle.png", "../Media/SpriteInfo/hero_knife_idle.txt", 1.0f);
	m_playerCharacter->setKnifeWalkingAnimation("../Media/Textures/hero_knife_move.png", "../Media/SpriteInfo/hero_knife_move.txt", 1.0f);
	m_playerCharacter->setKnifeMeleeAnimation("../Media/Textures/hero_knife_melee.png", "../Media/SpriteInfo/hero_knife_melee.txt", 1.0f);

	m_playerCharacter->equipKnife();

	m_playerCharacter->toIdleState();
	

	//Set playable character postion
	m_playerCharacter->setPosition(0, 0);
}

SFML_GameWorld::~SFML_GameWorld()
{
	//Clear the GameWorldLayer List
	for (std::vector<SFML_GameWorldLayer*>::iterator it = m_gameWorldLayerList.begin();
		it != m_gameWorldLayerList.end();
		it++)
	{
		delete (*it);
	}
	m_gameWorldLayerList.clear();
	
	//Clear the memory space used for the animated object
	delete m_animatedObject;
}

void SFML_GameWorld::processEvents(float elapsedTime)
{
	//Camera Zoom
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
	{
		m_camera.zoomUp(elapsedTime);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
	{
		m_camera.zoomDown(elapsedTime);
	}

	//Get camera transform
	sf::Transform transform;
	transform = m_camera.getProjTransform() * m_camera.getViewTransform();

	//Take inverse (screen -> world)
	sf::Transform inv = transform.getInverse();

	//Get mouseposition (in screen space)
	sf::Vector2f mouseScreenPosition = sf::Vector2f(sf::Mouse::getPosition(*m_parentWindow));

	//Convert to world space coordinate
	sf::Vector2f wp = inv.transformPoint(mouseScreenPosition);

	//Get sprite global bounding circle
	sf::FloatRect rect = m_spriteObject.getGlobalBounds();

	if (rect.contains(wp))
	{
		//If mouse pointer inside the bounding rectangle, tint the sprite red
		m_spriteObject.setColor(sf::Color(255, 0, 0));
	}
	else
	{
		//Otherwise revert back to normal color (white)
		m_spriteObject.setColor(sf::Color(255, 255, 255));
	}

	SFML_BulletSpriteObject* bullet = m_playerCharacter->processEvents(elapsedTime, transform, mouseScreenPosition);
	if (bullet)
		m_bulletList.push_back(bullet);
	
	//Update NPC Vision Cone
	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		if (m_zombieCharacterList[counter]->isInVision(wp))
		{
			//The mouse pointer is within vision of the NPC's vision cone
			m_zombieCharacterList[counter]->setColor(sf::Color(128, 0, 0));
		}
		else
		{
			//The mouse pointer is outside the NPC's vision cone
			m_zombieCharacterList[counter]->setColor(sf::Color(255, 255, 255));
		}
	}

	sf::FloatRect boundingBox = m_playerCharacter->getGlobalBounds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
	{
		//Equip Flashlight
		m_playerCharacter->setFlashlightIdleAnimation("../Media/Textures/hero_flashlight_idle.png", "../Media/SpriteInfo/hero_flashlight_idle.txt", 1.0f);
		m_playerCharacter->setFlashlightWalkingAnimation("../Media/Textures/hero_flashlight_move.png", "../Media/SpriteInfo/hero_flashlight_move.txt", 1.0f);
		m_playerCharacter->setFlashlightMeleeAnimation("../Media/Textures/hero_flashlight_melee.png", "../Media/SpriteInfo/hero_flashlight_melee.txt", 1.0f);

		m_playerCharacter->equipFlashlight();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		//Equip Knife
		m_playerCharacter->setKnifeIdleAnimation("../Media/Textures/hero_knife_idle.png", "../Media/SpriteInfo/hero_knife_idle.txt", 1.0f);
		m_playerCharacter->setKnifeWalkingAnimation("../Media/Textures/hero_knife_move.png", "../Media/SpriteInfo/hero_knife_move.txt", 1.0f);
		m_playerCharacter->setKnifeMeleeAnimation("../Media/Textures/hero_knife_melee.png", "../Media/SpriteInfo/hero_knife_melee.txt", 1.0f);

		m_playerCharacter->equipKnife();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
	{
		//Equip Pistol
		m_playerCharacter->setPistolIdleAnimation("../Media/Textures/hero_pistol_idle.png", "../Media/SpriteInfo/hero_pistol_idle.txt", 1.0f);
		m_playerCharacter->setPistolWalkingAnimation("../Media/Textures/hero_pistol_move.png", "../Media/SpriteInfo/hero_pistol_move.txt", 1.0f);
		m_playerCharacter->setPistolMeleeAnimation("../Media/Textures/hero_pistol_melee.png", "../Media/SpriteInfo/hero_pistol_melee.txt", 1.0f);
		m_playerCharacter->setPistolShootAnimation("../Media/Textures/hero_pistol_shoot.png", "../Media/SpriteInfo/hero_pistol_shoot.txt", 1.0f);

		m_playerCharacter->equipPistol();

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		//Equip Rifle
		m_playerCharacter->setRifleIdleAnimation("../Media/Textures/hero_rifle_idle.png", "../Media/SpriteInfo/hero_rifle_idle.txt", 1.0f);
		m_playerCharacter->setRifleWalkingAnimation("../Media/Textures/hero_rifle_move.png", "../Media/SpriteInfo/hero_rifle_move.txt", 1.0f);
		m_playerCharacter->setRifleMeleeAnimation("../Media/Textures/hero_rifle_melee.png", "../Media/SpriteInfo/hero_rifle_melee.txt", 1.0f);
		m_playerCharacter->setRifleShootAnimation("../Media/Textures/hero_rifle_shoot.png", "../Media/SpriteInfo/hero_rifle_shoot.txt", 1.0f);

		m_playerCharacter->equipRifle();

	}

	for (unsigned int counter = 0; counter < m_bulletList.size(); counter++)
	{
		for (unsigned int counter2 = 0; counter2 < m_zombieCharacterList.size(); counter2++)
		{
			sf::FloatRect otherBox = m_bulletList[counter]->getGlobalBounds();
			if (boundingBox.intersects(otherBox))
			{
				m_zombieCharacterList[counter2]->setColor(sf::Color(255, 0, 0));
			}
			else
			{
				m_zombieCharacterList[counter2]->setColor(sf::Color(255, 255, 255));
			}
		}	
	}
}

void SFML_GameWorld::update(float elapsedTime)
{
	processEvents(elapsedTime);
	m_animatedObject->update(elapsedTime);
	m_spriteObject.update(elapsedTime);
	m_camera.setPosition(m_cameraPosition);

	//Update Playable Character
	m_playerCharacter->update(elapsedTime);
	m_camera.setPosition(m_playerCharacter->getWorldPosition());


	for (std::size_t counter = 0; counter < m_gameWorldLayerList.size(); counter++)
	{
		m_gameWorldLayerList[counter]->followCamera(&m_camera);
	}
	
	//Update each zombie object in the list
	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		m_zombieCharacterList[counter]->update(elapsedTime);
	}

	//Update all bullets
	for (unsigned int counter = 0; counter < m_bulletList.size(); counter++)
	{
		m_bulletList[counter]->update(elapsedTime);
	}

}

void SFML_GameWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Render each game world layer with parallax factor < 1.0f
	for (unsigned int counter = 0; counter < m_gameWorldLayerList.size(); counter++)
	{
		if (m_gameWorldLayerList[counter]->getparallaxFactor() >= 1.0f)
			target.draw(*m_gameWorldLayerList[counter]);
	}

	//get the camera view-projection transform and set it to the render state
	sf::RenderStates renderState;
	renderState.transform = m_camera.getProjTransform() * m_camera.getViewTransform();

	//Draw the Sprite with the renderstate
	//target.draw(m_spriteObject, renderState);

	//Draw each zombie object in the list
	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		target.draw(*m_zombieCharacterList[counter], renderState);
	}

	//Draw Animations
	//target.draw(*m_animatedObject, renderState);

	//Draw Playable character
	target.draw(*m_playerCharacter, renderState);

	//Draw each bullet object in the list
	for (unsigned int counter = 0; counter < m_bulletList.size(); counter++)
	{
		target.draw(*m_bulletList[counter], renderState);
	}
	
	//Render each game world layer with parallax factor < 1.0f
	for (unsigned int counter = 0; counter < m_gameWorldLayerList.size(); counter++)
	{
		if (m_gameWorldLayerList[counter]->getparallaxFactor() < 1.0f)
			target.draw(*m_gameWorldLayerList[counter]);
	}

}

bool SFML_GameWorld::layerSortFunction(SFML_GameWorldLayer* a, SFML_GameWorldLayer* b)
{
	return (a->getparallaxFactor() > b->getparallaxFactor());
}