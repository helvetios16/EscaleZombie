#pragma once
#include <SFML/Graphics.hpp>
#include <type_traits>
template<typename T>
struct is_sfml_color : std::false_type {};

template<>
struct is_sfml_color<sf::Color> : std::true_type {};

template <typename ColorType, typename = std::enable_if_t<is_sfml_color<ColorType>::value>>
class Wall {
    private:
        sf::RectangleShape shape;
    public:
        Wall(sf::Vector2f size, sf::Vector2f position, ColorType outlineColor, float outlineThickness) {
            shape.setOutlineColor(outlineColor);
            shape.setOutlineThickness(outlineThickness);
            shape.setFillColor(sf::Color::Transparent);
            shape.setSize(size);
            shape.setPosition(position);
        }

        const sf::RectangleShape& getShape() const {
            return shape;
        }
};