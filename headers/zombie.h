#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <cmath>
class Zombie {
    protected:
        sf::Texture texture;
        sf::Sprite sprite;
        float velocity;
        bool speed;
        sf::Vector2f lastPosition;
        sf::FloatRect nextPosition;

    public:
        virtual void load() = 0;
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual void drawing(std::shared_ptr<sf::RenderWindow>& window) = 0;
        virtual void uptade(float deltaTime) = 0;
        virtual sf::Vector2f get_position() = 0;
        virtual void set_position(const sf::Vector2f& shape,float deltaTime) = 0;
        virtual void set_last(float deltaTime) = 0;
        virtual void collision(const sf::RectangleShape& shape,float deltaTime) = 0;
        virtual void stops(std::unique_ptr<Zombie>& zombie, float deltaTime) = 0;
        virtual sf::FloatRect getGlobal() const = 0;
        virtual void recibirdamage(int dam) = 0;
        virtual int getVida() = 0;
};

class NormalZombie : public Zombie {
    private:
        int vida;
    public:
        NormalZombie();
        ~NormalZombie() = default;
        void load() override;
        void draw(sf::RenderWindow& window);
        void drawing(std::shared_ptr<sf::RenderWindow>& window) override{
            window->draw(this->sprite);
        }
        void uptade(float deltaTime) override;
        sf::Vector2f get_position() override;
        void set_position(const sf::Vector2f& shape, float deltaTime) override;
        void set_last(float deltaTime) override;
        void collision(const sf::RectangleShape& shape,float deltaTime) override;
        void stops(std::unique_ptr<Zombie>& zombie, float deltaTime) override;
        sf::FloatRect getGlobal() const;
        void recibirdamage(int dam) override;
        int getVida() override;
};

NormalZombie::NormalZombie() {
    this->velocity = 40.f;
    this->load();
    this->sprite.setTexture(this->texture);
    float minX = 1395.f; float maxX = 1435.f;
    float minY = 840.f;  float maxY = 880.f;
    float randomX = static_cast<float>(rand())/(RAND_MAX + 1.f)*(maxX-minX)+minX;
    float rangomY = static_cast<float>(rand())/(RAND_MAX + 1.f)*(maxY-minY)+minY;
    this->sprite.setPosition(randomX,rangomY);
    this->sprite.setScale(1.f,1.f);
    this->speed = true;
    this->lastPosition.x = 0;
    this->lastPosition.y = 0;
    this->vida = 1;
}
void NormalZombie::load() {
    if(!this->texture.loadFromFile("resources/Zombie1_Izquierda_Caminando1.png")){
        std::cout<<"Error: Imagen Normal Zombie";
    }
}
void NormalZombie::draw(sf::RenderWindow& window) {
    window.draw(this->sprite);
}
void NormalZombie::uptade(float deltaTime) {
    sf::Vector2f currentPosition = sprite.getPosition();
    sf::Vector2f newPosition = currentPosition + sf::Vector2f(0.0f, -velocity * deltaTime);
    this->sprite.setPosition(newPosition);
}

sf::Vector2f NormalZombie::get_position() {
    return this->sprite.getPosition();
}

void NormalZombie::set_position(const sf::Vector2f& shape, float deltaTime) {
    sf::Vector2f newPosition = this->sprite.getPosition();
    float distance = std::sqrt(std::pow(shape.x - newPosition.x, 2) + std::pow(shape.y - newPosition.y, 2));
    if (distance <= 400) {
        this->velocity = 35;
        float displacement = velocity * deltaTime;
        newPosition.y += (newPosition.y > shape.y) ? -displacement : displacement;
        newPosition.x += (newPosition.x > shape.x) ? -displacement : displacement;
        lastPosition.x = (newPosition.x > shape.x) ? -1.f : 1.f;
        lastPosition.y = (newPosition.y > shape.y) ? -1.f : 1.f;
        this->speed = false;
        sprite.setPosition(newPosition);
    } else {
        if(speed)
            this->uptade(deltaTime);
        else 
            this->set_last(deltaTime);
    }
}

