#include <SFML/Graphics.hpp>
#include <memory>
#include "game.h"

Games* Games::instance = nullptr;

int main() {
    std::srand(static_cast<unsigned>(time(0)));
    Games* instance1 = Games::getInstance();

    //std::unique_ptr<Games> game = std::make_unique<Games>();
    instance1->run();

    return 0;
}
