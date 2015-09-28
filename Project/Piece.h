#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <queue>
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

using namespace std;

class Image;

struct Coord
{
    int x, y;
};

class Piece
{

private:

    static float RefinedBackgroundThreshold;
    static unsigned char RefinedBackgroundDiscardAlpha;

    vector<Coord> refinedPixelsCoords;

    sf::IntRect bounds; //Bounding box of the piece
    int size; //The number of pixels of the piece

    void RefineBackground(sf::Image *inputImage, sf::Image *&maskedImagePointer);

public:

    static int MinSizeToBeAPiece;

    Piece();

    void RefinePiece(sf::Image *inputImage);

    void SetBounds(sf::Image *inputImage, const sf::IntRect &bounds)
    {
        int boundExtension = 45;

        this->bounds.left = max(0, bounds.left - boundExtension);
        this->bounds.top  = max(0, bounds.top - boundExtension);

        this->bounds.width = min(int(inputImage->getSize().x), bounds.left + bounds.width + boundExtension*2) - this->bounds.left;
        this->bounds.height = min(int(inputImage->getSize().y), bounds.top + bounds.height + boundExtension*2) - this->bounds.top;

        cout << this->bounds.left << ", " << this->bounds.top << ", " << this->bounds.width << ", " << this->bounds.height << endl;
    }

    void SetSize(int size) { this->size = size; }

    bool IsInsideBounds(sf::Vector2i coords) { return coords.x >= bounds.left && coords.x < bounds.left + bounds.width &&
                                                      coords.y >= bounds.top && coords.y < bounds.top + bounds.height; }

    sf::IntRect *GetBounds() { return &bounds; }
    int GetSize() { return size; }
};

#endif // PIECE_H
