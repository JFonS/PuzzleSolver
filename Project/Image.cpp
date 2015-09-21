#include "Image.h"

unsigned char Image::DiscardAlpha = 0;

Image::Image(string filename, int windowWidth, int windowHeight)
{

    BackgroundOriginalColor = sf::Color(160, 160, 160);
    //BackgroundOriginalColor = sf::Color(90, 180, 100);

    BackgroundColorThreshold = 0.3f;
    DiscardAlpha = 0;
    BorderColor = sf::Color(255, 0, 0, 255);

    if (!image.loadFromFile(filename)) { cerr << "File not found." << endl; exit(-1); }
    imageWidth = image.getSize().x;
    imageHeight = image.getSize().y;
    cout << "ImageWidth: " << imageWidth << endl;
    cout << "ImageHeight: " << imageHeight << endl;
}

bool Image::IsBorderPixel(int x, int y)
{
    if (IsInsideImage(sf::Vector2i(x-1,y)) && IsDiscarded(image.getPixel(x-1,y))) return true;
    if (IsInsideImage(sf::Vector2i(x,y-1)) && IsDiscarded(image.getPixel(x,y-1))) return true;
    if (IsInsideImage(sf::Vector2i(x+1,y)) && IsDiscarded(image.getPixel(x+1,y))) return true;
    if (IsInsideImage(sf::Vector2i(x,y+1)) && IsDiscarded(image.getPixel(x,y+1))) return true;
    return false;
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
    //#define DEBUG_PIECES

    bool visited[imageWidth][imageHeight];
    for(int x = 0; x < imageWidth; ++x) for(int y = 0; y < imageHeight; ++y) visited[x][y] = false;

    for(int x = 0; x < imageWidth; ++x)
    {
        for(int y = 0; y < imageHeight; ++y)
        {
            if(visited[x][y]) continue;

            #ifdef DEBUG_PIECES
                sf::Color randomPieceColor = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255);
            #endif

            //BFS INSIDE THE PIECE
            int pieceSize = 0;
            int minX, minY, maxX, maxY;
            minX = imageWidth;
            minY = imageHeight;
            maxX = maxY = 0;

            queue<sf::Vector2i> toVisit;
            toVisit.push(sf::Vector2i(x,y));
            while(toVisit.size() > 0)
            {
                sf::Vector2i current = toVisit.front(); toVisit.pop();
                visited[current.x][current.y] = true;
                if(!IsDiscarded(image.getPixel(current.x, current.y)))
                {
                    ++pieceSize;
                    if (current.x < minX) minX = current.x; if (current.x > maxX) maxX = current.x;
                    if (current.y < minY) minY = current.y; if (current.y > maxY) maxY = current.y;

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

                    #ifdef DEBUG_PIECES
                        if (isBorderPixel(current.x, current.y))
                        {
                            image.setPixel(current.x, current.y, BorderColor);
                        } else
                        {
                            image.setPixel(current.x, current.y, randomPieceColor);
                        }
                    #endif
                }
            }

            static int count = 0;

            //Save the piece if its big enough
            if (pieceSize > Piece::MinSizeToBeAPiece)
            {
                Piece *piece = new Piece();

                sf::IntRect pieceBounds = sf::IntRect(minX, minY, maxX - minX, maxY - minY);
                piece->SetBounds(&(this->image), pieceBounds);
                piece->SetSize(pieceSize);

                pieces.push_back(piece);

                #ifdef DEBUG_PIECES
                    cout << ++count << endl;
                    cout << "found piece " << pieceSize << endl;
                    cout << minX << ", " << minY << "     " << maxX << ", " << maxY << endl;
                #endif
            }
            else
            {

            }
            //
        }
    }
}

void Image::RefinePieces()
{
    for(Piece *piece : pieces)
    {
        piece->Refine(&(this->image));
    }
}

sf::Sprite* Image::GetSprite()
{
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    return &sprite;
}


