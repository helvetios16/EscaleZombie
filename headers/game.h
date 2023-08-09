#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <string>
#include "zombie.h"
#include "horde.h"
#include "character.h"
#include "bullet.h"
#include "walls.h"

class Games {
private:
    sf::VideoMode video_mode;
    std::shared_ptr<sf::RenderWindow> window;
    sf::Event event_sfml;

    sf::RectangleShape rectangle;

    sf::RectangleShape shape;
    float movement = 200.f;

    sf::RectangleShape object;
    sf::RectangleShape object1;

    sf::Texture background_one_texture;
    sf::Sprite background_one_sprite;
    std::vector<Wall<sf::Color>> walls;
    std::unique_ptr<character_prototype> player_one;
    std::unique_ptr<character_prototype> player_two;
    character_factory factory;
    sf::Sprite* sprite1;
    sf::Sprite* sprite2;
    std::vector<bullet> bullets;

    bool player_one_shooting = false;
    bool player_two_shooting = false;
    sf::Clock player_one_shoot_clock;
    sf::Clock player_two_shoot_clock;

    std::vector<sf::Texture> gitFrames;
    sf::Texture frame;
    int current;
    bool showgif = true;

    sf::Texture staticImageTexture;
    sf::Sprite staticImageSprite;

    //Para que funcione la seleccion de la numero de jugadores
    sf::RectangleShape button_1Player;
    bool showImage1P = true;
    sf::RectangleShape button_2Player;
    bool showImage2P = true;
    sf::Sprite spriteInImg;
    sf::Time elapsed;
    int currentFrame = 0;
    const sf::Time frameInterval = sf::seconds(0.3f);

    sf::Clock clock;

    void init_window();

    Horder horde;

public:
    Games();
    ~Games() = default;

    void poll_events();
    void update();
    void render();
    void run();

