#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>
#include "zombie.h"
#include "walls.h"
class Horder {
private:
    std::vector<std::unique_ptr<Zombie>> horder_all;
    sf::Clock clock;
    sf::Time time;
    int index1, index2, index3, index4, index5;
    bool i1 = true;
    bool i2 = false;
    bool i3 = false;
    bool i4 = false;
    bool i5 = false;
public:
    Horder();
    ~Horder() = default;
    void adder_zombie(std::unique_ptr<Zombie> zombie, std::vector<std::unique_ptr<Zombie>>& horder);
    void create_zombies(std::vector<std::unique_ptr<Zombie>>& horder, int maxZombies);
    void draw(std::shared_ptr<sf::RenderWindow>& window, int h, int maxZombies);
    void approximate(sf::Sprite& sprite, int h, float deltaTime);
    void collision(const std::vector<Wall<sf::Color>>& shapes,int h, float deltaTime, int index);
    const std::vector<std::unique_ptr<Zombie>>& getHorderAll()  { return horder_all; }
};
Horder::Horder() : index1(0), index2(0), index3(0), index4(0), index5(0) {
    this->time = sf::seconds(0.45f);
}
void Horder::adder_zombie(std::unique_ptr<Zombie> zombie, std::vector<std::unique_ptr<Zombie>>& horder) {
    horder.push_back(std::move(zombie));
}
void Horder::create_zombies(std::vector<std::unique_ptr<Zombie>>& horder, int maxZombies) {
    if(i1){
        std::unique_ptr<Zombie> zombie = std::make_unique<NormalZombie>();
        this->adder_zombie(std::move(zombie),horder_all);
        index1++;
    } else if(index1 == maxZombies){
        i1 = false; i2 = true;
    }
    if(i2){
        if(index2 < maxZombies/2+1){
            std::unique_ptr<Zombie> zn = std::make_unique<NormalZombie>();
            this->adder_zombie(std::move(zn),horder_all);
            index2++;
        } else{
            std::unique_ptr<Zombie> zf = std::make_unique<FastZombie>();
            this->adder_zombie(std::move(zf),horder_all);
            index2++;
        }
    } else if(index2 == maxZombies){
        i2 = false; i3 = true;
    }
    if(i3){
        if(index3 < maxZombies/3+1){
            std::unique_ptr<Zombie> zn = std::make_unique<NormalZombie>();
            this->adder_zombie(std::move(zn),horder_all);
            index3++;
        } else{
            std::unique_ptr<Zombie> zf = std::make_unique<FastZombie>();
            this->adder_zombie(std::move(zf),horder_all);
            index3++;
        }
    } else if(index3 == maxZombies){
        i3 = false; i4 = true;
    }
    if(i4){
        if(index4 < maxZombies/3+1){
            std::unique_ptr<Zombie> zn = std::make_unique<NormalZombie>();
            this->adder_zombie(std::move(zn),horder_all);
            index4++;
        } else if(index4 < maxZombies/3 && index4 < maxZombies/2){
            std::unique_ptr<Zombie> zf = std::make_unique<FastZombie>();
            this->adder_zombie(std::move(zf),horder_all);
            index4++;
        } else {
            std::unique_ptr<Zombie> zs = std::make_unique<StrongZombie>();
            this->adder_zombie(std::move(zs),horder_all);
            index4++;
        }
    } else if(index4 == maxZombies){
        i4 = false; i5 = true;
    }
    if(i5){
        if(index5 < maxZombies/2){
            std::unique_ptr<Zombie> zn = std::make_unique<FastZombie>();
            this->adder_zombie(std::move(zn),horder_all);
            index5++;
        } else{
            std::unique_ptr<Zombie> zf = std::make_unique<StrongZombie>();
            this->adder_zombie(std::move(zf),horder_all);
            index5++;
        }
    } else if(index5 == maxZombies){
        i5 = false; 
    }
}
void Horder::draw(std::shared_ptr<sf::RenderWindow>& window,int h, int maxZombies) {
    int index = 0;
    if(h == 1) { index = index1; }
        else if (h == 2) { index = index2; }
        else if (h == 3) { index = index3; }
        else if (h == 4) { index = index4; }
        else if (h == 5) { index = index5; }
    if (index <= maxZombies && clock.getElapsedTime() >= time) {
        if(h == 1) { create_zombies(horder_all, maxZombies); index1++; }
        else if (h == 2) { create_zombies(horder_all, maxZombies); index2++; }
        else if (h == 3) { create_zombies(horder_all, maxZombies); index3++; }
        else if (h == 4) { create_zombies(horder_all, maxZombies); index4++; }
        else if (h == 5) { create_zombies(horder_all, maxZombies); index5++; }
        clock.restart();
    }
    for (const auto& zombie : horder_all) { 
        zombie->drawing(window); 
    } 
}

void Horder::approximate(sf::Sprite& sprite, int h, float deltaTime) {
    sf::Vector2f targetPosition = sprite.getPosition();
    for (const auto& zombie : horder_all) { 
        zombie->set_position(targetPosition, deltaTime); 
    }
}

void Horder::collision(const std::vector<Wall<sf::Color>>& shapes, int h, float deltaTime, int index) {
    for (auto it = horder_all.begin(); it != horder_all.end();) {
        
        for (auto it2 = horder_all.begin(); it2 != horder_all.end(); ++it2) {
            for (const auto& wall : shapes) {
                (*it)->collision(wall.getShape(), deltaTime);
            }
            (*it)->stops(*it2, deltaTime);
        }
        if ((*it)->getVida() <= 0) {
            it = horder_all.erase(it);
        } else {
            ++it;
        }
    }
}