void NormalZombie::set_last(float deltaTime) {
    if (lastPosition.x != 0.f || lastPosition.y != 0.f) {
        sf::Vector2f newPosition = sprite.getPosition();
        sf::Vector2f displacement = lastPosition * (velocity * deltaTime);

        newPosition += displacement;
        sprite.setPosition(newPosition);
    }
}

void NormalZombie::collision(const sf::RectangleShape& shape, float deltaTime) {
    sf::Vector2f runner;
    runner.x = 0.f;
    runner.y = 0.f;
    const sf::FloatRect playerBounds = sprite.getGlobalBounds();
    const sf::FloatRect shapeBounds = shape.getGlobalBounds();
    nextPosition = playerBounds;
    nextPosition.left += runner.x * deltaTime;
    nextPosition.top  += runner.y * deltaTime;
    if(shape.getGlobalBounds().intersects(nextPosition)){
        if (playerBounds.top < shapeBounds.top && playerBounds.top + playerBounds.height < shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {
            
            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top - playerBounds.height);
        }
        else if (playerBounds.top > shapeBounds.top && playerBounds.top + playerBounds.height > shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {
            
            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top + shapeBounds.height);
        }
        else if (playerBounds.left < shapeBounds.left && playerBounds.left + playerBounds.width < shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {
            
            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left - playerBounds.width, playerBounds.top);
        }
        else if (playerBounds.left > shapeBounds.left && playerBounds.left + playerBounds.width > shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {
            
            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left + shapeBounds.width, playerBounds.top);
        }
    }
    sprite.move(runner * deltaTime);
}

void NormalZombie::stops(std::unique_ptr<Zombie>& shape, float deltaTime) {
    sf::Vector2f runner;
    runner.x = 0.f;
    runner.y = 0.f;
    const sf::FloatRect playerBounds = sprite.getGlobalBounds();
    const sf::FloatRect shapeBounds = shape->getGlobal();
    nextPosition = playerBounds;
    nextPosition.left += runner.x * deltaTime;
    nextPosition.top += runner.y * deltaTime;
    if (shapeBounds.intersects(nextPosition)) {
        if (playerBounds.top < shapeBounds.top && playerBounds.top + playerBounds.height < shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {

            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top - playerBounds.height);
        }
        else if (playerBounds.top > shapeBounds.top && playerBounds.top + playerBounds.height > shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {

            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top + shapeBounds.height);
        }
        else if (playerBounds.left < shapeBounds.left && playerBounds.left + playerBounds.width < shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {

            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left - playerBounds.width, playerBounds.top);
        }
        else if (playerBounds.left > shapeBounds.left && playerBounds.left + playerBounds.width > shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {

            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left + shapeBounds.width, playerBounds.top);
        }
    }
    sprite.move(runner * deltaTime);
}

sf::FloatRect NormalZombie::getGlobal() const {
    return sprite.getGlobalBounds();
}
void NormalZombie::recibirdamage(int dam){
    this->vida -= dam;
    if(this->vida <= 0){
        this->vida = 0;
    }
}
int NormalZombie::getVida(){
    return vida;
}

class FastZombie : public Zombie {
    private:
        int vida;
    public:
        FastZombie();
        ~FastZombie() = default;
        void load() override;
        void draw(sf::RenderWindow& window);
        void drawing(std::shared_ptr<sf::RenderWindow>& window) override{
            window->draw(this->sprite);
        }
        void uptade(float deltaTime) override;
        sf::Vector2f get_position() override;
        void set_position(const sf::Vector2f& shape, float deltaTime) override;
        void set_last(float deltaTime) override;
        void collision(const sf::RectangleShape& shape,float deltaTime) override;
        void stops(std::unique_ptr<Zombie>& zombie, float deltaTime) override;
        sf::FloatRect getGlobal() const;
        void recibirdamage(int dam) override;
        int getVida() override;
};

