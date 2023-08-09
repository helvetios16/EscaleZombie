#include <SFML/Graphics.hpp>
#include <memory>
#include "game.h"
int main() {
    std::srand(static_cast<unsigned>(time(0)));

    std::unique_ptr<Games> game = std::make_unique<Games>();
    game->run();

    return 0;
}