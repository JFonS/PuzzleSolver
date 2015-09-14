#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

void error(string err) {
    cerr << err << endl;
    exit(-1);
}

int main(int argc, char* argv[])
{
    if (argc < 2) error("Missing input file.");

    string inputFile = argv[1];
    sf::RenderWindow window(sf::VideoMode(200, 200), "PuzzleSolver");

    sf::Image inputImg;
    if (!inputImg.loadFromFile(inputFile)) error("Can't open input file");
    
    sf::Texture inputTex;
    inputTex.loadFromImage(inputImg);

    sf::Sprite inputSprite;
    inputSprite.setTexture(inputTex);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(inputSprite); 
        window.display();
    }

    return 0;
}
