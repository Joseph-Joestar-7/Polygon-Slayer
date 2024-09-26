#include "Game.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#define PI 3.14159265

Game::Game(const std::string& configFilePath)
{
	this->init(configFilePath);
}

void Game::init(const std::string& filePath)
{
	std::ifstream fin(filePath);
	std::string token;

	int width, height, frameRate;
	bool isFullScreen;
	fin >> token >> width >> height >> frameRate >> isFullScreen;

	if (isFullScreen)
		m_window.create(sf::VideoMode(width, height), "Polygon Slayer", sf::Style::Fullscreen);
	else
		m_window.create(sf::VideoMode(width, height), "Polygon Slayer");
	m_window.setFramerateLimit(frameRate);

	std::string fontName;
	int fontSize, fontR, fontG, fontB;
	fin >> token >> fontName >> fontSize >> fontR >> fontG >> fontB;

	if (!m_font.loadFromFile(fontName)) {
		std::cerr << "Couldn't load the font\n";
		exit(-1);
	}
	m_text = sf::Text("Score: 0", m_font, fontSize);

	// player config
	PlayerConfig& p = m_playerConfig;
	fin>>token>>p.SR>> p.CR >> p.S >> p.FR >> p.FG >> p.FB >> p.OR >>
		p.OG >> p.OB >> p.OT >> p.V >> p.T;

	// enemy config
	EnemyConfig& e = m_enemyConfig;	 
	fin >> token >> e.SR >> e.CR >> e.SMIN >> e.SMAX >> e.OR >> e.OG >>
		e.OB >> e.OT >> e.VMIN >> e.VMAX >> e.L >> e.SI;

	// bullet config
	BulletConfig& b = m_bulletConfig;  
	fin >> token >> b.SR >> b.CR >> b.S >> b.FR >> b.FG >> b.FB >> b.OR >>
		b.OG >> b.OB >> b.OT >> b.V >> b.L;

	this->spawnPlayer();
}

void Game::sMovement()
{
	// player movement
	// reset player movement
	m_player->cTransform->velocity = { 0.0f, 0.0f };

	if (m_player->cInput->up) {
		m_player->cTransform->velocity.y = -m_playerConfig.S;
	}
	else if (m_player->cInput->down) {
		m_player->cTransform->velocity.y = m_playerConfig.S;
	}
	else if (m_player->cInput->left) {
		m_player->cTransform->velocity.x = -m_playerConfig.S;
	}
	else if (m_player->cInput->right) {
		m_player->cTransform->velocity.x = m_playerConfig.S;
	}

	for (const auto e : m_entityManager.getEntities()) {
		e->cTransform->position += e->cTransform->velocity;
	}
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_running = false;
			return;
		}

		// Keyboard events
		if (event.type == sf::Event::KeyPressed) {
			// Player movement
			switch (event.key.code) {
			case sf::Keyboard::W:
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = true;
				break;
				// Exit game
			case sf::Keyboard::Q:
				m_running = false;
				break;
				// Pause game
			case sf::Keyboard::Escape:
				m_paused = !m_paused;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code) {
				// Player movement
			case sf::Keyboard::W:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				break;
			default:
				break;
			}
		}

		// Mouse events
		if (event.type == sf::Event::MouseButtonPressed) {
			// Left mouse button shoots a button
			if (event.mouseButton.button == sf::Mouse::Left) {
				glm::vec2 mousePos(event.mouseButton.x,
					event.mouseButton.y);
				// Required to check if game is in pause state! Smart asses can
				// pause the game, shoot mutliple bullets then resume, multiple
				// bullets will be spawned!
				if (!m_paused)
					spawnBullet(m_player, mousePos);
			}

			if (event.mouseButton.button == sf::Mouse::Right) {
				// If special weapon is available
				if (m_lastUsedSpecialWeaponFrame +
					m_playerConfig.T <
					m_currentFrame) {
					m_lastUsedSpecialWeaponFrame =
						m_currentFrame;
					spawnSpecialWeapon(m_player);
				}
			}
		}
	}
}

void Game::sRender()
{
	m_window.clear();

	for (const auto e : m_entityManager.getEntities()) {
		// set position
		e->cShape->circle.setPosition(e->cTransform->position.x,
			e->cTransform->position.y);

		// set rotation
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);

		// set opacity proportional to it's remaining lifespan
		if (e->cLifespan) {
			int opacity = 255 * (e->cLifespan->remaining) /
				(e->cLifespan->total);
			// Fill color
			const sf::Color& currFill =
				e->cShape->circle.getFillColor();
			sf::Color fillColor(currFill.r, currFill.g, currFill.b,
				opacity);
			e->cShape->circle.setFillColor(fillColor);

			// Outline color
			const sf::Color& currOutline =
				e->cShape->circle.getOutlineColor();
			sf::Color outlineColor(currOutline.r, currOutline.g,
				currOutline.b, opacity);
			e->cShape->circle.setOutlineColor(outlineColor);
		}

		// draw entity's circle
		m_window.draw(e->cShape->circle);
	}

	// Render score
	std::string scoreStr = "Score: " + std::to_string(m_score);
	m_text.setString(scoreStr);
	m_window.draw(m_text);

	// When paused, show instructions and game controls in middle of the screen
	if (m_paused) {
		std::string controlsStr =
			"Press W to move Up\nPress S to move Down\nPress A to move Left\nPress D to move Right\nPress ESC to Play/Pause\nPress Q to Exit\nClick LEFT MOUSE BUTTON to shoot bullet\nClick RIGHT MOUSE BUTTON to use special weapon";
		auto controlsText = sf::Text(controlsStr, m_font,
			m_text.getCharacterSize());

		// Centering the text
		sf::FloatRect textRect = controlsText.getLocalBounds();
		controlsText.setOrigin(textRect.left + textRect.width / 2.0f,
			textRect.top + textRect.height / 2.0f);
		const auto& winWidth = m_window.getSize().x;
		const auto& winHeight = m_window.getSize().y;
		controlsText.setPosition(winWidth / 2.0f, winHeight / 2.0f);

		m_window.draw(controlsText);
	}

	m_window.display();
}