FastZombie::FastZombie() {
    this->velocity = 60.f;
    this->load();
    this->sprite.setTexture(this->texture);
    float minX = 1395.f; float maxX = 1435.f;
    float minY = 840.f;  float maxY = 880.f;
    float randomX = static_cast<float>(rand())/(RAND_MAX + 1.f)*(maxX-minX)+minX;
    float rangomY = static_cast<float>(rand())/(RAND_MAX + 1.f)*(maxY-minY)+minY;
    this->sprite.setPosition(randomX,rangomY);
    this->sprite.setScale(1.f,1.f);
    this->speed = true;
    this->lastPosition.x = 0;
    this->lastPosition.y = 0;
    this->vida = 1;
}
void FastZombie::load() {
    if(!this->texture.loadFromFile("resources/Zombie1_Izquierda_Caminando1.png")){
        std::cout<<"Error: Imagen Normal Zombie";
    }
}
void FastZombie::draw(sf::RenderWindow& window) {
    window.draw(this->sprite);
}
void FastZombie::uptade(float deltaTime) {
    sf::Vector2f currentPosition = sprite.getPosition();
    sf::Vector2f newPosition = currentPosition + sf::Vector2f(0.0f, -velocity * deltaTime);
    this->sprite.setPosition(newPosition);
}

sf::Vector2f FastZombie::get_position() {
    return this->sprite.getPosition();
}

void FastZombie::set_position(const sf::Vector2f& shape, float deltaTime) {
    sf::Vector2f newPosition = this->sprite.getPosition();
    float distance = std::sqrt(std::pow(shape.x - newPosition.x, 2) + std::pow(shape.y - newPosition.y, 2));
    if (distance <= 400) {
        this->velocity = 55;
        float displacement = velocity * deltaTime;
        newPosition.y += (newPosition.y > shape.y) ? -displacement : displacement;
        newPosition.x += (newPosition.x > shape.x) ? -displacement : displacement;
        lastPosition.x = (newPosition.x > shape.x) ? -1.f : 1.f;
        lastPosition.y = (newPosition.y > shape.y) ? -1.f : 1.f;
        this->speed = false;
        sprite.setPosition(newPosition);
    } else {
        if(speed)
            this->uptade(deltaTime);
        else 
            this->set_last(deltaTime);
    }
}

void FastZombie::set_last(float deltaTime) {
    if (lastPosition.x != 0.f || lastPosition.y != 0.f) {
        sf::Vector2f newPosition = sprite.getPosition();
        sf::Vector2f displacement = lastPosition * (velocity * deltaTime);

        newPosition += displacement;
        sprite.setPosition(newPosition);
    }
}

void FastZombie::collision(const sf::RectangleShape& shape, float deltaTime) {
    sf::Vector2f runner;
    runner.x = 0.f;
    runner.y = 0.f;
    const sf::FloatRect playerBounds = sprite.getGlobalBounds();
    const sf::FloatRect shapeBounds = shape.getGlobalBounds();
    nextPosition = playerBounds;
    nextPosition.left += runner.x * deltaTime;
    nextPosition.top  += runner.y * deltaTime;
    if(shape.getGlobalBounds().intersects(nextPosition)){
        if (playerBounds.top < shapeBounds.top && playerBounds.top + playerBounds.height < shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {
            
            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top - playerBounds.height);
        }
        else if (playerBounds.top > shapeBounds.top && playerBounds.top + playerBounds.height > shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {
            
            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top + shapeBounds.height);
        }
        else if (playerBounds.left < shapeBounds.left && playerBounds.left + playerBounds.width < shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {
            
            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left - playerBounds.width, playerBounds.top);
        }
        else if (playerBounds.left > shapeBounds.left && playerBounds.left + playerBounds.width > shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {
            
            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left + shapeBounds.width, playerBounds.top);
        }
    }
    sprite.move(runner * deltaTime);
}