    void move(float dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            shape.move(-movement * dt, 0.f);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            shape.move(movement * dt, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            shape.move(0.f, -movement * dt);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            shape.move(0.f, movement * dt);
    }
};

void Games::init_window() {
    this->video_mode = sf::VideoMode(1600, 900);
    this->window = std::make_shared<sf::RenderWindow>(this->video_mode, "Escale Zomboid", sf::Style::Close | sf::Style::Titlebar);
    this->window->setFramerateLimit(45);
}

Games::Games() : horde() {
    this->init_window();

    background_one_texture.loadFromFile("resources/mapa1.png");
    background_one_sprite.setTexture(background_one_texture);

    walls.emplace_back(sf::Vector2f(450.0f, 550.0f), sf::Vector2f(0.0f, 350.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(90.0f, 350.0f), sf::Vector2f(0.0f, 0.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(440.0f, 150.0f), sf::Vector2f(90.0f, 0.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(275.0f, 150.0f), sf::Vector2f(550.0f, 0.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(150.0f, 150.0f), sf::Vector2f(1100.0f, 0.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(352.0f, 460.0f), sf::Vector2f(1248.0f, 0.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(340.0f, 490.0f), sf::Vector2f(1260.0f, 20.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(580.0f, 40.0f), sf::Vector2f(620.0f, 580.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(88.0f, 40.0f), sf::Vector2f(450.0f, 580.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(88.0f, 40.0f), sf::Vector2f(450.0f, 860.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(782.0f, 40.0f), sf::Vector2f(610.0f, 860.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(100.0f, 40.0f), sf::Vector2f(1474.0f, 860.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(32.0f, 800.0f), sf::Vector2f(1570.0f, 400.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(190.0f, 600.0f), sf::Vector2f(1000.0f, 600.0f), sf::Color::Blue, 2.f);
    walls.emplace_back(sf::Vector2f(460.0f, 130.0f), sf::Vector2f(614.0f, 220.0f), sf::Color::Blue, 2.f);

    for (int i = 0; i < 100; ++i) {
        bullets.emplace_back();
        bullets.back().set_window(window);
    }
    for (auto& bullet : bullets) {
        bullet.setHorder(&horde);
    }

    player_one = factory.create_character("Personaje1");
    player_two = factory.create_character("Personaje2");

    //--------------------------------------------
    staticImageTexture.loadFromFile("resources/selectPlayer1.png");
    staticImageSprite.setTexture(staticImageTexture);

    for(int i=1;i<=4;i++){
        frame.loadFromFile("resources/FrameI" + std::to_string(i) + ".png");
        gitFrames.push_back(frame);
    }

    button_1Player.setSize(sf::Vector2f(644.0f, 490.0f));
    button_1Player.setPosition(92.0f, 320.0f);
    button_1Player.setFillColor(sf::Color::Green);

    button_2Player.setSize(sf::Vector2f(644.0f, 490.0f));
    button_2Player.setPosition(862.0f, 320.0f);
    button_2Player.setFillColor(sf::Color::Green);
    //----------------------------------------
}

void Games::poll_events() {
    while (this->window->pollEvent(this->event_sfml)) {
        if (this->event_sfml.type == sf::Event::Closed) {
            this->window->close();
        } 
        if (this->event_sfml.type == sf::Event::MouseButtonPressed && this->event_sfml.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = this->window->mapPixelToCoords(sf::Mouse::getPosition(*(this->window)));

            if (button_1Player.getGlobalBounds().contains(mousePos)) {
                showImage1P = true;
            } else if (button_2Player.getGlobalBounds().contains(mousePos)) {
                showImage2P = true;
            }
        }


        else if (this->event_sfml.type == sf::Event::KeyPressed) {
            if (this->event_sfml.key.code == sf::Keyboard::Escape) {
                this->window->close();
            } else if (this->event_sfml.key.code == sf::Keyboard::Q) {
                player_one_shooting = true;
            } else if (this->event_sfml.key.code == sf::Keyboard::Enter) {
                player_two_shooting = true;
            } else if (this->event_sfml.key.code == sf::Keyboard::Space) {
                   showgif = !showgif;
            }

        } else if (this->event_sfml.type == sf::Event::KeyReleased) {
            if (this->event_sfml.key.code == sf::Keyboard::Q) {
                player_one_shooting = false;
            } else if (this->event_sfml.key.code == sf::Keyboard::Enter) {
                player_two_shooting = false;
            }
        }
    }
}

void Games::update() {
    this->poll_events();
    sf::Time elapsedTime = player_one_shoot_clock.getElapsedTime();
    if (player_one_shooting && elapsedTime.asSeconds() >= 0.45f) {
        for (auto& b : bullets) {
            if (!b.is_discharged()) {
                b.shot(player_one->getPosition());
                player_one_shoot_clock.restart();
                break;
            }
        }
    }
    elapsedTime = player_two_shoot_clock.getElapsedTime();
    if (player_two_shooting && elapsedTime.asSeconds() >= 0.45f) {
        for (auto& b : bullets) {
            if (!b.is_discharged()) {
                b.shot(player_two->getPosition());
                player_two_shoot_clock.restart();
                break;
            }
        }
    }
    for (auto& b : bullets) {
        b.movement();
        b.handleCollision(walls);
        b.draw();
    }
    bullet::removeInactiveBullets(bullets, window);

    //------------------------------------------
    // Actualizar el temporizador y cambiar de cuadro
    elapsed = clock.getElapsedTime();
    if (elapsed >= frameInterval) {
        currentFrame = (currentFrame + 1)  % gitFrames.size();
        clock.restart();
    }
     //-----------------------------------
}



void Games::render() {
    sf::Time deltaTime = clock.restart();
    float dt = deltaTime.asSeconds();

    player_one->moviment(dt);
    player_two->moviment(dt);

    player_one->handleCollision(walls);
    player_two->handleCollision(walls);
    
    this->window->clear();

    //----------------------------
    //aqui deberia crear el fondo y todos los personajes
       
       if (showImage1P || showImage2P) {
            this->window->draw(background_one_sprite);
            //horder.draw_one(window);
            player_one->draw(window);
            if (showImage2P) {
                player_two->draw(window);
                //balaP2->draw(window);
            }
       }

       //aqui decide si dibujar el gif o la img para seleccionar
       if (showgif) {
           spriteInImg.setTexture(gitFrames[currentFrame]);
           this->window->draw(spriteInImg);
       } else {
           this->window->draw(staticImageSprite);
       }

    //-----------------------------------

    this->window->draw(background_one_sprite);

    player_one->draw(window);
    player_two->draw(window);

    /*for(const auto& b : walls){
        window->draw(b.getShape());
    }*/
    
    for (auto& b : bullets) {
        b.draw();
    }
    
    horde.draw(window,2,25);
    horde.approximate(*player_one->getSprite(),1,dt);
    horde.collision(walls,1,dt,20);

    this->window->display();
}


void Games::run() {
    while (this->window->isOpen()) {
        this->update();
        this->render();
    }
}