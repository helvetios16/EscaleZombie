#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "horde.h"
#include "walls.h"
class bullet {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float velocity = 2.0f;
    bool discharge = false;
    std::shared_ptr<sf::RenderWindow> window = nullptr;
    sf::Vector2f position;
    Horder* horder = nullptr;
public:
    bullet() {
        if (!texture.loadFromFile("resources/bala1.png")){
            std::cout << "Error: Failed to load bullet texture!" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setScale(0.5f, 0.5f);
        
    }

    void shot(const sf::Vector2f& position_player) {
        discharge = true;
        position = position_player;
        position.x += 20.0f;
        position.y += 20.0f;
        sprite.setPosition(position);
    }

    void movement() {
        if (discharge) {
            sprite.move(velocity, 0.0f);
            if (sprite.getPosition().x > window->getSize().x) {
                discharge = false;
            }
        }
    }

    void draw() {
        if (discharge) {
            window->draw(sprite);
        }
    }

    void set_window(std::shared_ptr<sf::RenderWindow> window) {
        this->window = window;
    }
    void handleCollision(const std::vector<Wall<sf::Color>>& walls) {
        for (const auto& wall : walls) {
            sf::FloatRect bulletBounds = sprite.getGlobalBounds();
            sf::FloatRect wallBounds = wall.getShape().getGlobalBounds();
            if (bulletBounds.intersects(wallBounds)) {
                discharge = false;
                sprite.setPosition(-100.0f, -100.0f);
                return; // Salir de la función después de colisionar con una pared
            }
        }
        for(const auto& zombie : horder->getHorderAll()){
            sf::FloatRect bulletBounds = sprite.getGlobalBounds();
            sf::FloatRect zombieBounds = zombie->getGlobal();
            if(bulletBounds.intersects(zombieBounds)){
                zombie->recibirdamage(1);
                discharge = false;
                sprite.setPosition(-100.0f, -100.0f);
                return;
            }
        }
    }

    bool is_discharged() const {
        return discharge;
    }
    void reset() {
        discharge = false;
        sprite.setPosition(-100.0f, -100.0f); 
    }
    sf::Vector2f getPosition() const {
        return position;
    }
    static void removeInactiveBullets(std::vector<bullet>& bullets, std::shared_ptr<sf::RenderWindow> window) {
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&window](const bullet& b) {
            bool remove = b.is_discharged() && (b.getPosition().x > window->getSize().x || b.getPosition().x < 0.0f || b.getPosition().y > window->getSize().y || b.getPosition().y < 0.0f);
            if (remove) {
                window = nullptr; // Liberar el puntero inteligente antes de eliminar la bala
            }
            return remove;
        }), bullets.end());
    }
    void setHorder(Horder* horderPtr) {
        horder = horderPtr;
    }
};