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
    int index1 = 0, index2 = 0, index3 = 0, index4 = 0, index5 = 0;
    int dex1 = 0, dex2 = 0, dex3 = 0, dex4 = 0, dex5 = 0;
    bool i1 = true;
    bool i2 = false;
    bool i3 = false;
    bool i4 = false;
    bool i5 = false;
    bool one = true;
    bool two = false;
    bool three = false;
    bool four = false;
    bool five = false;
    int codex = 0;
public:
    Horder();
    ~Horder() = default;
    void adder_zombie(std::unique_ptr<Zombie> zombie, std::vector<std::unique_ptr<Zombie>>& horder);
    void create_zombies(std::vector<std::unique_ptr<Zombie>>& horder, int maxZombies, int h);
    void draw(std::shared_ptr<sf::RenderWindow>& window, int h, int maxZombies);
    void approximate(sf::Sprite& sprite, int h, float deltaTime);
    void collision(const std::vector<Wall<sf::Color>>& shapes, int h, float deltaTime);
    const std::vector<std::unique_ptr<Zombie>>& getHorderAll()  { return horder_all; }
    int getCurrentIndex(int h) const {
        if (h == 1) { return index1; }
        else if (h == 2) { return index2; }
        else if (h == 3) { return index3; }
        else if (h == 4) { return index4; }
        else if (h == 5) { return index5; }
        return -1; // Valor por defecto si el argumento no es válido
    }
    bool endGame(sf::Sprite& sprite) const {
        for (const auto& zombie : horder_all) {
            if(zombie->end(sprite)) {return true; }
        }
        return false;
    }
    void call(std::shared_ptr<sf::RenderWindow>& window, int h, int maxZombies);
    int getCodex() const {
        return codex;
    }
};
Horder::Horder() : index1(1), index2(1), index3(1), index4(1), index5(1) {
    this->time = sf::seconds(0.45f);
}
void Horder::adder_zombie(std::unique_ptr<Zombie> zombie, std::vector<std::unique_ptr<Zombie>>& horder) {
    horder.push_back(std::move(zombie));
}
void Horder::create_zombies(std::vector<std::unique_ptr<Zombie>>& horder, int maxZombies, int h) {
    if(i1 || h==1){
        std::unique_ptr<Zombie> zombie = std::make_unique<NormalZombie>();
        this->adder_zombie(std::move(zombie),horder_all);
        index1++;
    } else if(index1 == maxZombies){
        i1 = false; i2 = true;
    }
    if(i2 || h==2){
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
    if(i3 || h==3){
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
    if(i4 || h==4){
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
    if(i5 || h==5){
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

void Horder::call(std::shared_ptr<sf::RenderWindow>& window, int h, int maxZombies){
    maxZombies = 5;
    int modifiedH = h; 
    if (dex1 == maxZombies) {
        modifiedH = 2;
        one = false;
        two = true;
    }
    if(dex2==maxZombies){
        modifiedH=3;
        two = false;
        three = true;
    }
    if(dex3==maxZombies){
        modifiedH=4;
        three = false;
        four = true;
    }
    if(dex4==maxZombies){
        modifiedH=5;
        four = false;
        five = true;
    }
    if(modifiedH==1 && one){
        draw(window,1,maxZombies);
    } else if(modifiedH==2 && two){
        draw(window,2,maxZombies);
    } else if (modifiedH==3 && three){
        draw(window,3,maxZombies);
    } else if(modifiedH==4 && four){
        draw(window,4,maxZombies);
    } else if(modifiedH==5 && five){
        draw(window,5,maxZombies);
    }
}

void Horder::draw(std::shared_ptr<sf::RenderWindow>& window, int h, int maxZombies) {
    int index = 0;    
    if (h == 1) { index = index1; }
    else if (h == 2) { index = index2; }
    else if (h == 3) { index = index3; }
    else if (h == 4) { index = index4; }
    else if (h == 5) { index = index5; }
    
    // Generar zombis si se cumplen las condiciones
    if (h == 1) {
        if (index <= maxZombies && clock.getElapsedTime() >= time) {
            create_zombies(horder_all, maxZombies, 1);
            clock.restart();
            std::cout << "1" << getCurrentIndex(1) << std::endl;
        }
    } else if (h == 2) {
        if (index <= maxZombies && clock.getElapsedTime() >= time) {
            create_zombies(horder_all, maxZombies, 2);
            clock.restart();
            std::cout <<"2"<< getCurrentIndex(2) << std::endl;
        }
    } else if (h == 3) {
        if (index <= maxZombies && clock.getElapsedTime() >= time) {
            create_zombies(horder_all, maxZombies, 3);
            clock.restart();
            std::cout <<"3"<< getCurrentIndex(3) << std::endl;
        }
    } else if (h == 4) {
        if (index <= maxZombies && clock.getElapsedTime() >= time) {
            create_zombies(horder_all, maxZombies, 4);
            clock.restart();
            std::cout <<"4"<< getCurrentIndex(4) << std::endl;
        }
    } else if (h == 5) {
        if (index <= maxZombies && clock.getElapsedTime() >= time) {
            create_zombies(horder_all, maxZombies, 5);
            clock.restart();
            std::cout <<"5"<< getCurrentIndex(5) << std::endl;
        }
    }
    
    // Dibujar los zombis en la ventana
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

void Horder::collision(const std::vector<Wall<sf::Color>>& shapes, int h, float deltaTime) {
    int hol = 5;
    if(dex1 ==hol) {
        h=2;
    } 
    if(dex2==hol){
        h=3;
    }
    if(dex3==hol){
        h=4;
    }
    if(dex4==hol){
        h=5;
    }
    for (auto it = horder_all.begin(); it != horder_all.end();) {
        for (auto it2 = horder_all.begin(); it2 != horder_all.end(); ++it2) {
            for (const auto& wall : shapes) {
                (*it)->collision(wall.getShape(), deltaTime);
            }
            (*it)->stops(*it2, deltaTime);
        }
        if ((*it)->getVida() <= 0) {
            if(h == 1) { dex1++; std::cout<<dex1<<std::endl;}//index1-=1; }
            else if (h == 2) { dex2++; std::cout<<dex2<<std::endl; }
            else if (h == 3) { dex3++; std::cout<<dex3<<std::endl;}
            else if (h == 4) { dex4++; std::cout<<dex4<<std::endl; }
            else if (h == 5) { dex5++; std::cout<<dex5<<std::endl; }
            it = horder_all.erase(it);
            codex++;
        } else {
            ++it;
        }
    }
}