void FastZombie::stops(std::unique_ptr<Zombie>& shape, float deltaTime) {
    sf::Vector2f runner;
    runner.x = 0.f;
    runner.y = 0.f;
    const sf::FloatRect playerBounds = sprite.getGlobalBounds();
    const sf::FloatRect shapeBounds = shape->getGlobal();
    nextPosition = playerBounds;
    nextPosition.left += runner.x * deltaTime;
    nextPosition.top += runner.y * deltaTime;
    if (shapeBounds.intersects(nextPosition)) {
        if (playerBounds.top < shapeBounds.top && playerBounds.top + playerBounds.height < shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {

            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top - playerBounds.height);
        }
        else if (playerBounds.top > shapeBounds.top && playerBounds.top + playerBounds.height > shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {

            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top + shapeBounds.height);
        }
        else if (playerBounds.left < shapeBounds.left && playerBounds.left + playerBounds.width < shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {

            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left - playerBounds.width, playerBounds.top);
        }
        else if (playerBounds.left > shapeBounds.left && playerBounds.left + playerBounds.width > shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {

            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left + shapeBounds.width, playerBounds.top);
        }
    }
    sprite.move(runner * deltaTime);
}

sf::FloatRect FastZombie::getGlobal() const {
    return sprite.getGlobalBounds();
}
void FastZombie::recibirdamage(int dam){
    this->vida -= dam;
    if(this->vida <= 0){
        this->vida = 0;
    }
}
int FastZombie::getVida(){
    return vida;
}

class StrongZombie : public Zombie {
    private:
        int vida;
    public:
        StrongZombie();
        ~StrongZombie() = default;
        void load() override;
        void draw(sf::RenderWindow& window);
        void drawing(std::shared_ptr<sf::RenderWindow>& window) override{
            window->draw(this->sprite);
        }
        void uptade(float deltaTime) override;
        sf::Vector2f get_position() override;
        void set_position(const sf::Vector2f& shape, float deltaTime) override;
        void set_last(float deltaTime) override;
        void collision(const sf::RectangleShape& shape,float deltaTime) override;
        void stops(std::unique_ptr<Zombie>& zombie, float deltaTime) override;
        sf::FloatRect getGlobal() const;
        void recibirdamage(int dam) override;
        int getVida() override;
};

StrongZombie::StrongZombie() {
    this->velocity = 30.f;
    this->load();
    this->sprite.setTexture(this->texture);
    float minX = 1395.f; float maxX = 1435.f;
    float minY = 840.f;  float maxY = 880.f;
    float randomX = static_cast<float>(rand())/(RAND_MAX + 1.f)*(maxX-minX)+minX;
    float rangomY = static_cast<float>(rand())/(RAND_MAX + 1.f)*(maxY-minY)+minY;
    this->sprite.setPosition(randomX,rangomY);
    this->sprite.setScale(1.f,1.f);
    this->speed = true;
    this->lastPosition.x = 0;
    this->lastPosition.y = 0;
    this->vida = 3;
}
void StrongZombie::load() {
    if(!this->texture.loadFromFile("resources/Zombie1_Izquierda_Caminando1.png")){
        std::cout<<"Error: Imagen Normal Zombie";
    }
}
void StrongZombie::draw(sf::RenderWindow& window) {
    window.draw(this->sprite);
}
void StrongZombie::uptade(float deltaTime) {
    sf::Vector2f currentPosition = sprite.getPosition();
    sf::Vector2f newPosition = currentPosition + sf::Vector2f(0.0f, -velocity * deltaTime);
    this->sprite.setPosition(newPosition);
}

sf::Vector2f StrongZombie::get_position() {
    return this->sprite.getPosition();
}

void StrongZombie::set_position(const sf::Vector2f& shape, float deltaTime) {
    sf::Vector2f newPosition = this->sprite.getPosition();
    float distance = std::sqrt(std::pow(shape.x - newPosition.x, 2) + std::pow(shape.y - newPosition.y, 2));
    if (distance <= 400) {
        this->velocity = 25;
        float displacement = velocity * deltaTime;
        newPosition.y += (newPosition.y > shape.y) ? -displacement : displacement;
        newPosition.x += (newPosition.x > shape.x) ? -displacement : displacement;
        lastPosition.x = (newPosition.x > shape.x) ? -1.f : 1.f;
        lastPosition.y = (newPosition.y > shape.y) ? -1.f : 1.f;
        this->speed = false;
        sprite.setPosition(newPosition);
    } else {
        if(speed)
            this->uptade(deltaTime);
        else 
            this->set_last(deltaTime);
    }
}

