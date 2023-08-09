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

    sf::Clock clock;

    void init_window();

    Horder horde;

    //-----------------------implementacion inicio de juego ------------------------------
    sf::Texture fondoJuego;
    bool showGif = true;
    sf::Sprite fondoJuegoSp;
    sf::Texture staticImageTexture;
    sf::Sprite staticImageSprite;
    //Seleccion de jugadores
    sf::RectangleShape button_1Player;
    bool showImage1P = false;
    sf::RectangleShape button_2Player;
    bool showImage2P = false;
    //Estado de juego
    bool showMainMenu = true; //Inicio
    bool showPlayerSelect = false; //Seleccion
    bool gameStarted = false; //Comenzo
    //-----------------------------------------------------

    bool one = true;

    //implementacion singleton
    static Games* instance;

public:
    Games();
    ~Games() = default;

    void poll_events();
    void update();
    void render();
    void run();

    static Games* getInstance(){
        // Método para acceder a la instancia única
        if (instance == nullptr) {
            instance = new Games();
        }
        return instance;
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
    //imagen 1 de inicio
    fondoJuego.loadFromFile("resources/Frame1.png");
    fondoJuegoSp.setTexture(fondoJuego);
    fondoJuegoSp.setScale(2,2);
    //imagen 2 de inicio
    staticImageTexture.loadFromFile("resources/selectPlayer.png");
    staticImageSprite.setTexture(staticImageTexture);
    staticImageSprite.setScale(2,2);
    //botones (rectangulos) para seleccionar
    button_1Player.setSize(sf::Vector2f(644.0f,490.0f));
    button_1Player.setPosition(92.0f,320.0f);
    button_1Player.setFillColor(sf::Color::Red);
    button_2Player.setSize(sf::Vector2f(644.0f,490.0f));
    button_2Player.setPosition(862.0f,320.0f);
    button_2Player.setFillColor(sf::Color::Blue);
    //----------------------------------------
}

void Games::poll_events() {
    while (this->window->pollEvent(this->event_sfml)) {
        if (this->event_sfml.type == sf::Event::Closed) {
            this->window->close();
        } 
        if (this->event_sfml.type == sf::Event::KeyPressed) {
            if (this->event_sfml.key.code == sf::Keyboard::Escape) {
                this->window->close();
            } else if (this->event_sfml.key.code == sf::Keyboard::Q) {
                player_one_shooting = true;
            } else if (this->event_sfml.key.code == sf::Keyboard::Enter) {
                player_two_shooting = true;
            } else if (this->event_sfml.key.code == sf::Keyboard::Space) {
                if (showMainMenu) {
                    showMainMenu = false;
                    showPlayerSelect = true;
                } else if (showPlayerSelect) {
                   showPlayerSelect = false;
               }
            }

        } else if (this->event_sfml.type == sf::Event::KeyReleased) {
            if (this->event_sfml.key.code == sf::Keyboard::Q) {
                player_one_shooting = false;
            } else if (this->event_sfml.key.code == sf::Keyboard::Enter) {
                player_two_shooting = false;
            } 

        } else if (!gameStarted) {
            if (this->event_sfml.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePosition(this->event_sfml.mouseButton.x, this->event_sfml.mouseButton.y);
                if (button_1Player.getGlobalBounds().contains(mousePosition)) {
                    showImage1P = true;  //determina si es 1 jugador
                    showImage2P = false;
                    gameStarted = true;
                } else if (button_2Player.getGlobalBounds().contains(mousePosition)) {
                    showImage1P = false;
                    showImage2P = true;  //determina si son 2 jugadores
                    gameStarted = true;
                }
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
    //aqui crea el fondo y todos los personajes
    if ( showMainMenu ) {
            this->window->draw(fondoJuegoSp);
       }
       if ( showPlayerSelect ) {
            this->window->draw(staticImageSprite);
            //this->window->draw(button_1Player);
            //this->window->draw(button_2Player);
       }
       if ( gameStarted ) {
            this->window->draw(background_one_sprite); //mapa
            player_one->draw(window);

            for (auto& b : bullets) { b.draw(); }
            horde.call(window,1,5);
            horde.approximate(*player_one->getSprite(),1,dt);
            horde.approximate(*player_two->getSprite(),1,dt);
            horde.collision(walls,1,dt);

            if (showImage2P) {
                player_two->draw(window);
            }
       }
    
    if(horde.endGame(*player_one->getSprite()) || horde.endGame(*player_two->getSprite())){
        std::cout<<"End Game"<<std::endl;
        this->window->close();
    }
    int n = horde.getCodex();
    std::cout<<n<<std::endl;
    if(n == 50){
        std::cout<<"You win, you will can past the hordes of zombies"<<std::endl; 
        this->window->close();
    }

    this->window->display();
}


void Games::run() {
    while (this->window->isOpen()) {
        this->update();
        this->render();
    }
}
