#include "Piece.h"
#include "Image.h"

int Piece::MinSizeToBeAPiece = 5000;
float Piece::RefinedBackgroundThreshold = 0.15f;

Piece::Piece()
{
}

void Piece::Refine(sf::Image *inputImage)
{
    //Get the background color
    int numPixelsInPiece = 0;
    sf::Vector3f backgroundMeanColorV(.0f, .0f, .0f);

    for(int x = bounds.left; x < bounds.left + bounds.width; ++x)
    {
        for(int y = bounds.top; y < bounds.top + bounds.height; ++y)
        {
            sf::Color pixelColor = inputImage->getPixel(x,y);
            if( Image::IsDiscarded(pixelColor) )
            {
                ++numPixelsInPiece;
                backgroundMeanColorV += sf::Vector3f(pixelColor.r, pixelColor.g, pixelColor.b);
            }
        }
    }
    backgroundMeanColorV /= float(numPixelsInPiece);
    //

    sf::Color backgroundMeanColor = sf::Color(backgroundMeanColorV.x, backgroundMeanColorV.y, backgroundMeanColorV.z);

    //BFS to get piece's pixels

    vector<Coord> connectedComponents;

    bool visited[bounds.width][bounds.height];
    for(int x = 0; x < bounds.width; ++x) for(int y = 0; y < bounds.height; ++y) visited[x][y] = false;

    for(int x = bounds.left; x < bounds.left + bounds.width; ++x)
    {
        for(int y = bounds.top; y < bounds.top + bounds.height; ++y)
        {
            if(visited[x - bounds.left][y - bounds.top]) continue;

            queue<sf::Vector2i> toVisit;
            toVisit.push(sf::Vector2i(x, y));
            vector<Coord> ccCoords;
            while(toVisit.size() > 0)
            {
                sf::Vector2i current = toVisit.front(); toVisit.pop();
                visited[current.x - bounds.left][current.y - bounds.top] = true;

                sf::Color currentColor = inputImage->getPixel(current.x, current.y);
                if( !Image::EqualColors(currentColor, backgroundMeanColor, RefinedBackgroundThreshold) )
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

                    Coord coord; coord.x = current.x; coord.y = current.y;
                    ccCoords.push_back(coord);
                }

                if(ccCoords.size() > connectedComponents.size())
                    connectedComponents = ccCoords;
            }
        }
    }

    sf::Color randomPieceColor = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255);
    for(int i = 0; i < connectedComponents.size(); ++i)
    {
        Coord coord = connectedComponents[i];
        inputImage->setPixel(coord.x, coord.y, randomPieceColor);
    }
}