void Game::sEnemySpawner()
{
	// spawn enemy only if certain amount of frames have passed (Spawn Interval)
	if (m_currentFrame - m_lastEnemySpawnFrame < m_enemyConfig.SI)
		return;
	this->spawnEnemy();
}

void Game::sCollision()
{
	auto& playerPos = m_player->cTransform->position;
	const auto& pCR = m_player->cCollision->collisionRadius;
	const auto& winWidth = m_window.getSize().x;
	const auto& winHeight = m_window.getSize().y;

	// Player bounds check
	// Collision is only checked once per frame, so if else if works
	if (playerPos.y < pCR) {
		// Top collision
		playerPos.y = pCR;
	}
	else if (playerPos.y + pCR > winHeight) {
		// Bottom collision
		playerPos.y = winHeight - pCR;
	}
	else if (playerPos.x < pCR) {
		// Left collision
		playerPos.x = pCR;
	}
	else if (playerPos.x + pCR > winWidth) {
		// Right collision
		playerPos.x = winWidth - pCR;
	}

	// Enemies bounce off boundaries
	for (auto e : m_entityManager.getEntities("enemy")) {
		const auto& enemyPos = e->cTransform->position;
		auto& enemyVel = e->cTransform->velocity;
		const auto& eCR = e->cCollision->collisionRadius;

		if (enemyPos.y < eCR) {
			// Top collision
			enemyVel.y *= -1.0f;
		}
		else if (enemyPos.y + eCR > winHeight) {
			// Bottom collision
			enemyVel.y *= -1.0f;
		}
		else if (enemyPos.x < eCR) {
			// Left collision
			enemyVel.x *= -1.0f;
		}
		else if (enemyPos.x + eCR > winWidth) {
			// Right collision
			enemyVel.x *= -1.0f;
		}
	}

	// Bullet collision with enemy
	// Spawn smaller enemies when enemy dies
	for (auto b : m_entityManager.getEntities("bullet")) {
		for (auto e : m_entityManager.getEntities("enemy")) {
			const auto& enemyPos = e->cTransform->position;
			const auto& eCR = e->cCollision->collisionRadius;
			const auto& bulletPos = b->cTransform->position;
			const auto& bCR = b->cCollision->collisionRadius;

			// destroy bullet and enemy
			if (glm::distance(bulletPos,enemyPos) < bCR + eCR) {
				e->destroy();
				b->destroy();
				spawnSmallEnemies(e);
				m_score += e->cScore->score;
			}
		}
	}

	// Bullet collision with small enemy
	// Doesn't spawn smaller enemies when it dies
	for (auto b : m_entityManager.getEntities("bullet")) {
		for (auto e : m_entityManager.getEntities("small enemy")) {
			const auto& enemyPos = e->cTransform->position;
			const auto& eCR = e->cCollision->collisionRadius;
			const auto& bulletPos = b->cTransform->position;
			const auto& bCR = b->cCollision->collisionRadius;

			// destroy bullet and enemy
			if (glm::distance(bulletPos,enemyPos) < bCR + eCR) {
				e->destroy();
				b->destroy();
				m_score += e->cScore->score;
			}
		}
	}

	// Player collision with enemies
	// If player dies, new spawns at the middle of the screen
	for (const auto e : m_entityManager.getEntities("enemy")) {
		const glm::vec2& enemyPos = e->cTransform->position;
		const float& eCR = e->cCollision->collisionRadius;

		// Current player died, spawn a new one
		if (glm::distance(playerPos,enemyPos) < pCR + eCR) {
			m_player->destroy();
			e->destroy();
			spawnPlayer();
			auto& points = e->cScore->score;
			m_score = m_score < points ? 0 : m_score - points;
			return;
		}
	}

	// Player collision with small enemies
	// If player dies, new spawns at the middle of the screen
	for (const auto e : m_entityManager.getEntities("small enemy")) {
		const glm::vec2& enemyPos = e->cTransform->position;
		const float& eCR = e->cCollision->collisionRadius;

		// Current player died, spawn a new one
		if (glm::distance(playerPos,enemyPos) < pCR + eCR) {
			m_player->destroy();
			e->destroy();
			spawnPlayer();
			auto& points = e->cScore->score;
			m_score = m_score < points ? 0 : m_score - points;
			return;
		}
	}
}

void Game::sLifespan()
{
	for (const auto e : m_entityManager.getEntities()) {
		if (e->cLifespan && e->cLifespan->remaining-- <= 0) {
			e->cLifespan->remaining = 0;
			e->destroy();
		}
	}
}