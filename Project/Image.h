#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <SFML/Graphics.hpp>

using namespace std;

class Image
{
private:

    float BackgroundColorThreshold;
    sf::Color BackgroundOriginalColor, BorderColor;
    unsigned char DiscardAlpha; //The alpha of a pixel whichs been discarded

    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    int imageWidth, imageHeight;

    bool isBorderPixel(int x, int y);

public:

    Image(string filename, int windowWidth, int windowHeight);

    void DiscardBackground();
    void GetPieces();

    //Utils
    bool IsDiscarded(const sf::Color &color) { return color.a == DiscardAlpha; }
    bool IsInsideImage(const sf::Vector2i &pos) { return pos.x >= 0 && pos.x < imageWidth && pos.y >= 0 && pos.y < imageHeight; }
    bool EqualColors(const sf::Color &color1, const sf::Color &color2, float threshold)
    {
        float maxDistance = threshold * 255.0f;
        return abs(color1.r - color2.r) + abs(color1.g - color2.g) + abs(color1.b - color2.b) < maxDistance;
    }
    //

    float GetWidth() { return imageWidth; }
    float GetHeight() { return imageHeight; }

    //Getters
    sf::Sprite* GetSprite();

    //Debug
    void PrintColor(const sf::Color &color) { cout << color.r << ", " << color.g << ", " << color.b << endl;}
};

#endif // IMAGE_H
