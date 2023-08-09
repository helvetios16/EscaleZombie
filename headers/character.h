#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include "walls.h"
class character_prototype {
    public:
        virtual std::unique_ptr<character_prototype> clone() const = 0;
        virtual void moviment(float deltaTime) = 0;
        virtual void draw(std::shared_ptr<sf::RenderWindow>& window) = 0;
        virtual void updateAnimation(float deltaTime) = 0;
        virtual void handleCollision(const std::vector<Wall<sf::Color>>& walls) = 0;
        virtual sf::Vector2f getPosition() const = 0; 
        virtual sf::Sprite* getSprite() = 0;
        virtual ~character_prototype() {}
};

class character_one : public character_prototype {
    private:
        sf::Vector2f position;
        sf::Sprite sprite;
        float speed = 100.0f;

        sf::Texture spriteSheet; // Hoja de sprites
        sf::IntRect spriteRect; 
    public:
        virtual std::unique_ptr<character_prototype> clone() const override;
        character_one(sf::Vector2f position, sf::Texture& sheetTexture);
        void moviment(float deltaTime) override;
        void draw(std::shared_ptr<sf::RenderWindow>& window) override;
        void updateAnimation(float deltaTime) override;
        void handleCollision(const std::vector<Wall<sf::Color>>& walls) override;
        sf::Vector2f getPosition() const override;
        sf::Sprite* getSprite() override {
            return &sprite;
        }
};

std::unique_ptr<character_prototype> character_one::clone() const {
    return std::make_unique<character_one>(*this);
}
character_one::character_one(sf::Vector2f position, sf::Texture& sheetTexture)
           : position(position), spriteSheet(sheetTexture)  {
       // Ajusta cuadroAncho y cuadroAlto según tus necesidades
       sprite.setTexture(spriteSheet);
       sprite.setTextureRect(sf::IntRect(0, 0, 47, 50));
       sprite.setPosition(position);
   }

void character_one::moviment(float deltaTime){
    sf::Vector2f movement(0.0f, 0.0f);
       if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
           int xTexture = 0;
           xTexture = (int)sprite.getPosition().y/10 % 4;
           xTexture = xTexture*47;
           sprite.setTextureRect(sf::IntRect(xTexture, 162, 47, 50));
           sprite.move(0,-5);
          
           movement.y -= speed * deltaTime;
       }
       else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
           int xTexture = 0;
           xTexture = (int)sprite.getPosition().y/10 % 4;
           xTexture = xTexture*47;
           sprite.setTextureRect(sf::IntRect(xTexture, 0, 47, 50));
           sprite.move(0,5);
          
           movement.y += speed * deltaTime;
       }
       else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
           int xTexture = 0;
           xTexture = (int)sprite.getPosition().x/10 % 4;
           xTexture = xTexture*47;
           sprite.setTextureRect(sf::IntRect(xTexture, 54, 47, 50));
           sprite.move(-5,0);
          
           movement.x -= speed * deltaTime;
       }
       else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
           int xTexture = 0;
           xTexture = (int)sprite.getPosition().x/10 % 4;
           xTexture = xTexture*47;
           sprite.setTextureRect(sf::IntRect(xTexture, 108, 47, 50));
           sprite.move(5,0);


           movement.x += speed * deltaTime;
       }
      
       sf::Vector2f newPosition = position + movement;
       position = newPosition;

       // Actualizar posición del sprite
       sprite.setPosition(position);

}
void character_one::draw(std::shared_ptr<sf::RenderWindow>& window){
    window->draw(this->sprite);
}

//-------------------
void character_one::updateAnimation(float deltaTime) {
       spriteRect.left = (spriteRect.left + 47) % spriteSheet.getSize().x;
       sprite.setTextureRect(spriteRect);


       // Actualizar posición del sprite
       position.x += 10 * deltaTime;
       sprite.setPosition(position);
   }

//------------------
void character_one::handleCollision(const std::vector<Wall<sf::Color>>& walls) {
    for (const auto& wall : walls) {
        sf::FloatRect playerBounds = sprite.getGlobalBounds();
        sf::FloatRect wallBounds = wall.getShape().getGlobalBounds();
        if (playerBounds.intersects(wallBounds)) {
            sf::FloatRect intersection;
            if (playerBounds.intersects(wallBounds, intersection)) {
                if (intersection.width < intersection.height) {
                    if (playerBounds.left < wallBounds.left) {
                        position.x -= intersection.width;
                    } else {
                        position.x += intersection.width;
                    }
                } else {
                    if (playerBounds.top < wallBounds.top) {
                        position.y -= intersection.height;
                    } else {
                        position.y += intersection.height;
                    }
                }
            }
        }
    }
    sprite.setPosition(position);
}
sf::Vector2f character_one::getPosition() const {
    return position;
}

class character_two : public character_prototype {
    private:
        sf::Vector2f position;
        sf::Sprite sprite;
        float speed = 100.0f;

