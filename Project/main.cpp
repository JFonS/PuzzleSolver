#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include "Image.h"
#include <SFML/Graphics.hpp>

using namespace std;

int windowWidth = 1800, windowHeight = 900;

int main(int argc, char* argv[])
{

    srand(time(0));
    if (argc < 2) { cerr << "Missing input file." << endl; exit(-1); }

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Puzzle Solver");
    string inputFilename = argv[1];

    Image *originalImage = new Image(inputFilename, windowWidth, windowHeight);
    Image *image = new Image(inputFilename, windowWidth, windowHeight);
    image->DiscardBackground();
    image->GetPiecesBounds();

    image->RefinePieces();

    sf::Vector2i lastMouseCoord(0,0);

    sf::Sprite *sprite = image->GetSprite();
    sf::Sprite *originalSprite = originalImage->GetSprite();
    sprite->setPosition(0,0);
    originalSprite->setPosition(image->GetWidth(), 0);

    bool moving = false;
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                moving = true;
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                moving = false;
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                if (moving)
                {
                    float dx = event.mouseMove.x - lastMouseCoord.x;
                    float dy = event.mouseMove.y - lastMouseCoord.y;
                    sprite->move(dx,dy);
                    originalSprite->move(dx, dy);
                }
                lastMouseCoord = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
            }
        }
        window.clear();

        //sprite->setScale(float(windowWidth) / (image->GetWidth()), float(windowWidth) / (image->GetWidth()));
        //sprite->setScale(float(windowWidth) / (image->GetWidth() * 2), float(windowHeight) / image->GetHeight());
        sprite->setScale(1, 1);
        window.draw(*sprite);

        originalSprite->setScale(1, 1);
        window.draw(*originalSprite);

        window.display();
    }

    return 0;
}
