#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
using namespace std;
using namespace sf;

const int M = 20; 
const int N = 10;
int field[M][N] = {0};

class Point
{
public:
    int x ,y; // set this zero is only for initialization
};
Point a[4], b[4]; // a represents current block, b represents previous state of current block

int figures[7][4] = 
{
    1,3,5,7, // I  reference:
    2,4,5,7, // Z            0 1
    3,5,4,6, // S            2 3
    3,5,4,7, // T            4 5
    2,3,5,7, // L            6 7
    3,5,7,6, // J
    2,3,4,5, // O
};

bool check() // to see whether expection condition happen.
{
    for (int i = 0; i< 4; i++)
    {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) // expection1: one of the block exceed window and hit bottom
            return false;
        else if (field[a[i].y][a[i].x] > 0) // expection2: one of the block hit exsitied block
            return false;
    }
    return true;
}

int main()
{
    srand(time(0));
    RenderWindow window(sf::VideoMode(320, 480), "The Game!");
    
    Texture t;
    t.loadFromFile("images/tiles.png");
    Texture t2;
    t2.loadFromFile("images/background.png");
    Texture t3;
    t3.loadFromFile("images/frame.png");
    Sprite s(t);
    s.setTextureRect(IntRect(0,0,18,18)); // sparse tiles into one tile
    Sprite background(t2);
    
    int dx = 0; bool rotate = false; int colorNum = 1;
    float timer = 0, delay = 0.3;
    
    Clock clock;
    
    // set a initial block for player
    int first = rand()%7;
    for (int i = 0; i < 4; i++)
    {
        a[i].x = figures[first][i] % 2;
        a[i].y = figures[first][i] / 2;
    }
    
    
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        
        
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up)
                    rotate = true;
                else if (event.key.code == Keyboard::Left)
                    dx = -1;
                else if (event.key.code == Keyboard::Right)
                    dx = +1;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;
        
        
        /////// Move operation ///////
        for (int i =0; i < 4; i++)
        { 
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
        
        
        /////// Rotation operation ///////
        if (rotate)
        {
            Point center = a[1];
            for (int i = 0; i < 4 ; i++)
            {
                int x_diff = a[i].x - center.x;
                int y_diff = a[i].y - center.y;
                a[i].y = center.y + x_diff;
                a[i].x = center.x - y_diff;
            }
            /*
            // tutorial version, I think it can't handle rotate at edge.
            if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i]; 
            */
            
            // my version
            if (!check()) for (int i = 0; i < 4; i++)
            {
                // two case
                while(a[i].x < 0)
                    for (int i = 0; i < 4; i++) a[i].x++;
                    
                while(a[i].x >= N)
                    for (int i = 0; i < 4; i++) a[i].x--;
            }
            
        }
        
        ////// Tick ///////
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++){ b[i] = a[i]; a[i].y += 1;}
            
            // at this stage, the expection only happen when block hits bottom or exsited block
            // therefore, we need to fix these blocks
            if (!check())
            {
                // generates fixed blocks
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;
                // now set a new block for player
                colorNum = 1+rand()%7; // new color
                int n = rand()%7; // new patten
                
                // now generate new block
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[n][i] % 2; 
                    a[i].y = figures[n][i] / 2;
                }
            }
            timer = 0;
        }
        ///// check line //////
        int k = M - 1; // this means fixed line (bottom line in the beginning)
        for (int i = M - 1; i > 0; i--) // loop row of field
        {
            int counter = 0;
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] > 0)
                    counter++;
                field[k][j] = field[i][j];
            }                               
            if (counter < N) k--;
            /*
                we always store the field value in the 
                bottom line, in this case when counter is
                full k won't change. This means next row will
                replace previous row, causing previois row disappear.
            */
            
        }
        dx = 0; rotate = false; delay = 0.3;
        
        ////// draw blocks//////
        window.clear(Color::White);
        window.draw(background);
        
        // this part draw exsited block
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0) continue;
                s.setTextureRect(IntRect(field[i][j]*18 ,0,18,18));
                s.setPosition(j*18, i*18);
                s.move(28,31);
                window.draw(s);
            }
        // this part draw current block (that one is moving)
        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(colorNum*18 ,0,18,18));
            s.setPosition(a[i].x * 18, a[i].y * 18);
            s.move(28,31);
            window.draw(s);
        }
        
        window.display();
    }

    return 0;
}
