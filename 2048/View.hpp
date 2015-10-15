#ifndef VIEW_HPP
#define VIEW_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class View {
  public: 
    View();
    ~View();
    void render(sf::RenderWindow&);

    sf::Color getColor(int);
    sf::Vector2f getPos(int);

    sf::RectangleShape* blocks[16];
    sf::RectangleShape* newBlocks[16];
    sf::Text* texts[16];
    sf::Text* newTexts[16];

  private:
    sf::RectangleShape* frame;
    sf::RectangleShape* grids[16];
};

const float GRID_SIZE = 60.0f;
const float SPACE_SIZE = 10.0f;
const float FRAME_POS = 20.0f;
const float FRAME_SIZE = GRID_SIZE * 4 + SPACE_SIZE * 5;

const int GRID_NUMBER = 16;
const int FONT_SIZE = 20;

const sf::Color FRAME_COLOR = sf::Color(186, 173, 159);
const sf::Color GRID_COLOR = sf::Color(204, 192, 178);

#endif