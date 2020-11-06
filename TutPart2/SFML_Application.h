#pragma once
#include <SFML\Graphics.hpp>
#include <SFML/System/NonCopyable.hpp>
#include "SFML_TitleScreen.h"
#include "SFML_GameWorldLayer.h"
#include "SFML_2DCamera.h"
#include "SFML_GameWorld.h"
#include "SFML_GameMenu.h"
#include "SFML_OptionMenu.h"
#include "SFML_Credits.h"

class SFML_Application: 
	public sf::NonCopyable
{
public:
	SFML_Application(int windowWidth, int windowHeight);
	~SFML_Application();
	void run();

private:
	void update(sf::Time elapsedTime);
	void render();
	sf::Font m_textFont;
	sf::Text m_messageText;

	//The array to store the game world layers
	std::vector<SFML_GameWorldLayer*> m_gameWorldLayerList;

	//The Member attribute to store the camera object
	SFML_2DCamera m_camera;

	//Member attribute to store the camera position
	sf::Vector2f m_cameraPosition;

	//Application State Enumeration
	enum AppState {
		TITLESCREEN,
		MAINMENU,
		GAMEPLAYING,
		EXITING,
		OPTIONMENU,
		CREDITS
	};

	//Member attribute for SFML_TitleScreen object
	SFML_TitleScreen m_titleScreen;

	//Member attribute for AppState
	AppState m_appState;

	//Member attributes for FPS data
	sf::Time m_timeCounter;
	sf::Time m_desiredUpdateTime;
	std::size_t m_FPSCounter;

	//function to update FPS data
	void updateFPS(sf::Time elapsedTime);

	//Game world object that contains m_gameWorld LayerList and m_camera
	SFML_GameWorld m_gameWorld;


private:
	//True if the application has been successfully initilised
	bool m_initialised;

	//Member attrubytes for graphics/window
	sf::RenderWindow m_window;
	std::size_t m_screenWidth;
	std::size_t m_screenHeight;

	//Member attribute for capturing events
	sf::Event m_event;

	//GameMenu Attirbute
	SFML_GameMenu m_mainMenu;

	//OptionMenu Attribute
	SFML_OptionMenu m_optionMenu;

	//Credits Attribute
	SFML_Credits m_creditScene;

};

