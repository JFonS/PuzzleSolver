#include <iostream>
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>

using namespace std;

//Globals
sf::Image inputImg;
sf::Sprite inputSprite;
sf::Texture inputTex;
sf::Color bgColor;
float bgColorThreshold = 0.25f;

float windowWidth = 900.0f, windowHeight = 900.0f;
//

bool sameColors(sf::Color &color1, sf::Color &color2, float threshold)
{
    float maxDistance = threshold * 255.0f;
    return abs(color1.r - color2.r) + abs(color1.g - color2.g) + abs(color1.b - color2.b) < maxDistance;
}

void printColor(sf::Color &color)
{
    cout << color.r << ", " << color.g << ", " << color.b << endl;
}

void paintBackground()
{
    for(int x = 0; x < inputImg.getSize().x; ++x)
    {
        for(int y = 0; y < inputImg.getSize().y; ++y)
        {
            sf::Color pixelColor = inputImg.getPixel(x, y);
            if( sameColors(pixelColor, bgColor, bgColorThreshold) )
            {
                inputImg.setPixel(x, y, sf::Color(255, 0, 0, 255));
            }
        }
    }

    inputTex.loadFromImage(inputImg);
    inputSprite.setTexture(inputTex);
}

void error(string err)
{
    cerr << err << endl;
    exit(-1);
}

int main(int argc, char* argv[])
{
    if (argc < 2) error("Missing input file.");

    bgColor = sf::Color(88, 180, 100, 255);

    string inputFile = argv[1];
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "PuzzleSolver");

    if (!inputImg.loadFromFile(inputFile)) error("Can't open input file");

    inputTex.loadFromImage(inputImg);

    inputSprite.setTexture(inputTex);
    inputSprite.setOrigin(0.0f, 0.0f);
    inputSprite.setScale(windowWidth / inputImg.getSize().x, windowHeight / inputImg.getSize().y);

    cout << windowWidth / inputImg.getSize().x << "  " << windowHeight / inputImg.getSize().y << endl;
    paintBackground();

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
