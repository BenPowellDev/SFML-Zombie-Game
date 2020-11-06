#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "SFML_Application.h"

SFML_Application::SFML_Application(int windowWidth, int windowHeight)
	:m_window(sf::VideoMode(windowWidth, windowHeight), "SFML Application", sf::Style::Fullscreen)
	, m_initialised(true)
	, m_screenWidth(windowWidth)
	, m_screenHeight(windowHeight)
	, m_appState(TITLESCREEN)
	, m_titleScreen("../Media/Textures/title-screen.png", windowWidth, windowHeight)
	, m_mainMenu("../Media/MenuInfo/main-menu.txt", windowWidth, windowHeight)
	, m_optionMenu("../Media/MenuInfo/option-menu.txt", windowWidth, windowHeight)
	, m_creditScene(windowWidth, windowHeight)
	, m_camera(windowWidth, windowHeight)
	, m_gameWorld(windowWidth, windowHeight, &m_window)
	, m_desiredUpdateTime(sf::seconds(1.f / 60.f))
	, m_timeCounter(sf::seconds(0))
	, m_FPSCounter(0)
{
	m_initialised = m_textFont.loadFromFile("../Media/Fonts/SegoeMarker.ttf");

	m_messageText.setFont(m_textFont);

	m_mainMenu.setScale(0.85f, 0.85f);
	m_mainMenu.setPosition(m_screenWidth / 2, m_screenHeight / 2);

	m_optionMenu.setScale(0.85f, 0.85f);
	m_optionMenu.setPosition(m_screenWidth / 2, m_screenHeight / 2);

}

SFML_Application::~SFML_Application()
{
	//Clear the gameWorldLayerList
	for (std::vector<SFML_GameWorldLayer*>::iterator it = m_gameWorldLayerList.begin();
		it != m_gameWorldLayerList.end();
		it++)
	{
		delete (*it);
	}

	m_gameWorldLayerList.clear();
}

void SFML_Application::run()
{
	if (!m_initialised)
		return;

	sf::Clock clock;

	//Variable to track the time since update() was last excectued
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (m_window.isOpen())
	{
		//Get the elapsed time since the last loop
		sf::Time elapsedTime = clock.restart();
		
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > m_desiredUpdateTime)
		{
			//Update application states
			update(m_desiredUpdateTime);

			//Subtract m_desiredUpdateTime for timeSinceLastUpdate
			timeSinceLastUpdate -= m_desiredUpdateTime;
		}

		//Calculate FPS
		updateFPS(elapsedTime);

		//Render result to the screen
		render();
	}
}

void SFML_Application::updateFPS(sf::Time elapsedTime)
{
	//Increment the time counter by the time since the last render
	m_timeCounter += elapsedTime;

	//Increment FPS counter by one
	m_FPSCounter += 1;

	//if one second has passed
	if (m_timeCounter >= sf::seconds(1.0f))
	{
		//Set the message text
		m_messageText.setString(
			"Refresh rate = " + std::to_string(m_FPSCounter) +
			" fps \n" +
			"Time between screen refreshes = " +
			std::to_string(m_timeCounter.asMicroseconds() / m_FPSCounter) +
			"microseconds \n"
		);

		//Take away one second from the time counter
		m_timeCounter -= sf::seconds(1.0f);

		//Reset the FPS counter
		m_FPSCounter = 0;
	}
}


	void SFML_Application::update(sf::Time elapsedTime)
	{
		//Check if the player has tried to close the window, if so close the application
		//This m_event triggers when 1) The x button is pressed or 2) alt+f4 is used.
		bool validEventPoll = m_window.pollEvent(m_event);
		if (m_event.type == sf::Event::Closed)
			m_window.close();

		int selection;

		switch (m_appState)
		{
		case TITLESCREEN:
			if (m_event.type == sf::Event::KeyPressed)
			{
				m_appState = MAINMENU;
			}

			m_titleScreen.update(elapsedTime);
			break;

		case MAINMENU:
			selection = m_mainMenu.update(elapsedTime);
			if (selection >= 0)
			{
				if (selection == 0)
					m_appState = GAMEPLAYING;
				else if (selection == 1)
					m_appState = OPTIONMENU;
				else if (selection == 2)
					m_appState = EXITING;
			}
			break;

		case OPTIONMENU:
			selection = m_optionMenu.update(elapsedTime);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				m_appState = MAINMENU;
			else if (selection >= 0)
			{
				if (selection == 0)
					m_appState = CREDITS;
				else if (selection == 1)
					m_appState = OPTIONMENU;
				else if (selection == 2)
					m_appState = MAINMENU;
			}
			break;

		case GAMEPLAYING:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				m_appState = MAINMENU;
			
			m_gameWorld.update(elapsedTime.asSeconds());
			
			for (std::size_t counter = 0; counter < m_gameWorldLayerList.size(); counter++)
			{
				m_gameWorldLayerList[counter]->followCamera(&m_camera);
			}
			
			{
				//Camera
				float cameraPanSpeed = 60.0f;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					m_cameraPosition.x = m_cameraPosition.x - cameraPanSpeed * elapsedTime.asSeconds();
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
					m_cameraPosition.x = m_cameraPosition.x + cameraPanSpeed * elapsedTime.asSeconds();
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					m_cameraPosition.y = m_cameraPosition.y - cameraPanSpeed * elapsedTime.asSeconds();

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					m_cameraPosition.y = m_cameraPosition.y + cameraPanSpeed * elapsedTime.asSeconds();
				}
			}

			//Zoom
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
			{
				m_camera.zoomUp(elapsedTime.asSeconds());
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			{
				m_camera.zoomDown(elapsedTime.asSeconds());
			}
			
			m_camera.setPosition(m_cameraPosition);
			
			break;
		
		case CREDITS:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				m_appState = OPTIONMENU;
			break;

		case EXITING:
			m_window.close();
			break;
		}

		m_messageText.setPosition(m_screenWidth / 2, m_screenHeight / 2);
		m_messageText.setCharacterSize(100);
	}

	void SFML_Application::render()
	{
		m_window.clear();
		
		switch (m_appState)
		{
		case TITLESCREEN:
				m_window.draw(m_titleScreen);
				break;
			case MAINMENU:
				m_window.draw(m_mainMenu);
				break;
			case OPTIONMENU:
				m_window.draw(m_optionMenu);
				break;
			case GAMEPLAYING:
				m_window.draw(m_gameWorld);
				for (std::size_t counter = 0; counter < m_gameWorldLayerList.size(); counter++)
				{
					m_window.draw(*m_gameWorldLayerList[counter]);
				}
				break;
			case CREDITS:
				m_window.draw(m_creditScene);
				break;
			case EXITING:
				break;
		}

		m_window.display();
	}