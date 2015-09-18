#include "Image.h"

bool Image::isBorderPixel(int x, int y)
{
    if (IsInsideImage(sf::Vector2i(x-1,y)) && IsDiscarded(image.getPixel(x-1,y))) return true;
    if (IsInsideImage(sf::Vector2i(x,y-1)) && IsDiscarded(image.getPixel(x,y-1))) return true;
    if (IsInsideImage(sf::Vector2i(x+1,y)) && IsDiscarded(image.getPixel(x+1,y))) return true;
    if (IsInsideImage(sf::Vector2i(x,y+1)) && IsDiscarded(image.getPixel(x,y+1))) return true;
    return false;
}

Image::Image(string filename, int windowWidth, int windowHeight)
{    BackgroundOriginalColor = sf::Color(160, 160, 160);

    BackgroundOriginalColor = sf::Color(90, 180, 100);
    BackgroundColorThreshold = 0.3f;
    DiscardAlpha = 0;
    BorderColor = sf::Color(255, 0, 0, 255);

    if (!image.loadFromFile(filename)) { cerr << "File not found." << endl; exit(-1); }
    imageWidth = image.getSize().x;
    imageHeight = image.getSize().y;
}

void Image::DiscardBackground()
{
    bool visited[imageWidth][imageHeight];
    for(int x = 0; x < imageWidth; ++x) for(int y = 0; y < imageHeight; ++y) visited[x][y] = false;

    queue<sf::Vector2i> toVisit;
    toVisit.push(sf::Vector2i(0,0));
    while(toVisit.size() > 0)
    {
        sf::Vector2i current = toVisit.front(); toVisit.pop();
        visited[current.x][current.y] = true;

        sf::Color currentColor = image.getPixel(current.x, current.y);
        if(EqualColors(currentColor, BackgroundOriginalColor, BackgroundColorThreshold) )
        {
            sf::Vector2i adj = sf::Vector2i(current.x-1, current.y);
            if(IsInsideImage(adj) && !visited[adj.x][adj.y])
            {
                toVisit.push(adj);
                visited[adj.x][adj.y] = true;
            }

            adj = sf::Vector2i(current.x+1, current.y);
            if(IsInsideImage(adj) && !visited[adj.x][adj.y])
            {
                toVisit.push(adj);
                visited[adj.x][adj.y] = true;
            }

            adj = sf::Vector2i(current.x, current.y+1);
            if(IsInsideImage(adj) && !visited[adj.x][adj.y])
            {
                toVisit.push(adj);
                visited[adj.x][adj.y] = true;
            }

            adj = sf::Vector2i(current.x, current.y-1);
            if(IsInsideImage(adj) && !visited[adj.x][adj.y])
            {
                toVisit.push(adj);
                visited[adj.x][adj.y] = true;
            }

            currentColor.a = DiscardAlpha;
            image.setPixel(current.x, current.y, currentColor);
        }
    }
}


void Image::GetPieces()
{
    bool visited[imageWidth][imageHeight];
    for(int x = 0; x < imageWidth; ++x) for(int y = 0; y < imageHeight; ++y) visited[x][y] = false;

    for(int x = 0; x < imageWidth; ++x)
    {
        for(int y = 0; y < imageHeight; ++y)
        {
            if(visited[x][y]) continue;
            sf::Color randomPieceColor = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255);

            //BFS INSIDE THE PIECE
            queue<sf::Vector2i> toVisit;
            toVisit.push(sf::Vector2i(x,y));
            while(toVisit.size() > 0)
            {
                sf::Vector2i current = toVisit.front(); toVisit.pop();
                visited[current.x][current.y] = true;
                if(!IsDiscarded(image.getPixel(current.x, current.y))) {

                    sf::Vector2i adj = sf::Vector2i(current.x-1, current.y);
                    if(IsInsideImage(adj) && !visited[adj.x][adj.y])
                    {
                        toVisit.push(adj);
                        visited[adj.x][adj.y] = true;
                    }

                    adj = sf::Vector2i(current.x+1, current.y);
                    if(IsInsideImage(adj) && !visited[adj.x][adj.y])
                    {
                        toVisit.push(adj);
                        visited[adj.x][adj.y] = true;
                    }

                    adj = sf::Vector2i(current.x, current.y+1);
                    if(IsInsideImage(adj) && !visited[adj.x][adj.y])
                    {
                        toVisit.push(adj);
                        visited[adj.x][adj.y] = true;
                    }

                    adj = sf::Vector2i(current.x, current.y-1);
                    if(IsInsideImage(adj) && !visited[adj.x][adj.y])
                    {
                        toVisit.push(adj);
                        visited[adj.x][adj.y] = true;
                    }

                    if (isBorderPixel(current.x, current.y)) {
                        image.setPixel(current.x, current.y, BorderColor);
                    } else {
                        image.setPixel(current.x, current.y, randomPieceColor);
                    }
                }
            }
            //
        }
    }
}

sf::Sprite* Image::GetSprite()
{
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    return &sprite;
}


