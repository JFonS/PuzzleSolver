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

    Image originalImage(inputFilename, windowWidth, windowHeight);
    Image image(inputFilename, windowWidth, windowHeight);
    image.DiscardBackground();
    image.GetPiecesBounds();

    image.RefinePieces();

    sf::Vector2i lastMouseCoord(0,0);

    sf::Sprite *sprite = image.GetSprite();
    sf::Sprite *originalSprite = originalImage.GetSprite();
    sprite->setPosition(0,0);
    originalSprite->setPosition(0, 0);


    sf::View viewLeft(sf::FloatRect(0, 0, window.getSize().x/2, window.getSize().y));
    viewLeft.setViewport(sf::FloatRect(0, 0, 0.5, 1));
    sf::View viewRight(sf::FloatRect(0, 0, window.getSize().x/2, window.getSize().y));
    viewRight.setViewport(sf::FloatRect(0.5, 0, 0.5, 1));
    
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
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                float zoom = -0.05*float(event.mouseWheelScroll.delta) + 1;
                cout << event.mouseWheelScroll.delta << ", " << zoom << endl;
                viewLeft.zoom(zoom);
                viewRight.zoom(zoom);
            }
        }
        window.clear();


        //sprite->setScale(float(windowWidth) / (image.GetWidth()), float(windowWidth) / (image.GetWidth()));
        //sprite->setScale(float(windowWidth) / (image.GetWidth() * 2), float(windowHeight) / image.GetHeight());
        window.setView(viewLeft);
        sprite->setScale(1, 1);
        window.draw(*sprite);

        window.setView(viewRight);
        originalSprite->setScale(1, 1);
        window.draw(*originalSprite);

        window.display();
    }

    return 0;
}
