#include "Piece.h"
#include "Image.h"

constexpr int Piece::MinSizeToBeAPiece = 5000;
constexpr float Piece::RefinedBackgroundThreshold = 0.15f;
constexpr unsigned char Piece::RefinedBackgroundDiscardAlpha = 1;

Piece::Piece()
{
}

void Piece::RefineBackground(sf::Image *inputImage, sf::Image *&maskedImagePointer)
{

    sf::Image *maskedImage = new sf::Image();
    sf::Uint8 *pixels = new sf::Uint8[bounds.width * bounds.height * 4];
    maskedImage->create(bounds.width, bounds.height, pixels);
    sf::IntRect copyRect(bounds.left, bounds.top, bounds.left + bounds.width, bounds.top + bounds.height);
    maskedImage->copy(*inputImage, 0,0, copyRect);

    //Get the background color
    int numPixelsInPiece = 0;
    sf::Vector3f refinedBackgroundColorV(.0f, .0f, .0f);

    for(int x = bounds.left; x < bounds.left + bounds.width; ++x)
    {
        for(int y = bounds.top; y < bounds.top + bounds.height; ++y)
        {
            sf::Color pixelColor = inputImage->getPixel(x,y);
            if( Image::IsDiscarded(pixelColor) )
            {
                ++numPixelsInPiece;
                refinedBackgroundColorV += sf::Vector3f(pixelColor.r, pixelColor.g, pixelColor.b);
            }
        }
    }
    refinedBackgroundColorV /= float(numPixelsInPiece);
    //

    refinedBackgroundColor = sf::Color(refinedBackgroundColorV.x, refinedBackgroundColorV.y, refinedBackgroundColorV.z);

    //BFS to discard background's pixels
    bool visited[bounds.width][bounds.height];
    for(int x = 0; x < bounds.width; ++x) for(int y = 0; y < bounds.height; ++y) visited[x][y] = false;

    for(int x = bounds.left; x < bounds.left + bounds.width; ++x)
    {
        for(int y = bounds.top; y < bounds.top + bounds.height; ++y)
        {
            if(visited[x - bounds.left][y - bounds.top]) continue;

            sf::Color currentColor = maskedImage->getPixel(x - bounds.left, y - bounds.top);
            if(!Image::EqualColors(refinedBackgroundColor, currentColor, RefinedBackgroundThreshold)) continue;

            queue<sf::Vector2i> toVisit;
            toVisit.push(sf::Vector2i(x, y));
            while(toVisit.size() > 0)
            {
                sf::Vector2i current = toVisit.front(); toVisit.pop();
                visited[current.x - bounds.left][current.y - bounds.top] = true;

                currentColor = maskedImage->getPixel(current.x - bounds.left, current.y - bounds.top);
                if( Image::EqualColors(currentColor, refinedBackgroundColor, RefinedBackgroundThreshold) )
                {
                    sf::Vector2i adj = sf::Vector2i(current.x-1, current.y);
                    if(IsInsideBounds(adj) && !visited[adj.x - bounds.left][adj.y - bounds.top])
                    {
                        toVisit.push(adj);
                        visited[adj.x - bounds.left][adj.y - bounds.top] = true;
                    }

                    adj = sf::Vector2i(current.x+1, current.y);
                    if(IsInsideBounds(adj) && !visited[adj.x - bounds.left][adj.y - bounds.top])
                    {
                        toVisit.push(adj);
                        visited[adj.x - bounds.left][adj.y - bounds.top] = true;
                    }

                    adj = sf::Vector2i(current.x, current.y+1);
                    if(IsInsideBounds(adj) && !visited[adj.x - bounds.left][adj.y - bounds.top])
                    {
                        toVisit.push(adj);
                        visited[adj.x - bounds.left][adj.y - bounds.top] = true;
                    }

                    adj = sf::Vector2i(current.x, current.y-1);
                    if(IsInsideBounds(adj) && !visited[adj.x - bounds.left][adj.y - bounds.top])
                    {
                        toVisit.push(adj);
                        visited[adj.x - bounds.left][adj.y - bounds.top] = true;
                    }

                    currentColor.a = RefinedBackgroundDiscardAlpha;
                    maskedImage->setPixel(current.x - bounds.left, current.y - bounds.top, currentColor);
                }
            }
            maskedImagePointer = maskedImage;
            return;
        }
    }
}

