#include <iostream>
#include <cstdlib>
#include "SFML\Audio.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "SFML\System.hpp"


//#######################################\\
//Title : PONG          Date : 22-10-2023\\
//                                       \\
//           Developer : Reon23          \\ 
//                                       \\
//#######################################\\


#define SCREEN_WIDTH    900
#define SCREEN_HEIGHT   900
#define PONG_SIZE       40.f
#define BAR_THICK       20.f
#define MOVE_SPEED      25.f
#define PONG_SPEED      15.f
#define LOSE_SPEED      2.f
#define MAX_SCORE       5
using namespace sf;

void Bar_Movement(RectangleShape &bar, RectangleShape&bar2, RenderWindow &window);

int main()
{
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "PING PONG!", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);

    bool anchored = true;
    bool game_over = false;
    float pongx = 5;
    float pongy = 1;

    int P1 = 0;
    int P2 = 0;
    
    //Game timer
    Clock clock;                    
    

    //Game Audio
    SoundBuffer AUD1;               
    AUD1.loadFromFile("pong_hit.wav");

    Sound Hit;
    Hit.setBuffer(AUD1);

    SoundBuffer AUD2;
    AUD2.loadFromFile("pong_miss.wav");

    Sound Miss;
    Miss.setBuffer(AUD2);

    //Game ScoreBoard
    Font font;
    font.loadFromFile("SuperMario256.ttf");

    Text P1_score;
    P1_score.setFont(font);
    P1_score.setCharacterSize(69);
    P1_score.setString(std::to_string(P1));
    P1_score.setPosition(window.getSize().x / 3.f, 20);

    Text P2_score;
    P2_score.setFont(font);
    P2_score.setCharacterSize(69);
    P2_score.setString(std::to_string(P2));
    P2_score.setPosition(window.getSize().x / 1.5f, 20);

    Text title;
    title.setFont(font);
    title.setCharacterSize(48);
    title.setString("PONG");
    title.setPosition(window.getSize().x / 2.2f, 10);

    Text win;
    win.setFont(font);
    win.setCharacterSize(48);
    win.setString("!! DRAW !!");
    win.setPosition(window.getSize().x / 3.f, window.getSize().y / 1.7f);

    Text restart;
    restart.setFont(font);
    restart.setCharacterSize(38);
    restart.setFillColor(Color::Color(73, 78, 87, 255));
    restart.setString("<SPACE>");
    restart.setPosition(window.getSize().x / 2.5f, window.getSize().y / 1.5f);

    //Game Objects

    RectangleShape bar1;
    bar1.setSize(Vector2f(BAR_THICK, SCREEN_HEIGHT / 2.6f));
    
    RectangleShape bar2;
    bar2.setSize(Vector2f(BAR_THICK, SCREEN_HEIGHT / 2.6f));
    bar2.setPosition(window.getSize().x - BAR_THICK, window.getSize().y - (SCREEN_HEIGHT / 2.6f));

    RectangleShape pong;
    pong.setSize(Vector2f(PONG_SIZE, PONG_SIZE));
    pong.setFillColor(Color::White);
    pong.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        //Update Section :


        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        //Bar Movement :

        Bar_Movement(bar1, bar2, window);

        //Pong movement :

        if (time >= 2 && window.hasFocus())         //Initial Start Position
        {
            anchored = false;
            pong.setFillColor(Color::White);
        }

        if (anchored == false)
        {
            pong.move(pongx, pongy);
        }
        
        //Pong Bounce Physics :

        if (pong.getPosition().x > window.getSize().x - pong.getSize().x && pong.getGlobalBounds().intersects(bar2.getGlobalBounds()))      //Player2 Hit Pong   
        {
            pongx = -(PONG_SPEED + rand() % 10);
            pong.setFillColor(Color::White);
            Hit.play();
        }
        else if (pong.getPosition().x > window.getSize().x - pong.getSize().x)  //If Player2 Miss
        {
            pong.setPosition(window.getSize().x / 2, window.getSize().y / 2);
            bar1.setPosition(0.f, 0.f);
            bar2.setPosition(window.getSize().x - BAR_THICK, window.getSize().y - (SCREEN_HEIGHT / 2.6f));
            pongx = -(LOSE_SPEED + rand() % 10);
            pong.setFillColor(Color::Blue);
            Miss.play();
            anchored = true;
            P1 += 1;
            P1_score.setString(std::to_string(P1));
            clock.restart();
        }
        if (pong.getPosition().x < 0 && pong.getGlobalBounds().intersects(bar1.getGlobalBounds()))                                          //Player1 Hit Pong                                    
        {
            pongx = (PONG_SPEED + rand() % 10) - 10;
            pong.setFillColor(Color::White);
            Hit.play();
        }
        else if (pong.getPosition().x < 0)                                      //If Player1 Miss
        {
            pong.setPosition(window.getSize().x / 2, window.getSize().y / 2);
            bar1.setPosition(0.f, 0.f);
            bar2.setPosition(window.getSize().x - BAR_THICK, window.getSize().y - (SCREEN_HEIGHT / 2.6f));
            pongx = (LOSE_SPEED + rand() % 10);
            pong.setFillColor(Color::Blue);
            Miss.play();
            anchored = true;
            P2 += 1;
            P2_score.setString(std::to_string(P2));
            clock.restart();
        }

        if (pong.getPosition().y > window.getSize().y - pong.getSize().y )   //When pong collide bottom bound
        {
            pongy = -(PONG_SPEED + rand() % 10);
            pong.setFillColor(Color::White);
            Hit.play();
        }
        else if (pong.getPosition().y < 0)                                      //When pong collide top bound
        {
            pongy = (PONG_SPEED + rand() % 10);
            pong.setFillColor(Color::White);
            Hit.play();
        }

        //Check If player win
        if (P1 == MAX_SCORE)
        {
            win.setString("!! P1 WIN !!");
            game_over = true;
        }
        else if (P2 == MAX_SCORE)
        {
            win.setString("!! P2 WIN !!");
            game_over = true;
        }
        else
            game_over = false;

        //Clears previous frame

        window.clear();     

        //Draw

        window.draw(bar1);
        window.draw(bar2);
        window.draw(P1_score);
        window.draw(P2_score);
        window.draw(title);
        window.draw(pong);

        if (game_over == true)
        {
            window.draw(win);
            window.draw(restart);
            clock.restart();
            pong.setFillColor(Color::White);
        }

        // Restart Game

        if (Keyboard::isKeyPressed(Keyboard::Space) && game_over == true)
        {
            P1 = 0;
            P2 = 0;

            P1_score.setString(std::to_string(P1));
            P2_score.setString(std::to_string(P2));

            bar1.setPosition(0.f, 0.f);
            bar2.setPosition(window.getSize().x - BAR_THICK, window.getSize().y - (SCREEN_HEIGHT / 2.6f));
            clock.restart();
            game_over = false;
        }

        //Display Rendered Objects

        window.display();
    }




    return 0;
}

void Bar_Movement(RectangleShape &bar, RectangleShape &bar2, RenderWindow &window )
{
    //First Bar
    if (Keyboard::isKeyPressed(Keyboard::W) && bar.getPosition().y >= 0)            //BAR1 MOVE UP (Player 1)
    {
        bar.move(0, -MOVE_SPEED);
    }

    if (Keyboard::isKeyPressed(Keyboard::S) && bar.getPosition().y + bar.getSize().y <= window.getSize().y) //BAR1 MOVE DOWN (Player 1)
    {
        bar.move(0, MOVE_SPEED);
    }

    //Second Bar
    if (Keyboard::isKeyPressed(Keyboard::I) && bar2.getPosition().y >= 0)           //BAR2 MOVE UP (Player 2)
    {
        bar2.move(0, -MOVE_SPEED);
    }

    if (Keyboard::isKeyPressed(Keyboard::K) && bar2.getPosition().y + bar2.getSize().y <= window.getSize().y) //BAR2 MOVE DOWN (Player 2)
    {
        bar2.move(0, MOVE_SPEED);
    }
}

    