void StrongZombie::set_last(float deltaTime) {
    if (lastPosition.x != 0.f || lastPosition.y != 0.f) {
        sf::Vector2f newPosition = sprite.getPosition();
        sf::Vector2f displacement = lastPosition * (velocity * deltaTime);

        newPosition += displacement;
        sprite.setPosition(newPosition);
    }
}

void StrongZombie::collision(const sf::RectangleShape& shape, float deltaTime) {
    sf::Vector2f runner;
    runner.x = 0.f;
    runner.y = 0.f;
    const sf::FloatRect playerBounds = sprite.getGlobalBounds();
    const sf::FloatRect shapeBounds = shape.getGlobalBounds();
    nextPosition = playerBounds;
    nextPosition.left += runner.x * deltaTime;
    nextPosition.top  += runner.y * deltaTime;
    if(shape.getGlobalBounds().intersects(nextPosition)){
        if (playerBounds.top < shapeBounds.top && playerBounds.top + playerBounds.height < shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {
            
            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top - playerBounds.height);
        }
        else if (playerBounds.top > shapeBounds.top && playerBounds.top + playerBounds.height > shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {
            
            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top + shapeBounds.height);
        }
        else if (playerBounds.left < shapeBounds.left && playerBounds.left + playerBounds.width < shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {
            
            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left - playerBounds.width, playerBounds.top);
        }
        else if (playerBounds.left > shapeBounds.left && playerBounds.left + playerBounds.width > shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {
            
            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left + shapeBounds.width, playerBounds.top);
        }
    }
    sprite.move(runner * deltaTime);
}

void StrongZombie::stops(std::unique_ptr<Zombie>& shape, float deltaTime) {
    sf::Vector2f runner;
    runner.x = 0.f;
    runner.y = 0.f;
    const sf::FloatRect playerBounds = sprite.getGlobalBounds();
    const sf::FloatRect shapeBounds = shape->getGlobal();
    nextPosition = playerBounds;
    nextPosition.left += runner.x * deltaTime;
    nextPosition.top += runner.y * deltaTime;
    if (shapeBounds.intersects(nextPosition)) {
        if (playerBounds.top < shapeBounds.top && playerBounds.top + playerBounds.height < shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {

            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top - playerBounds.height);
        }
        else if (playerBounds.top > shapeBounds.top && playerBounds.top + playerBounds.height > shapeBounds.top + shapeBounds.height && playerBounds.left < shapeBounds.left + shapeBounds.width && playerBounds.left + playerBounds.width > shapeBounds.left) {

            runner.y = 0.f;
            sprite.setPosition(playerBounds.left, shapeBounds.top + shapeBounds.height);
        }
        else if (playerBounds.left < shapeBounds.left && playerBounds.left + playerBounds.width < shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {

            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left - playerBounds.width, playerBounds.top);
        }
        else if (playerBounds.left > shapeBounds.left && playerBounds.left + playerBounds.width > shapeBounds.left + shapeBounds.width && playerBounds.top < shapeBounds.top + shapeBounds.height && playerBounds.top + playerBounds.height > shapeBounds.top) {

            runner.x = 0.f;
            sprite.setPosition(shapeBounds.left + shapeBounds.width, playerBounds.top);
        }
    }
    sprite.move(runner * deltaTime);
}

sf::FloatRect StrongZombie::getGlobal() const {
    return sprite.getGlobalBounds();
}
void StrongZombie::recibirdamage(int dam){
    this->vida -= dam;
    if(this->vida <= 0){
        this->vida = 0;
    }
}
int StrongZombie::getVida(){
    return vida;
}