void Piece::RefinePiece(sf::Image *inputImage) //RefinedBackgroundDiscardAlpha
{
    sf::Image *maskedImage;
    RefineBackground(inputImage, maskedImage); //Get the masked image with the refined background discarded

    //BFS to get piece's pixels
    vector<Coord> connectedComponent;

    bool visited[bounds.width][bounds.height];
    for(int x = 0; x < bounds.width; ++x) for(int y = 0; y < bounds.height; ++y) visited[x][y] = false;

    for(int x = 0; x < bounds.width; ++x)
    {
        for(int y = 0; y < bounds.height; ++y)
        {
            if(visited[x][y]) continue;

            queue<sf::Vector2i> toVisit;
            toVisit.push(sf::Vector2i(x, y));
            vector<Coord> ccCoords;
            vector<Coord> ccBorder;
            while(toVisit.size() > 0)
            {
                sf::Vector2i current = toVisit.front(); toVisit.pop();
                visited[current.x][current.y] = true;

                sf::Color currentColor = maskedImage->getPixel(current.x, current.y);
                if(currentColor.a != RefinedBackgroundDiscardAlpha )
                {
                    bool border = false;

                    sf::Vector2i adj = sf::Vector2i(current.x-1, current.y);
                    if(adj.x >= 0 && adj.y >= 0 && adj.x < bounds.width && adj.y < bounds.height) {
                        if(maskedImage->getPixel(adj.x,adj.y).a == RefinedBackgroundDiscardAlpha) border = true;
                        else if(!visited[adj.x][adj.y])
                        {
                            toVisit.push(adj);
                            visited[adj.x][adj.y] = true;
                        }
                    }

                    adj = sf::Vector2i(current.x+1, current.y);
                    if(adj.x >= 0 && adj.y >= 0 && adj.x < bounds.width && adj.y < bounds.height) {
                        if(maskedImage->getPixel(adj.x,adj.y).a == RefinedBackgroundDiscardAlpha) border = true;
                        else if(!visited[adj.x][adj.y])
                        {
                            toVisit.push(adj);
                            visited[adj.x][adj.y] = true;
                        }
                    }

                    adj = sf::Vector2i(current.x, current.y+1);
                    if(adj.x >= 0 && adj.y >= 0 && adj.x < bounds.width && adj.y < bounds.height) {
                        if(maskedImage->getPixel(adj.x,adj.y).a == RefinedBackgroundDiscardAlpha) border = true;
                        else if(!visited[adj.x][adj.y])
                        {
                            toVisit.push(adj);
                            visited[adj.x][adj.y] = true;
                        }
                    }

                    adj = sf::Vector2i(current.x, current.y-1);
                    if(adj.x >= 0 && adj.y >= 0 && adj.x < bounds.width && adj.y < bounds.height) {
                        if(maskedImage->getPixel(adj.x,adj.y).a == RefinedBackgroundDiscardAlpha) border = true;
                        else if(!visited[adj.x][adj.y])
                        {
                            toVisit.push(adj);
                            visited[adj.x][adj.y] = true;
                        }
                    }

                    Coord coord; coord.x = current.x + bounds.left; coord.y = current.y + bounds.top;
                    ccCoords.push_back(coord);

                    //cout << border << endl;
                    if (border) {
                        ccBorder.push_back(coord);
                    }
                }
            }

            if(ccCoords.size() > connectedComponent.size())
            {
                connectedComponent = ccCoords;
                refinedBorderPixels = ccBorder;
            }
        }
    }

    sf::Color randomPieceColor = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255);
    for(int i = 0; i < connectedComponent.size(); ++i)
    {
        Coord coord = connectedComponent[i];
        inputImage->setPixel(coord.x, coord.y, randomPieceColor);
    }

    for(int i = 0; i < refinedBorderPixels.size(); ++i)
    {
        Coord coord = refinedBorderPixels[i];
        inputImage->setPixel(coord.x, coord.y, sf::Color(255,0,255,255));
    }

    //CORNERS STUFF
    GetPieceCorners(inputImage);
    DrawCorners(inputImage);

    delete maskedImage;
}

void Piece::GetPieceCorners(sf::Image *inputImage)
{
    for(int i = 0; i < refinedBorderPixels.size(); ++i)
    {
        Coord coord = refinedBorderPixels[i];
        if(IsCorner(inputImage, coord))
        {
            cornerPixelCoords.push_back(coord);
        }
    }
}

bool Piece::IsCorner(sf::Image *inputImage, Coord pixelCoord)
{
    float AcceptanceBackgroundPixelsPercent = 0.6f;
    int radius = 20;
    int totalPixels = 0, bgPixelCount = 0;
    for(int x = pixelCoord.x - radius; x <= pixelCoord.x + radius; ++x)
    {
        for(int y = pixelCoord.y - radius; y <= pixelCoord.y + radius; ++y)
        {
            if(IsInsideBounds(sf::Vector2i(x,y)))
            {
                ++totalPixels;
                sf::Color color = inputImage->getPixel(x, y);
                if(Image::EqualColors(color, refinedBackgroundColor, RefinedBackgroundThreshold))
                {
                    ++bgPixelCount;
                }
            }
        }
    }

    return bgPixelCount >= totalPixels * AcceptanceBackgroundPixelsPercent;
}

void Piece::DrawCorners(sf::Image *inputImage)
{
    int pointRadius = 1;
    for(int i = 0; i < cornerPixelCoords.size(); ++i)
    {
        Coord pixelCoord = cornerPixelCoords[i];
        for(int x = pixelCoord.x - pointRadius; x <= pixelCoord.x + pointRadius; ++x)
        {
            for(int y = pixelCoord.y - pointRadius; y <= pixelCoord.y + pointRadius; ++y)
            {
                if(IsInsideBounds(sf::Vector2i(x,y)))
                {
                    inputImage->setPixel(x, y, sf::Color(255, 0, 0,255));
                }
            }
        }
    }
}
