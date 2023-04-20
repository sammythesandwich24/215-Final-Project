#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <cmath>
#include <stdio.h>


// define constants
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int BLOCK_WIDTH = 40;
const int BLOCK_HEIGHT = 20;
const int WINDOW_BORDER = 20;
const int NUM_BLOCKS_PER_ROW = 14;
const int NUM_ROWS = 6;
const float BALL_RADIUS = 10.f;
const float PADDLE_WIDTH = 80.f;
const float PADDLE_HEIGHT = 10.f;
const float PADDLE_SPEED = 0.55f;


class GameManager {
private:
	RenderWindow* window;
	sfp::PhysicsCircle* ball;
	sfp::PhysicsRectangle* paddle;
	sfp::PhysicsRectangle* blocks[NUM_ROWS][NUM_BLOCKS_PER_ROW];
	sfp::World* world;
	Clock* clock;

	sfp::PhysicsRectangle* leftBorder;
	sfp::PhysicsRectangle* rightBorder;
	sfp::PhysicsRectangle* topBorder;
	sfp::PhysicsRectangle* bottomBorder;

public:

	void setup() {

		ball->setFillColor(Color::White);
		ball->setCenter(Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
		paddle->setFillColor(Color::Green);
		paddle->setCenter(Vector2f(WINDOW_WIDTH / 2.f - PADDLE_WIDTH / 2.f, WINDOW_HEIGHT - 50.f));
		for (int i = 0; i < NUM_ROWS; i++) {
			for (int j = 0; j < NUM_BLOCKS_PER_ROW; j++) {
				blocks[i][j]->setSize(Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
				blocks[i][j]->setFillColor(Color::Yellow);
				blocks[i][j]->setCenter(Vector2f(j * BLOCK_WIDTH + 10.f, i * BLOCK_HEIGHT + 50.f));
				blocks[i][j]->setStatic(true);
				world->AddPhysicsBody(*blocks[i][j]);
			}
		}

		
		leftBorder->setCenter(Vector2f(WINDOW_BORDER / 2, 480 / 2));
		leftBorder->setSize(Vector2f(WINDOW_BORDER, 480));
		leftBorder->setStatic(true);
		world->AddPhysicsBody(*leftBorder);
		rightBorder->setCenter(Vector2f(640 - (WINDOW_BORDER / 2), 480 / 2));
		rightBorder->setSize(Vector2f(WINDOW_BORDER, 480));
		rightBorder->setStatic(true);
		world->AddPhysicsBody(*rightBorder);
		topBorder->setCenter(Vector2f(320, WINDOW_BORDER / 2));
		topBorder->setSize(Vector2f(640, WINDOW_BORDER));
		topBorder->setStatic(true);
		world->AddPhysicsBody(*topBorder);
		world->AddPhysicsBody(*bottomBorder);
		bottomBorder->setCenter(Vector2f(320, 478));
		bottomBorder->setSize(Vector2f(640, 5));
		bottomBorder->setStatic(true);




	}


	void paddleCheck() {
		// move paddle
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			paddle->setCenter(Vector2f(paddle->getBounds().getPosition().x - PADDLE_SPEED, paddle->getBounds().getPosition().y));
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			paddle->setCenter(Vector2f(paddle->getBounds().getPosition().x + PADDLE_SPEED, paddle->getBounds().getPosition().y));
		}

		// prevent paddle from going out of bounds
		if (paddle->getBounds().getPosition().x - PADDLE_WIDTH / 2 < 0.f) {
			paddle->getBounds().setPosition(Vector2f(0.f + PADDLE_WIDTH / 2, paddle->getBounds().getPosition().y));
		}
		if (paddle->getBounds().getPosition().x + PADDLE_WIDTH / 2 > WINDOW_WIDTH) {
			paddle->getBounds().setPosition(Vector2f(WINDOW_WIDTH - PADDLE_WIDTH / 2, paddle->getBounds().getPosition().y));
		}
	}

	void handleEvents() {
		// handle events
		Event event;
		while (window->pollEvent(event)) {
			if (event.type == Event::Closed) {
				window->close();
			}
		}
	}
	void gameLoop() {
		Time l = clock->getElapsedTime();
		while (window->isOpen()) {
			handleEvents();
			paddleCheck();
			Time currentTime(clock->getElapsedTime());
			Time deltaTime(currentTime - l);
			int deltaTimeMS(deltaTime.asMilliseconds());

			if (deltaTimeMS > 0) {
				world->UpdatePhysics(deltaTimeMS);
				l = currentTime;
			}
			//ballCollisionCheck();
			//blockCollisionCheck();
			repaint();
		}

	}



	void repaint() {
		window->clear(Color::Black);
		// draw objects
		window->draw(*ball);
		window->draw(*paddle);
		for (int i = 0; i < NUM_ROWS; i++) {
			for (int j = 0; j < NUM_BLOCKS_PER_ROW; j++) {
				window->draw(*blocks[i][j]);
			}
		}

		// display window
		window->display();
	}
	GameManager()
	{
		window = new RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sammy's Breakout");
		ball = new sfp::PhysicsCircle();
		ball->setSize(Vector2f(BALL_RADIUS, BALL_RADIUS));
		ball->setCenter(Vector2f(400, 500));
		ball->applyImpulse(Vector2f(0.05, -0.4));
		paddle = new sfp::PhysicsRectangle();
		paddle->setSize(Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT * 2));
		paddle->setStatic(true);
		world = new sfp::World(Vector2f(0, 0));
		clock = new Clock();
		world->AddPhysicsBody(*ball);
		world->AddPhysicsBody(*paddle);



		leftBorder = new sfp::PhysicsRectangle();
		rightBorder = new sfp::PhysicsRectangle();
		topBorder = new sfp::PhysicsRectangle();
		bottomBorder = new sfp::PhysicsRectangle();

		for (int i = 0; i < NUM_ROWS; i++) {
			for (int j = 0; j < NUM_BLOCKS_PER_ROW; j++) {
				blocks[i][j] = new sfp::PhysicsRectangle();

			}
		}

	}
	~GameManager() {
		delete window;
		delete ball;
		delete paddle;
		delete clock;
		delete world;
		delete leftBorder;
		delete rightBorder;
		delete topBorder;
		delete bottomBorder;
		for (int i = 0; i < NUM_ROWS; i++) {
			for (int j = 0; j < NUM_BLOCKS_PER_ROW; j++) {
				delete blocks[i][j];
			}
		}
	}

};

int main()
{
	GameManager manager;
	manager.setup();
	manager.gameLoop();
	return 0;
}