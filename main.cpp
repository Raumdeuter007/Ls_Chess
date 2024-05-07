/*
    ***********************************************
    L's Chess
    Made by Muhammad Luqman Arshad
    ***********************************************
*/
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "piece.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main()
{
    const double ts = 102.4;
    int total_time = 600;
    int inc = 2;
    sf::Vector2i offset (50, 50);
    sf::RenderWindow window(sf::VideoMode(1500, 920), "L's Chess");
    window.setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - 1500) / 2
    , (sf::VideoMode::getDesktopMode().height - 920) / 2 - 50));
    sf::Texture white, black;
    sf::Texture w_note, b_note;
    sf::Texture b_pawn, w_pawn;
    sf::Texture b_rook, w_rook;
    sf::Texture b_knight, w_knight;
    sf::Texture b_bishop, w_bishop;
    sf::Texture b_king, w_king;
    sf::Texture b_queen, w_queen;
    sf::Texture background, rect, first_bg;
    sf::SoundBuffer b_castle, b_move_check, b_move, b_promote, b_capture, b_mate;
    sf::SoundBuffer starting, trans, stmate;
    sf::Sound castle, move, move_check, promote, capture, mate;
    sf::Sound stalemate, start, transition;
    int w_kx, w_ky;
    int b_time = 0, w_time = 0;
    int *times[2] = {&b_time, &w_time};
    int b_kx, b_ky;
    sf::Image icon;
    icon.loadFromFile("Assets\\icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    sf::Font merida, lato;
    sf::Text player1, player2;
    sf::Text black_time, white_time;
    sf::Text normal;
    sf::Text time_control, name;

    // Load Sounds  
    if (!stmate.loadFromFile("Assets\\stalemate.wav"))
    {
        cout << "Error loading sound\n";
        return 1;
    }
    if (!trans.loadFromFile("Assets\\transition.wav"))
    {
        cout << "Error loading sound\n";
        return 1;
    } 
    if (!starting.loadFromFile("Assets\\start.wav"))
    {
        cout << "Error loading sound\n";
        return 1;
    }
    if (!b_capture.loadFromFile("Assets\\capture.wav"))
    {
        cout << "error loading sound\n";
        return 1;
    }
    if (!b_castle.loadFromFile("Assets\\castle.wav"))
    {
        cout << "error loading sound\n";
        return 1;
    }
    if (!b_move.loadFromFile("Assets\\move-self.wav"))
    {
        cout << "error loading sound\n";
        return 1;
    }
    if (!b_move_check.loadFromFile("Assets\\move-check.wav"))
    {
        cout << "error loading sound\n";
        return 1;
    }
    if (!b_promote.loadFromFile("Assets\\promote.wav"))
    {
        cout << "error loading sound\n";
        return 1;
    }
    if (!b_mate.loadFromFile("Assets\\checkmate.wav"))
    {
        cout << "error loading sound\n";
        return 1;
    }

    // Load picture
    if (!background.loadFromFile("Assets\\bg_pic.jpg"))
    {
        cout << "error loading image\n";
        return 1;
    }
    if (!rect.loadFromFile("Assets\\rect.png"))
    {
        cout << "error loading image\n";
        return 1;
    }
    if (!first_bg.loadFromFile("Assets\\bg_picw.jpg"))
    {
        cout << "error loading image\n";
        return 1;
    }

    // Load Font
    if (!merida.loadFromFile("Assets\\Merida.ttf"))
    {
        cout << "error loading font\n";
        return 1;
    }
    if (!lato.loadFromFile("Assets\\Lato-Regular.ttf"))
    {
        cout << "error loading font\n";
        return 1;
    }

    // Load Tiles
    if (!white.loadFromFile("Assets\\white.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }
    if (!black.loadFromFile("Assets\\black.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }

    // Load Notation Background
    if (!w_note.loadFromFile("Assets\\w_notation.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }
    if (!b_note.loadFromFile("Assets\\b_notation.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }

    // Load pawns
    if (!b_pawn.loadFromFile("Assets\\b_pawn.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }
    if (!w_pawn.loadFromFile("Assets\\w_pawn.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }

    // Load Rooks
    if (!b_rook.loadFromFile("Assets\\b_rook.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }
    if (!w_rook.loadFromFile("Assets\\w_rook.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }

    // Load Knights
    if (!b_knight.loadFromFile("Assets\\b_knight.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }
    if (!w_knight.loadFromFile("Assets\\w_knight.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }

    // Load bishops
    if (!b_bishop.loadFromFile("Assets\\b_bishop.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }
    if (!w_bishop.loadFromFile("Assets\\w_bishop.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }

    // Load Kings
    if (!b_king.loadFromFile("Assets\\b_king.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }
    if (!w_king.loadFromFile("Assets\\w_king.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }

    // Load queens
    if (!b_queen.loadFromFile("Assets\\b_queen.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }
    if (!w_queen.loadFromFile("Assets\\w_queen.png"))
    {
        cout << "Error loading texture\n";
        return 1;
    }
    
    // Set Sound
    castle.setBuffer(b_castle);
    move.setBuffer(b_move);
    move_check.setBuffer(b_move_check);
    promote.setBuffer(b_promote);
    capture.setBuffer(b_capture);
    mate.setBuffer(b_mate);
    start.setBuffer(starting);
    start.setLoop(true);
    transition.setBuffer(trans);
    stalemate.setBuffer(stmate);

    // Set Text
    player1.setCharacterSize(50);
    player1.setFillColor(sf::Color::White);
    player1.setOutlineColor(sf::Color::Black);
    player1.setOutlineThickness(1);
    player1.setFont(lato);
    player1.setString("White");
    player1.setPosition(1000.f, 800.f);
    player2.setCharacterSize(50);
    player2.setFillColor(sf::Color::Black);
    player2.setFont(lato);
    player2.setString("Black");
    player2.setOutlineColor(sf::Color::White);
    player2.setOutlineThickness(1);
    player2.setPosition(1000.f, 30.f);
    black_time.setCharacterSize(50);
    black_time.setFillColor(sf::Color::Black);
    black_time.setOutlineColor(sf::Color::White);
    black_time.setOutlineThickness(1);
    black_time.setFont(lato);
    black_time.setPosition(1200.f, 30.f);
    white_time.setCharacterSize(50);
    white_time.setFillColor(sf::Color::White);
    white_time.setOutlineColor(sf::Color::Black);
    white_time.setOutlineThickness(1);
    white_time.setFont(lato);
    white_time.setPosition(1200.f, 800.f);
    normal.setCharacterSize(50);
    normal.setFillColor(sf::Color::White);
    normal.setOutlineColor(sf::Color::Black);
    normal.setFont(lato);
    name.setFont(lato);
    name.setCharacterSize(100);
    name.setPosition(550.f, 50.f);
    name.setFillColor(sf::Color::White);
    name.setOutlineColor(sf::Color::Black);
    name.setOutlineThickness(2);
    name.setString("L's Chess");
    time_control.setFont(lato);
    time_control.setCharacterSize(70);
    time_control.setPosition(200.f, 350.f);
    time_control.setFillColor(sf::Color::Black);
    time_control.setOutlineColor(sf::Color::White);
    time_control.setOutlineThickness(2);
    time_control.setString("Time Control");
    
    // Set Background
    sf::Sprite bg_pic, fi_bg;
    bg_pic.setTexture(background);
    fi_bg.setTexture(first_bg);

    button bullet(lato, rect, sf::Vector2f(300.f, 475.f), "2 + 1"); 
    button blitz(lato, rect, sf::Vector2f(500.f, 475.f), "3 + 2");
    button blitz2(lato, rect, sf::Vector2f(700.f, 475.f), "5 + 0");
    button rapid(lato, rect, sf::Vector2f(900.f, 475.f), "10 + 0");
    button rapid2(lato, rect, sf::Vector2f(1100.f, 475.f), "10 + 5");


    // Set marker
    sf::CircleShape cap(30.f);
    cap.setPointCount(100);
    cap.setFillColor(sf::Color(153, 102, 51));

    // Sprites for Pawn Promotion

    sf::Sprite w_q, b_q, w_r, b_r, w_b, b_b, w_n, b_n, b, w;
    w_q.setTexture(w_queen); w_q.setScale(sf::Vector2f(0.15f, 0.15f));
    b.setTexture(black); w.setTexture(white);
    w_r.setTexture(w_rook); w_r.setScale(sf::Vector2f(0.15f, 0.15f));
    w_b.setTexture(w_bishop); w_b.setScale(sf::Vector2f(0.15f, 0.15f));
    w_n.setTexture(w_knight); w_n.setScale(sf::Vector2f(0.15f, 0.15f));
    b_q.setTexture(b_queen); b_q.setScale(sf::Vector2f(0.15f, 0.15f));
    b_r.setTexture(b_rook); b_r.setScale(sf::Vector2f(0.15f, 0.15f));
    b_b.setTexture(b_bishop); b_b.setScale(sf::Vector2f(0.15f, 0.15f));
    b_n.setTexture(b_knight); b_n.setScale(sf::Vector2f(0.15f, 0.15f));
    b.setScale(sf::Vector2f(0.2f, 0.2f)); w.setScale(sf::Vector2f(0.2f, 0.2f));

    w_q.setPosition(sf::Vector2f(1021, 421)); 
    w_r.setPosition(sf::Vector2f(1030 + ts, 422)); 
    w_b.setPosition(sf::Vector2f(1027 + 2 * ts, 420)); 
    w_n.setPosition(sf::Vector2f(1027 + 3 * ts, 420)); 
    b_q.setPosition(sf::Vector2f(1021, 420)); 
    b_r.setPosition(sf::Vector2f(1030 + ts, 422)); 
    b_b.setPosition(sf::Vector2f(1027 + 2 * ts, 420)); 
    b_n.setPosition(sf::Vector2f(1027 + 3 * ts, 420)); 

    piece* chessmen[8][8];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            chessmen[i][j] = nullptr;
        }
    }
    // chessmen[0][4] = new pawn(b_pawn, 0, 4, false); 
    // chessmen[6][2] = new pawn(b_pawn, 6, 2, false); 
    // chessmen[5][5] = new pawn(b_pawn, 5, 5, false); 
    // chessmen[4][6] = new pawn(b_pawn, 4, 6, false); 
    // chessmen[0][6] = new pawn(w_pawn, 0, 6, true);
    // chessmen[1][6] = new pawn(w_pawn, 1, 6, true);
    // chessmen[2][0] = new rook(w_rook, 2, 0, true);
    // chessmen[6][7] = new rook(w_rook, 6, 7, true);
    // chessmen[3][6] = new king(w_king, 3, 6, true);
    // chessmen[5][6] = new king(b_king, 5, 6, false);
    // b_kx = 5; b_ky = 6;
    // w_kx = 3; w_ky = 6;

    for (int i = 0; i < 8; i++)
    {
        chessmen[i][1] = new pawn(b_pawn, i, 1, false); 
        chessmen[i][6] = new pawn(w_pawn, i, 6, true);
    }

    // Initializing black pieces
    chessmen[0][0] = new rook(b_rook, 0, 0, false);
    chessmen[1][0] = new knight(b_knight, 1, 0, false);
    chessmen[2][0] = new bishop(b_bishop, 2, 0, false);
    chessmen[3][0] = new queen(b_queen, 3, 0, false);
    chessmen[4][0] = new king(b_king, 4, 0, false);
    chessmen[5][0] = new bishop(b_bishop, 5, 0, false);
    chessmen[6][0] = new knight(b_knight, 6, 0, false);
    chessmen[7][0] = new rook(b_rook, 7, 0, false);
    b_kx = 4; b_ky = 0;
    bool check = false;
    int t_move = 0;

    // Initializing white pieces
    chessmen[0][7] = new rook(w_rook, 0, 7, true);
    chessmen[1][7] = new knight(w_knight, 1, 7, true);
    chessmen[2][7] = new bishop(w_bishop, 2, 7, true);
    chessmen[3][7] = new queen(w_queen, 3, 7, true);
    chessmen[4][7] = new king(w_king, 4, 7, true);
    chessmen[5][7] = new bishop(w_bishop, 5, 7, true);
    chessmen[6][7] = new knight(w_knight, 6, 7, true);
    chessmen[7][7] = new rook(w_rook, 7, 7, true);
    w_kx = 4; w_ky = 7;

    int moves[8][8] = {};
    vector <string> record;
    int num_moves = 0;
    bool played = false;
    board B (white, black);
    notation bg_note (w_note, b_note);
    n_text note(merida);
    sf::Clock clock, curr_time;
    int click = 0;
    bool turn = true;
    int x, y, x0, y0;
    int state = 0;
    sf::Vector2i pos;
    while (window.isOpen())
    {
        sf::Event event;
        sf::Time elapsed;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {   
                if (state == 0)
                {
                    pos = sf::Mouse::getPosition(window);
                    if (pos.y >= 475 && pos.y <= 540)
                    {
                        if (pos.x >= 300 && pos.x <= 405)
                        {
                            total_time = 120;
                            inc = 1;
                            state = 1;
                            clock.restart();
                        }
                        else if (pos.x >= 500 && pos.x <= 605)
                        {
                            total_time = 180;
                            inc = 2;
                            state = 1;
                            clock.restart();
                        }
                        else if (pos.x >= 700 && pos.x <= 805)
                        {
                            total_time = 300;
                            inc = 0;
                            state = 1;
                            clock.restart();
                        }
                        else if (pos.x >= 900 && pos.x <= 1005)
                        {
                            total_time = 600;
                            inc = 0;
                            state = 1;
                            clock.restart();
                        }
                        else if (pos.x >= 1100 && pos.x <= 1205)
                        {
                            total_time = 600;
                            inc = 5;
                            state = 1;
                            clock.restart();
                        }
                    }
                }
                else if (state == 2)
                {
                    if (click == 0)
                    {
                        click = 1;
                        pos = sf::Mouse::getPosition(window) - offset;
                        if (pos.x < 0 || pos.y < 0)
                            click = 0;
                    }
                    else if (click == 1)
                    {
                        pos = sf::Mouse::getPosition(window) - offset;
                        x = pos.x / ts;
                        y = pos.y / ts;
                        if (pos.x < 0 || pos.y < 0)
                        {
                            clearMoves(moves);
                            click = 0;
                        }
                        if (x >= 0 && x <= 7 && y >= 0 && y <= 7 && moves[x][y] != 0)
                        {
                            click = 2;
                            if (moves[x][y] == 1 || moves[x][y] == 3)
                                move.play();
                            else if (moves[x][y] == 2 || moves[x][y] == 4)
                                capture.play();
                            else if (moves[x][y] == 5)
                                castle.play();

                            if (moves[x][y] < 6)
                                clock.restart();
                        }

                    }
                    else if (click == 3)
                    {
                        pos = sf::Mouse::getPosition(window);
                        clock.restart();
                        click = 4;
                    }
                }
            }
        }
        elapsed = clock.getElapsedTime();
        if (state == 0 && start.getStatus() != 2)
        {
            start.play();
        }
        else if (state == 1)
        {
            start.stop();
            if (transition.getStatus() != 2)
                transition.play();
            fi_bg.setPosition(sf::Vector2f(((0 - 1500) *  elapsed.asMilliseconds()) / 1000, fi_bg.getPosition().y));
            if (elapsed.asMilliseconds() > 1000)
            {
                curr_time.restart();
                state = 2;
                w_time = 0;
            }
        }
        else if (state == 2)
        {
            if (click == 1 && *times[turn] < total_time)
            {
                x0 = pos.x / ts;
                y0 = pos.y / ts;
                if (x0 >= 0 && x0 <= 7 && y0 >= 0 && y0 <= 7 && chessmen[x0][y0] != nullptr && turn == chessmen[x0][y0]->White())
                {
                    if (turn)
                        chessmen[x0][y0]->showMoves(chessmen, moves, w_kx, w_ky);
                    else
                        chessmen[x0][y0]->showMoves(chessmen, moves, b_kx, b_ky);
                }
                else
                {
                    clearMoves(moves);
                    click = 0;
                }
            }
            else if (click == 2 && *times[turn] < total_time)
            {
                string this_move;
                if (chessmen[x0][y0]->type() != 'p')
                {
                    this_move += toupper(chessmen[x0][y0]->type());
                }
                else
                {
                    this_move += 'a' + x0;
                }
                if (moves[x][y] == 1 || moves[x][y] == 3 || moves[x][y] == 5)
                {
                    chessmen[x0][y0]->move(x, y, elapsed.asMilliseconds());
                    if (moves[x][y] == 5)
                    {
                        if (chessmen[x + 1][y] != nullptr && chessmen[x + 1][y]->type() == 'r')
                        {
                            chessmen[x + 1][y]->move(x - 1, y, elapsed.asMilliseconds());
                        }
                        else
                        {
                            chessmen[x - 2][y]->move(x + 1, y, elapsed.asMilliseconds());
                        }
                    }
                    if (elapsed.asMilliseconds() > move_t)
                    {
                        click = 0;
                        chessmen[x][y] = chessmen[x0][y0];
                        chessmen[x0][y0] = nullptr;
                        if (chessmen[x][y]->type() == 'k')
                        {
                            if (turn)
                            {
                                w_kx = x;
                                w_ky = y;
                            }
                            else 
                            {
                                b_kx = x;
                                b_ky = y;
                            }
                        }
                        if (moves[x][y] == 5)
                        {
                            if (chessmen[x + 1][y] != nullptr && chessmen[x + 1][y]->type() == 'r')
                            {
                                chessmen[x - 1][y] = chessmen[x + 1][y];
                                chessmen[x + 1][y] = nullptr;
                            }
                            else
                            {
                                chessmen[x + 1][y] = chessmen[x - 2][y];
                                chessmen[x - 2][y] = nullptr;
                            }
                        }
                        clear_en(chessmen);
                        if (moves[x][y] == 3)
                        {
                            pawn* pwn = (pawn*) chessmen[x][y];
                            if (turn)
                                pwn->check_en(chessmen, x, y, b_kx, b_ky);
                            else
                                pwn->check_en(chessmen, x, y, w_kx, w_ky);

                        }
                        if (chessmen[x][y]->type() != 'p')
                            this_move += x + 'a';
                        this_move += 49 - y + 7;
                        if (moves[x][y] == 5)
                        {
                            if (x == 2)
                                this_move = "O-O-O";
                            else
                                this_move = "O-O";
                        }
                        clearMoves(moves);
                        turn = !turn;
                        t_move = 0;
                        if (turn)
                        {
                            t_move = valid_moves(chessmen, turn, w_kx, w_ky);
                        }
                        else 
                        {
                            t_move = valid_moves(chessmen, turn, b_kx, b_ky);
                        }
                        if (t_move == 1)
                        {    
                            this_move += '+';
                            move_check.play();
                        }
                        else if (t_move == 2)
                        {
                            this_move += '#';
                            mate.play();
                        }
                        else if (t_move == 3)
                        {
                            stalemate.play();
                        }
                        num_moves++;
                        record.push_back(this_move); 
                        sf::Time elapse = curr_time.getElapsedTime();    
                        if (!turn)
                        {
                            w_time += elapse.asSeconds() - inc;
                        }    
                        else
                        {
                            b_time += elapse.asSeconds() - inc;
                        }
                        curr_time.restart();
                    }     
                }
                else if (moves[x][y] == 2 || moves[x][y] == 4)
                {
                    chessmen[x0][y0]->move(x, y, elapsed.asMilliseconds());
                    if (moves[x][y] == 2)
                    {
                        if (chessmen[x][y] != nullptr)
                        {
                            delete chessmen[x][y];
                            chessmen[x][y] = nullptr;
                        }
                    }
                    else
                    {
                        if (chessmen[x][y0] != nullptr)
                        {
                            delete chessmen[x][y0];
                            chessmen[x][y0] = nullptr;
                        }               
                    }
                    if (elapsed.asMilliseconds() > move_t)
                    {
                        click = 0;
                        chessmen[x][y] = chessmen[x0][y0];
                        chessmen[x0][y0] = nullptr;
                        if (chessmen[x][y]->type() == 'k')
                        {
                            if (turn)
                            {
                                w_kx = x;
                                w_ky = y;
                            }
                            else 
                            {
                                b_kx = x;
                                b_ky = y;
                            }
                        }
                        this_move += 'x';
                        this_move += x + 'a';
                        this_move += 49 - y + 7;
                        clearMoves(moves);
                        t_move = 0;
                        turn = !turn;
                        if (turn)
                        {
                            t_move = valid_moves(chessmen, turn, w_kx, w_ky);
                        }
                        else 
                        {
                            t_move = valid_moves(chessmen, turn, b_kx, b_ky);
                        }
                        if (t_move == 1)
                        {
                            this_move += '+';
                            move_check.play();
                        }
                        else if (t_move == 2)
                        {
                            this_move += '#';
                            mate.play();
                        }
                        else if (t_move == 3)
                        {
                            stalemate.play();
                        }
                        num_moves++;
                        record.push_back(this_move);
                        sf::Time elapse = curr_time.getElapsedTime();    
                        if (!turn)
                        {
                            w_time += elapse.asSeconds() - inc;
                        }    
                        else
                        {
                            b_time += elapse.asSeconds() - inc;
                        }
                        curr_time.restart();
                    }    
                }
                else if (moves[x][y] == 6 || moves[x][y] == 7)
                {
                    click = 3;
                }
                else
                {
                    click = 0;
                } 
            }
            else if ((click == 4 || click == 5) && *times[turn] < total_time)
            {
                int y1 = pos.y / ts;
                int x1 = pos.x / ts - 10;
                if (y1 == 4 && x1 >= 0 && x1 <= 3)
                {
                    if (!played && promote.getStatus() != 2)
                    {
                        promote.play();
                    }
                    click = 5;
                    chessmen[x0][y0]->move(x, y, elapsed.asMilliseconds());
                    if (elapsed.asMilliseconds() > move_t)
                    {
                        string this_move;
                        this_move += 'a' + x0;
                        if (chessmen[x][y] == nullptr)
                        {
                            delete chessmen[x0][y0];
                            chessmen[x0][y0] = nullptr;
                        }
                        else
                        {
                            this_move += 'x';
                            this_move += 'a' + x;
                            delete chessmen [x0][y0];
                            chessmen[x0][y0] = nullptr;
                            delete chessmen[x][y];
                            chessmen[x][y] = nullptr;
                        }
                        this_move += 49 - y + 7;
                        this_move += '=';
                        if (x1 == 0)
                        {
                            if (turn)
                                chessmen[x][y] = new queen(w_queen, x, y, turn);
                            else
                                chessmen[x][y] = new queen(b_queen, x, y, turn);
                            this_move += 'Q';
                        }
                        else if (x1 == 1)
                        {
                            if (turn)
                                chessmen[x][y] = new rook(w_rook, x, y, turn);
                            else
                                chessmen[x][y] = new rook(b_rook, x, y, turn);
                            this_move += 'R';
                        }
                        else if (x1 == 2)
                        {
                            if (turn)
                                chessmen[x][y] = new bishop(w_bishop, x, y, turn);
                            else
                                chessmen[x][y] = new bishop(b_bishop, x, y, turn);       
                            this_move += 'B';
                        }
                        else if (x1 == 3)
                        {
                            if (turn)
                                chessmen[x][y] = new knight(w_knight, x, y, turn);
                            else
                                chessmen[x][y] = new knight(b_knight, x, y, turn);
                            this_move += 'N';
                        }
                        clearMoves(moves);
                        t_move = 0;
                        turn = !turn;
                        played = false;
                        if (turn)
                        {
                            t_move = valid_moves(chessmen, turn, w_kx, w_ky);
                        }
                        else 
                        {
                            t_move = valid_moves(chessmen, turn, b_kx, b_ky);
                        }
                        if (t_move == 1)
                        {
                            this_move += '+';
                            move_check.play();
                        }
                        else if (t_move == 2)
                        {
                            this_move += '#';
                            mate.play();
                        }
                        else if (t_move == 3)
                        {
                            stalemate.play();
                        }
                        click = 0;
                        num_moves++;
                        record.push_back(this_move);
                        sf::Time elapse = curr_time.getElapsedTime();    
                        if (!turn)
                        {
                            w_time += elapse.asSeconds() - inc; 
                        }    
                        else
                        {
                            b_time += elapse.asSeconds() - inc;
                        }
                        curr_time.restart();
                    }
                }
                else
                {
                    click = 3;
                }
            }
            else if (click != 3)
            {
                click = 0;
                clearMoves(moves);
            }
            sf::Time elapse = curr_time.getElapsedTime();
            if (turn && t_move != 2 && t_move != 3)
            {
                int seconds = elapse.asSeconds();
                string show;
                int tmp = total_time - w_time - seconds;
                if (tmp > 0)
                {
                    if (t_move < 2)
                    {
                        if (tmp / 60 < 10)
                            show += '0';
                        show += to_string(tmp / 60);
                        show += " : ";
                        if (tmp % 60 < 10)
                            show += "0";
                        show += to_string(tmp % 60);
                    }
                    else
                    {
                        if (w_time / 60 < 10)
                            show += '0';
                        show += to_string(w_time / 60);
                        show += " : ";
                        if (w_time % 60 < 10)
                            show += "0";
                        show += to_string((total_time - w_time) % 60);
                    }
                }
                else
                {
                    w_time = total_time;
                    show += "00 : 00";
                }
                white_time.setString(show);
                    show.clear();
                if ((total_time - b_time) / 60 < 10)
                    show += '0';
                show += to_string((total_time - b_time) / 60);
                show += " : ";
                if ((total_time - b_time) % 60 < 10)
                    show += "0";
                show += to_string((total_time - b_time) % 60);
                black_time.setString(show);
            }
            else if (t_move != 2 && t_move != 3)
            {
                int seconds = elapse.asSeconds();
                string show;
                int tmp = total_time - b_time - seconds;
                if (tmp > 0)
                {
                    if (t_move < 2)
                    {
                        if (tmp / 60 < 10)
                            show += '0';
                        show += to_string(tmp / 60);
                        show += " : ";
                        if (tmp % 60 < 10)
                            show += "0";
                        show += to_string(tmp % 60);
                    }
                    else
                    {
                        if ((total_time - b_time) / 60 < 10)
                            show += '0';
                        show += to_string((total_time - b_time) / 60);
                        show += " : ";
                        if ((total_time - b_time) % 60 < 10)
                            show += "0";
                        show += to_string((total_time - b_time) % 60);
                    }
                }
                else
                {
                    b_time = total_time;
                    show += "00 : 00";
                }
                black_time.setString(show);
                show.clear();
                if ((total_time - w_time) / 60 < 10)
                    show += '0';
                show += to_string((total_time - w_time) / 60);
                show += " : ";
                if ((total_time - w_time) % 60 < 10)
                    show += "0";
                show += to_string((total_time - w_time) % 60);
                white_time.setString(show);
            }
        }
        window.clear(sf::Color(28, 119, 195));
        if (state == 0)
        {
            window.draw(fi_bg);
            bullet.draw(window);
            blitz.draw(window);
            blitz2.draw(window);
            rapid.draw(window);
            rapid2.draw(window);
            window.draw(name);
            window.draw(time_control);
        }
        else if (state == 1 || state == 2)
        {
            window.draw(bg_pic);
            window.draw(black_time);
            window.draw(white_time);
            B.draw(window); 
            if (b_time < total_time && w_time < total_time && t_move < 2 && click != 3)
            {
                bg_note.draw(window);
                note.draw(window, record, num_moves);
            }
            else if (click == 3)
            {
                normal.setString("Promote into: ");
                normal.setPosition(1020.f, 300.f);
                window.draw(normal);
            }
            else if (turn)
            {
                if (t_move == 2 || w_time == total_time)
                {
                    normal.setString("Black Wins ! ");
                    normal.setPosition(1050.f, 350.f);
                    window.draw(normal);
                    if (w_time == total_time)
                    {
                        static bool done = false;
                        if (!done && mate.getStatus() != 2)
                        {
                            mate.play();
                            done = true;
                        }
                        normal.setString("By Timeout");   
                    }
                    else
                    {
                        normal.setString("Check Mate");
                    }
                    normal.setPosition(1120.f, 425.f);
                    normal.setCharacterSize(25);
                    window.draw(normal);
                    normal.setCharacterSize(50);
                }
                else
                {
                    normal.setString(" Draw ! ");
                    normal.setPosition(1100.f, 350.f);
                    window.draw(normal);
                    normal.setString("Stalemate");
                    normal.setCharacterSize(25);
                    normal.setPosition(1120.f, 425.f);
                    window.draw(normal);
                    normal.setCharacterSize(50);
                }
            }
            else
            {
                if (t_move == 2 || b_time == total_time)
                {
                    normal.setString("White Wins ! ");
                    normal.setPosition(1050.f, 350.f);
                    window.draw(normal);
                    if (b_time == total_time)
                    {
                        static bool done = false;
                        if (!done && mate.getStatus() != 2)
                        {
                            mate.play();
                            done = true;
                        }
                        normal.setString("By Timeout");   
                    }
                    else
                    {
                        normal.setString("Check Mate");
                    }
                    normal.setPosition(1120.f, 425.f);
                    normal.setCharacterSize(25);
                    window.draw(normal);
                    normal.setCharacterSize(50);
                }
                else
                {
                    normal.setString(" Draw ! ");
                    normal.setPosition(1090.f, 350.f);
                    window.draw(normal);
                    normal.setString("Stalemate");
                    normal.setPosition(1120.f, 425.f);
                    normal.setCharacterSize(25);
                    window.draw(normal);
                    normal.setCharacterSize(50);
                }
            }
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (chessmen[i][j] != nullptr)
                    {
                        chessmen[i][j]->draw(window);
                    }
                    if ((moves[i][j] == 1 || moves[i][j] == 3 || moves[i][j] == 5 || moves[i][j] == 6) && click != 2)
                    {
                        cap.setFillColor(sf::Color(153, 102, 51));
                        cap.setPosition(sf::Vector2f(offset.x + ts * i + 22, offset.y + ts * j + 20));
                        window.draw(cap);
                    }
                    else if ((moves[i][j] == 2 || moves[i][j] == 4 || moves[i][j] == 7) && click != 2)
                    {
                        cap.setFillColor(sf::Color(179, 0, 0, 100));
                        cap.setPosition(sf::Vector2f(offset.x + ts * i + 22, offset.y + ts * j + 20));
                        window.draw(cap);       
                    }
                    if (click == 3)
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if (i % 2)
                            {
                                w.setPosition(sf::Vector2f(1024 + i * ts - 10, 409.6));
                                window.draw(w);
                            }
                            else
                            {
                                b.setPosition(sf::Vector2f(1024 + i * ts - 10, 409.6));
                                window.draw(b);
                            }
                        }
                        if (turn)
                        {
                            window.draw(w_q);
                            window.draw(w_r);
                            window.draw(w_b);
                            window.draw(w_n);
                        }
                        else
                        {
                            window.draw(b_q);
                            window.draw(b_r);
                            window.draw(b_b);
                            window.draw(b_n);    
                        }
                    }
                }
            }
            window.draw(player1);
            window.draw(player2);
        }
        if (state == 1)
            window.draw(fi_bg);
        window.display();
    }

    if (turn)
    {
        if (t_move == 2 || w_time == total_time)
            record_game("record.txt", record, 2, turn);
        else if (t_move == 3)
            record_game("record.txt", record, 3, turn);
    }
    else
    {
        if (t_move == 2 || b_time == total_time)
            record_game("record.txt", record, 2, turn);
        else if (t_move == 3)
            record_game("record.txt", record, 3, turn);
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (chessmen[i][j] != nullptr)
            {    
                delete chessmen[i][j];
            }
        }
    }

    return 0;
}
