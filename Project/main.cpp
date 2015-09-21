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
    image->GetPieces();
    image->RefinePieces();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        sf::Sprite *sprite = image->GetSprite();
        sprite->setScale(float(windowWidth) / (image->GetWidth() * 2), float(windowHeight) / image->GetHeight());
        window.draw(*sprite);

        sf::Sprite *originalSprite = originalImage->GetSprite();
        originalSprite->setScale(float(windowWidth) / (originalImage->GetWidth() * 2), float(windowHeight) / originalImage->GetHeight());
        originalSprite->setPosition(windowWidth/2, 0);
        window.draw(*originalSprite);

        window.display();
    }

    return 0;
}