        sf::Texture spriteSheet; // Hoja de sprites
        sf::IntRect spriteRect;
    public:
        virtual std::unique_ptr<character_prototype> clone() const override;
        character_two(sf::Vector2f position, sf::Texture& texture);
        void moviment(float deltaTime) override;
        void draw(std::shared_ptr<sf::RenderWindow>& window) override;
        void updateAnimation(float deltaTime) override;
        void handleCollision(const std::vector<Wall<sf::Color>>& walls) override;
        sf::Vector2f getPosition() const override;
        sf::Sprite* getSprite() override {
            return &sprite;
        }
};

std::unique_ptr<character_prototype> character_two::clone() const {
    return std::make_unique<character_two>(*this);
}
character_two::character_two(sf::Vector2f position, sf::Texture& sheetTexture)
           : position(position), spriteSheet(sheetTexture)  {
           spriteRect = sf::IntRect(0, 0, 47, 50); // Ajusta cuadroAncho y cuadroAlto según tus necesidades
           sprite.setTexture(spriteSheet);
           sprite.setTextureRect(spriteRect);
           sprite.setPosition(position);
       }

void character_two::moviment(float deltaTime){
    sf::Vector2f movement(0.0f, 0.0f);
           if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
               int xTexture = 0;
               xTexture = (int)sprite.getPosition().y/10 % 4;
               xTexture = xTexture*47;
               sprite.setTextureRect(sf::IntRect(xTexture, 162, 47, 50));
               sprite.move(0,-5);
              
               movement.y -= speed * deltaTime;
           }
           else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
               int xTexture = 0;
               xTexture = (int)sprite.getPosition().y/10 % 4;
               xTexture = xTexture*47;
               sprite.setTextureRect(sf::IntRect(xTexture, 0, 47, 50));
               sprite.move(0,5);
              
               movement.y += speed * deltaTime;
           }
           else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
               int xTexture = 0;
               xTexture = (int)sprite.getPosition().x/10 % 4;
               xTexture = xTexture*47;
               sprite.setTextureRect(sf::IntRect(xTexture, 54, 47, 50));
               sprite.move(-5,0);
              
               movement.x -= speed * deltaTime;
           }
           else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
               int xTexture = 0;
               xTexture = (int)sprite.getPosition().x/10 % 4;
               xTexture = xTexture*47;
               sprite.setTextureRect(sf::IntRect(xTexture, 108, 47, 50));
               sprite.move(5,0);

               movement.x += speed * deltaTime;
           }

           sf::Vector2f newPosition = position + movement;
           position = newPosition;

           // Actualizar posición del sprite
           sprite.setPosition(position);

}
void character_two::draw(std::shared_ptr<sf::RenderWindow>& window){
    window->draw(this->sprite);
}
void character_two::updateAnimation(float deltaTime) {
           spriteRect.left = (spriteRect.left + 47) % spriteSheet.getSize().x;
           sprite.setTextureRect(spriteRect);


           // Actualizar posición del sprite
           position.x += 10 * deltaTime;
           sprite.setPosition(position);
       }

void character_two::handleCollision(const std::vector<Wall<sf::Color>>& walls) {
    for (const auto& wall : walls) {
        sf::FloatRect playerBounds = sprite.getGlobalBounds();
        sf::FloatRect wallBounds = wall.getShape().getGlobalBounds();
        if (playerBounds.intersects(wallBounds)) {
            sf::FloatRect intersection;
            if (playerBounds.intersects(wallBounds, intersection)) {
                if (intersection.width < intersection.height) {
                    if (playerBounds.left < wallBounds.left) {
                        position.x -= intersection.width;
                    } else {
                        position.x += intersection.width;
                    }
                } else {
                    if (playerBounds.top < wallBounds.top) {
                        position.y -= intersection.height;
                    } else {
                        position.y += intersection.height;
                    }
                }
            }
        }
    }
    sprite.setPosition(position);
}
sf::Vector2f character_two::getPosition() const {
    return position;
}
class character_factory {
    private:
        std::unordered_map<std::string, std::unique_ptr<character_prototype>> prototypes;
        sf::Texture texture_for_one;
        sf::Texture texture_for_two;
    public:
        character_factory();
        void register_prototype(const std::string& type, std::unique_ptr<character_prototype> prototype);
        std::unique_ptr<character_prototype> create_character(const std::string& type);
        ~character_factory() = default;
};
character_factory::character_factory(){
    texture_for_one.loadFromFile("resources/imgP1.png");
    texture_for_two.loadFromFile("resources/imgP1.png");
    prototypes["Personaje1"] = std::make_unique<character_one>(sf::Vector2f(580.0f, 760.0f),texture_for_one);
    prototypes["Personaje2"] = std::make_unique<character_two>(sf::Vector2f(540.0f, 760.0f),texture_for_two);
}
void character_factory::register_prototype(const std::string& type, std::unique_ptr<character_prototype> prototype){
    prototypes[type] = std::move(prototype);
}
std::unique_ptr<character_prototype> character_factory::create_character(const std::string& type){
    auto it = prototypes.find(type);
    if(it!=prototypes.end()){
        return it->second->clone();
    }
    return nullptr;
}