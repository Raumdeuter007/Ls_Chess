#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

const int move_t = 200;
void clearMoves(int [][8]);
class piece;
bool in_check(piece* [8][8], int, int, bool, int, int);
bool in_check(piece* [8][8], int, int, bool, int, int, int, int);
int valid_moves(piece* [8][8], bool, int, int);

/*  Move Types:
    1: Normal move
    2: Capture
    3: Double Pawn
    4: En Passant
    5: Castling
    6: Pawn Promotion (Without Capture)
    7: Pawn Promotion (With Capture)
*/

class board
{
    public:
        board(sf::Texture &w, sf::Texture &b)
        {
            white.setTexture(w);
            black.setTexture(b);
            white.setScale(sf::Vector2f(0.2f, 0.2f));
            black.setScale(sf::Vector2f(0.2f, 0.2f));
        }
        void draw(sf::RenderWindow &window)
        {
            white.setPosition(sf::Vector2f(50.f, 50.f));
            black.setPosition(sf::Vector2f(50.f, 50.f + 102.4f));
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    window.draw(white);
                    window.draw(black);
                    white.move(sf::Vector2f(0, 204.8f));
                    black.move(sf::Vector2f(0, 204.8f));
                }
                white.move(sf::Vector2f(102.4f, -819.2f));
                black.move(sf::Vector2f(102.4f, -819.2f));        
                sf::Vector2f temp = white.getPosition();
                white.setPosition(black.getPosition());
                black.setPosition(temp);
            }
        }
    private:
        sf::Sprite white, black;
};

class notation 
{
    public:
        notation(sf::Texture &w, sf::Texture &b)
        {
            white.setTexture(w);
            black.setTexture(b);
            white.setScale(sf::Vector2f(0.4f, 0.4f));
            black.setScale(sf::Vector2f(0.4f, 0.4f));
        }
        void draw(sf::RenderWindow &window)
        {
            for (int i = 0; i < 10; i++)
            {
                if (i % 2 == 0)
                {
                    white.setPosition(sf::Vector2f(1000.f, 200 + i * 52));
                    window.draw(white);
                }
                else
                {
                    black.setPosition(sf::Vector2f(1000.f, 200 + i * 52));
                    window.draw(black);
                }
            }
        }
    private:
        sf::Sprite white, black;
};

class n_text
{
    public:
        n_text(sf::Font& f)
        {
            t.setFont(f);
            t.setCharacterSize(24);
            t.setFillColor(sf::Color::White);
            t.setStyle(sf::Text::Bold);
        }
        void draw(sf::RenderWindow& window, std::vector<std::string>& record, int num_moves)
        {
            int count = (num_moves < 20 ? 0 : num_moves - 20);
            if (num_moves > 20 && num_moves % 2)
                count++;
            for (int i = 0; i < num_moves && i < 20 && count < num_moves; i++)
            {
                if (i % 2 == 0)
                {
                    t.setString(std::to_string(count / 2 + 1) + ". " + record.at(count++));
                    t.setPosition(sf::Vector2f(1050.f, 210 + (i / 2) * 52));
                }
                else
                {
                    t.setString(record.at(count++));
                    t.setPosition(sf::Vector2f(1250.f, 210 + i / 2 * 52));
                }
                window.draw(t);
            }
        }
    private:
        sf::Text t;
};

class button
{
    public:
        button(sf::Font& f, sf::Texture& t, sf::Vector2f pos, std::string s)
        {
            sp.setTexture(t);
            text.setFont(f);
            text.setCharacterSize(25);
            text.setString(s);
            if (s.length() <= 5)
                text.setPosition(pos.x + 25, pos.y + 25);
            else
                text.setPosition(pos.x + 18, pos.y + 25);
            sp.setPosition(pos.x, pos.y);
            sp.setScale(0.42f, 0.32f);
            text.setFillColor(sf::Color::White);
        }
        void draw(sf::RenderWindow& window)
        {
            window.draw(sp);
            window.draw(text);   
        }
    private:
        sf::Text text;
        sf::Sprite sp;
};

class piece
{
public:
    piece(sf::Texture &t, int x, int y, bool f)
    {
        sp.setTexture(t);
        pos_x = x;
        isWhite = f;
        pos_y = y;
        sp.setScale(sf::Vector2f(0.15f, 0.15f));
        sp.setPosition(sf::Vector2f(pos_x * 102.4 + 70, pos_y * 102.4 + 60));
    }
    void draw(sf::RenderWindow& window)
    {
        window.draw(sp);
    }
    virtual void move(int x, int y, int time)
    {
        if (time < move_t)
        {
            sp.setPosition(sf::Vector2f(pos_x * 102.4 + 70 + ((x - pos_x) * 102.4 * time) / move_t,
            60 + pos_y * 102.4 + ((y - pos_y) * 102.4 * time) / move_t));
        }
        else
        {
            pos_x = x;
            pos_y = y;
            sp.setPosition(sf::Vector2f(pos_x * 102.4 + 70, pos_y * 102.4 + 60));
        }
    }
    virtual char type() = 0;
    void setPos(int x, int y)
    {
        pos_x = x;
        pos_y = y;
    }
    bool White()
    {
        return isWhite;
    }
    virtual void showMoves(piece* chessmen[][8], int moves[][8], int, int) = 0;
protected:
    int pos_x, pos_y;
    sf::Sprite sp;
    bool isWhite;
};

class pawn : public piece
{
    public:
        pawn(sf::Texture& t, int x, int y, bool f) :
        piece(t, x, y, f)
        {
            first_move = true;
            en_passant = false;
        }
        void showMoves(piece* chessmen[8][8], int moves[][8], int kx, int ky)
        {
            clearMoves(moves);
            int forward = (White() ? - 1 : 1);
            if (chessmen[pos_x][pos_y + forward] == nullptr && 
            !in_check(chessmen, pos_x, pos_y + forward, White(), pos_x, pos_y, kx, ky))
            {
                if ((pos_y + forward) == 7 || (pos_y + forward) == 0)
                    moves[pos_x][pos_y + forward] = 6;
                else
                    moves[pos_x][pos_y + forward] = 1;
            }
            if (first_move)
            {
                if (chessmen[pos_x][pos_y + forward] == nullptr && chessmen[pos_x][pos_y + forward * 2] == nullptr &&
                !in_check(chessmen, pos_x, pos_y + forward * 2, White(), pos_x, pos_y, kx, ky))
                {
                    moves[pos_x][pos_y + forward * 2] = 3;
                } 
            }
            if (pos_x > 0 && chessmen[pos_x - 1][pos_y + forward] != nullptr && 
            chessmen[pos_x - 1][pos_y + forward]->White() != this->White() && 
            !in_check(chessmen, pos_x - 1, pos_y + forward, White(), pos_x, pos_y, kx, ky))
            {
                if ((pos_y + forward) == 7 || (pos_y + forward) == 0)
                    moves[pos_x - 1][pos_y + forward] = 7;
                else
                moves[pos_x - 1][pos_y + forward] = 2;
            }
            if (pos_x < 7 && chessmen[pos_x + 1][pos_y + forward] != nullptr &&
            chessmen[pos_x + 1][pos_y + forward]->White() != this->White() && 
            !in_check(chessmen, pos_x + 1, pos_y + forward, White(), pos_x, pos_y, kx, ky))
            {
                if ((pos_y + forward == 7) || (pos_y + forward == 0))
                    moves[pos_x + 1][pos_y + forward] = 7;
                else
                moves[pos_x + 1][pos_y + forward] = 2;
            }
            if (en_passant)
            {
                moves[en_x][en_y + forward] = 4;
            }
        }
        void move(int x, int y, int time)
        {
            piece::move(x, y, time);
            if (first_move)
            {
                first_move = false;
            }
            if (en_passant)
            {
                en_passant = false;
            }
        }
        void check_en(piece* chessmen[8][8], int x, int y, int kx, int ky)
        {
            int forward = (White() ? - 1 : 1);
            if (x < 7 && chessmen[x + 1][y] != nullptr && chessmen[x + 1][y]->type() == 'p'
                && chessmen[x + 1][y]->White() != this->White())
            {
                piece* temp = chessmen[x + 1][y], *temp2 = chessmen[pos_x][pos_y];
                chessmen[pos_x][y - forward] = temp2, chessmen[pos_x][pos_y] = nullptr;
                chessmen[x + 1][y] = nullptr;
                bool check = in_check(chessmen, kx, ky, !White(), kx, ky);
                chessmen[x + 1][y] = temp; 
                chessmen[pos_x][pos_y] = temp2;
                chessmen[pos_x][y - forward] = nullptr;
                if (!check)
                {
                    pawn* pwn = (pawn*) chessmen[x + 1][y];
                    pwn->set_passant(x, y); 
                }
            }
            else if (x > 0 && chessmen[x - 1][y] != nullptr && chessmen[x - 1][y]->type() == 'p'
                && chessmen[x - 1][y]->White() != this->White())
            {
                piece* temp = chessmen[x - 1][y], *temp2 = chessmen[pos_x][pos_y];
                chessmen[pos_x][y - forward] = temp2, chessmen[pos_x][pos_y] = nullptr;
                chessmen[x - 1][y] = nullptr;
                bool check = in_check(chessmen, kx, ky, !White(), kx, ky);
                chessmen[x - 1][y] = temp; 
                chessmen[pos_x][y - forward] = nullptr;
                chessmen[pos_x][pos_y] = temp2;
                if (!check)
                {
                    pawn* pwn = (pawn*) chessmen[x - 1][y];
                    pwn->set_passant(x, y); 
                }
            }
        }
        char type()
        {
            return 'p';
        }
        void set_passant(int x, int y)
        {
            en_passant = true;
            en_x = x;
            en_y = y;
        }
        void set_passant()
        {
            en_passant = false;
        }
    private:
        bool en_passant;
        bool first_move;
        int en_x, en_y;
};

class rook : public piece
{
    public:
        rook(sf::Texture& t, int x, int y, bool f) :
        piece(t, x, y, f)
        {
            sp.move(sf::Vector2f(-2.0f, 1.0f));
            first_move = true;
        }
        void move(int x, int y, int time)
        {
            piece::move(x, y, time);
            sp.move(sf::Vector2f(-2.0f, 1.0f));
            if (first_move)
                first_move = false;
        }
        void showMoves(piece* chessmen[8][8], int moves[8][8], int kx, int ky)
        {
            clearMoves(moves);
            bool blocked = false;
            // Check left
            for (int i = pos_y - 1; !blocked && i >= 0; i--)
            {
                if (!in_check(chessmen, pos_x, i, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[pos_x][i] != nullptr)
                    {
                        if (chessmen[pos_x][i]->White() != this->White())
                        {
                            moves[pos_x][i] = 2;
                        }
                        blocked = true;
                    }
                    else
                    {
                        moves[pos_x][i] = 1;
                    }
                }
                else if (chessmen[pos_x][i] != nullptr)
                {
                    blocked = true;
                }
            }  
            // Check Right
            blocked = false;
            for (int i = pos_y + 1; !blocked && i < 8; i++)
            {
                if (!in_check(chessmen, pos_x, i, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[pos_x][i] != nullptr)
                    {
                        if (chessmen[pos_x][i]->White() != this->White())
                        {
                            moves[pos_x][i] = 2;
                        }
                        blocked = true;
                    }
                    else if (!in_check(chessmen, pos_x, i, White(), pos_x, pos_y, kx, ky))
                    {
                        moves[pos_x][i] = 1;
                    }
                }
                else if (chessmen[pos_x][i] != nullptr)
                {
                    blocked = true;
                }
            }
            // Check Up
            blocked = false;
            for (int i = pos_x - 1; !blocked && i >= 0; i--)
            {
                if (!in_check(chessmen, i, pos_y, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][pos_y] != nullptr)
                    {
                        if (chessmen[i][pos_y]->White() != this->White())
                        {
                            moves[i][pos_y] = 2;
                        }
                        blocked = true;
                    }
                    else
                    {
                        moves[i][pos_y] = 1;
                    }
                }
                else if (chessmen[i][pos_y] != nullptr)
                {
                    blocked = true;
                }
            }    
            // Check Down
            blocked = false;
            for (int i = pos_x + 1; !blocked && i < 8; i++)
            {
                if (!in_check(chessmen, i, pos_y, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][pos_y] != nullptr)
                    {
                        if (chessmen[i][pos_y]->White() != this->White())
                        {
                            moves[i][pos_y] = 2;
                        }
                        blocked = true;
                    }
                    else
                    {
                        moves[i][pos_y] = 1;
                    }
                }
                else if (chessmen[i][pos_y] != nullptr)
                {
                    blocked = true;
                }
            }    
        }
        char type()
        {
            return 'r';
        }
        bool f_move()
        {
            return first_move;
        }
    private:
        bool first_move;
};

class knight : public piece
{
    public:
        knight(sf::Texture& t, int x, int y, bool f) :
        piece(t, x, y, f)
        {
            sp.move(sf::Vector2f(-2.0f, 0.f));
        }
        void move(int x, int y, int time)
        {
            piece::move(x, y, time);
            sp.move(sf::Vector2f(-2.0f, 0.f));
        }
        void showMoves(piece* chessmen[8][8], int moves[8][8], int kx, int ky)
        {
            clearMoves(moves);
            int arr[][2] = 
            {
                {- 2, - 1},
                {- 2, 1},
                {- 1, - 2}, 
                {- 1, 2},
                {1, - 2},
                {1, 2},
                {2, - 1},
                {2, 1}
            };
            for (int i = 0; i < 8; i++)
            {
                if (pos_x + arr[i][0] <= 7 && pos_x + arr[i][0] >= 0 
                    && pos_y + arr[i][1] <= 7 && pos_y + arr[i][1] >= 0
                    && !in_check(chessmen, pos_x + arr[i][0], pos_y + arr[i][1], White(), pos_x, pos_y, kx, ky))
                {
                    if(chessmen[pos_x + arr[i][0]][pos_y + arr[i][1]] != nullptr)
                    {
                        if (chessmen[pos_x + arr[i][0]][pos_y + arr[i][1]]->White() != this->White())
                            moves[pos_x + arr[i][0]][pos_y + arr[i][1]] = 2;                   
                    }
                    else
                    {   
                        moves[pos_x + arr[i][0]][pos_y + arr[i][1]]= 1;
                    }
                }
            }
        }
        char type()
        {
            return 'n';
        }
};

class bishop : public piece
{
    public:
        bishop(sf::Texture& t, int x, int y, bool f) :
        piece(t, x, y, f)
        {
            sp.move(sf::Vector2f(-5.0f, 0.f));
        }
        void move(int x, int y, int time)
        {
            piece::move(x, y, time);
            sp.move(sf::Vector2f(-5.f , -2.f));
        }
        void showMoves(piece* chessmen[8][8], int moves[8][8], int kx, int ky)
        {
            clearMoves(moves);
            bool blocked = false;
            // Left Up
            for (int i = pos_x - 1, j = pos_y - 1; !blocked && i >= 0 && j >= 0; i--, j--)
            {
                if (!in_check(chessmen, i, j, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][j] != nullptr)
                    {
                        if (chessmen[i][j]->White() != this->White())
                            moves[i][j] = 2;                   
                        blocked = true;
                    }
                    else
                    {   
                        moves[i][j] = 1;
                    }
                }
                else if (chessmen[i][j] != nullptr)
                {
                    blocked = true;
                }
            }
            // Left Down
            blocked = false;
            for (int i = pos_x - 1, j = pos_y + 1; !blocked && i >= 0 && j < 8; i--, j++)
            {
                if (!in_check(chessmen, i, j, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][j] != nullptr)
                    {
                        if (chessmen[i][j]->White() != this->White())
                            moves[i][j] = 2;                   
                        blocked = true;
                    }
                    else
                    {   
                        moves[i][j] = 1;
                    }
                }
                else if (chessmen[i][j] != nullptr)
                {
                    blocked = true;
                }
            }
            // Right Up
            blocked = false;
            for (int i = pos_x + 1, j = pos_y - 1; !blocked && i < 8 && j >= 0; i++, j--)
            {
                if (!in_check(chessmen, i, j, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][j] != nullptr)
                    {
                        if (chessmen[i][j]->White() != this->White())
                            moves[i][j] = 2;                   
                        blocked = true;
                    }
                    else
                    {   
                        moves[i][j] = 1;
                    }
                }
                else if (chessmen[i][j] != nullptr)
                {
                    blocked = true;
                }
            }
            // Right Down
            blocked = false;
            for (int i = pos_x + 1, j = pos_y + 1; !blocked && i < 8 && j < 8; i++, j++)
            {
                if (!in_check(chessmen, i, j, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][j] != nullptr)
                    {
                        if (chessmen[i][j]->White() != this->White())
                            moves[i][j] = 2;                   
                        blocked = true;
                    }
                    else
                    {   
                        moves[i][j] = 1;
                    }
                }
                else if (chessmen[i][j] != nullptr)
                {
                    blocked = true;
                }
            }
        }
        char type()
        {
            return 'b';
        }
};

class queen : public piece
{
    public:
        queen(sf::Texture& t, int x, int y, bool f) :
        piece(t, x, y, f)
        {
            sp.move(sf::Vector2f(-10.0f, 0.f));
        }
        void move(int x, int y, int time)
        {
            piece::move(x, y, time);
            sp.move(sf::Vector2f(-10.0f, 0.f));
        }
        void showMoves(piece* chessmen[8][8], int moves[8][8], int kx, int ky)
        {
            clearMoves(moves);
            bool blocked = false;
            // Check left
            for (int i = pos_y - 1; !blocked && i >= 0; i--)
            {
                if (!in_check(chessmen, pos_x, i, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[pos_x][i] != nullptr)
                    {
                        if (chessmen[pos_x][i]->White() != this->White())
                        {
                            moves[pos_x][i] = 2;
                        }
                        blocked = true;
                    }
                    else
                    {
                        moves[pos_x][i] = 1;
                    }
                }
                else if (chessmen[pos_x][i] != nullptr)
                {
                    blocked = true;
                }                
            }  
            // Check Right
            blocked = false;
            for (int i = pos_y + 1; !blocked && i < 8; i++)
            {
                if (!in_check(chessmen, pos_x, i, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[pos_x][i] != nullptr)
                    {
                        if (chessmen[pos_x][i]->White() != this->White())
                        {
                            moves[pos_x][i] = 2;
                        }
                        blocked = true;
                    }
                    else
                    {
                        moves[pos_x][i] = 1;
                    }
                }
                else if (chessmen[pos_x][i] != nullptr)
                {
                    blocked = true;
                }
            }
            // Check Up
            blocked = false;
            for (int i = pos_x - 1; !blocked && i >= 0; i--)
            {
                if (!in_check(chessmen, i, pos_y, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][pos_y] != nullptr)
                    {
                        if (chessmen[i][pos_y]->White() != this->White())
                        {
                            moves[i][pos_y] = 2;
                        }
                        blocked = true;
                    }
                    else
                    {
                        moves[i][pos_y] = 1;
                    }                
                }
                else if (chessmen[i][pos_y] != nullptr)
                {
                    blocked = true;
                }
            }    
            // Check Down
            blocked = false;
            for (int i = pos_x + 1; !blocked && i < 8; i++)
            {
                if (!in_check(chessmen, i, pos_y, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][pos_y] != nullptr)
                    {
                        if (chessmen[i][pos_y]->White() != this->White())
                        {
                            moves[i][pos_y] = 2;
                        }
                        blocked = true;
                    }
                    else
                    {
                        moves[i][pos_y] = 1;
                    }
                }
                else if (chessmen[i][pos_y] != nullptr)
                {
                    blocked = true;
                }
            }
            blocked = false;
            // Left Up
            for (int i = pos_x - 1, j = pos_y - 1; !blocked && i >= 0 && j >= 0; i--, j--)
            {
                if (!in_check(chessmen, i, j, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][j] != nullptr)
                    {
                        if (chessmen[i][j]->White() != this->White())
                            moves[i][j] = 2;                   
                        blocked = true;
                    }
                    else
                    {   
                        moves[i][j] = 1;
                    }
                }
                else if (chessmen[i][j] != nullptr)
                {
                    blocked = true;
                }                
            }
            // Left Down
            blocked = false;
            for (int i = pos_x - 1, j = pos_y + 1; !blocked && i >= 0 && j < 8; i--, j++)
            {
                if (!in_check(chessmen, i, j, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][j] != nullptr)
                    {
                        if (chessmen[i][j]->White() != this->White())
                            moves[i][j] = 2;                   
                        blocked = true;
                    }
                    else
                    {   
                        moves[i][j] = 1;
                    }
                }
                else if (chessmen[i][j] != nullptr)
                {
                    blocked = true;
                }
            }
            // Right Up
            blocked = false;
            for (int i = pos_x + 1, j = pos_y - 1; !blocked && i < 8 && j >= 0; i++, j--)
            {
                if (!in_check(chessmen, i, j, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][j] != nullptr)
                    {
                        if (chessmen[i][j]->White() != this->White())
                            moves[i][j] = 2;                   
                        blocked = true;
                    }
                    else
                    {   
                        moves[i][j] = 1;
                    }
                }
                else if (chessmen[i][j] != nullptr)
                {
                    blocked = true;
                }                
            }
            // Right Down
            blocked = false;
            for (int i = pos_x + 1, j = pos_y + 1; !blocked && i < 8 && j < 8; i++, j++)
            {
                if (!in_check(chessmen, i, j, White(), pos_x, pos_y, kx, ky))
                {
                    if (chessmen[i][j] != nullptr)
                    {
                        if (chessmen[i][j]->White() != this->White())
                            moves[i][j] = 2;                   
                        blocked = true;
                    }
                    else
                    {   
                        moves[i][j] = 1;
                    }
                }
                else if (chessmen[i][j] != nullptr)
                {
                    blocked = true;
                }                
            }
        }
        char type()
        {
            return 'q';
        }
};

class king : public piece
{
    public:
        king(sf::Texture& t, int x, int y, bool f) :
        piece(t, x, y, f)
        {
            first_move = true;
            check = false;
            sp.move(sf::Vector2f(-6.f, 0.f));
        }
        void move(int x, int y, int time)
        {
            piece::move(x, y, time);
            sp.move(sf::Vector2f(-6.0f, 0.f));
            if (first_move)
            {
                first_move = false;
            }
        }
        void showMoves(piece* chessmen[8][8], int moves[8][8], int kx, int ky)
        {
            clearMoves(moves);
            for (int i = (0 > pos_x - 1 ? 0 : pos_x - 1); i < pos_x + 2 && i < 8; i++)
            {
                for (int j = (0 > pos_y - 1 ? 0 : pos_y - 1); j < pos_y + 2 && j < 8; j++)
                {
                    if (j == pos_y && i == pos_x)
                        continue;
                    if (chessmen[i][j] != nullptr && chessmen[i][j]->White() != this->White())
                    { 
                        bool check = in_check(chessmen, i, j, White(), pos_x, pos_y);
                        if (!check)
                            moves[i][j] = 2;
                    }
                    else if (chessmen[i][j] == nullptr)
                    {
                        bool check = in_check(chessmen, i, j, White(), pos_x, pos_y);
                        if (!check)
                            moves[i][j] = 1;
                    }
                }
            }
            if (first_move && !check)
            {
                if (chessmen[pos_x + 1][pos_y] == nullptr && chessmen[pos_x + 2][pos_y] == nullptr
                    && chessmen[pos_x + 3][pos_y] != nullptr && chessmen[pos_x + 3][pos_y]->type() == 'r')
                {
                    rook *temp = (rook*) chessmen[pos_x + 3][pos_y];
                    if (!in_check(chessmen, pos_x + 1, pos_y, White(), pos_x, pos_y) &&
                        !in_check(chessmen, pos_x + 2, pos_y, White(), pos_x, pos_y) &&
                        temp->f_move())
                    {
                        moves[pos_x + 2][pos_y] = 5;
                    }
                }
                if (chessmen[pos_x - 1][pos_y] == nullptr && chessmen[pos_x - 2][pos_y] == nullptr
                    && chessmen[pos_x - 3][pos_y] == nullptr && chessmen[pos_x - 4][pos_y] != nullptr && 
                    chessmen[pos_x - 4][pos_y]->type() == 'r')
                {
                    rook *temp = (rook*) chessmen[pos_x - 4][pos_y];
                    if (!in_check(chessmen, pos_x - 1, pos_y, White(), pos_x, pos_y) &&
                        !in_check(chessmen, pos_x - 2, pos_y, White(), pos_x, pos_y) &&
                        temp->f_move())
                    {
                        moves[pos_x - 2][pos_y] = 5;
                    }
                }
            }
        }
        bool attack(piece* chessmen[8][8])
        {
            check = in_check(chessmen, pos_x, pos_y, White(), pos_x, pos_y);
            return check;
        }
        char type()
        {
            return 'k';
        }
    private:
        bool check;
        bool first_move;
};

void clear_en(piece *chessmen[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (chessmen[i][j] != nullptr && chessmen[i][j]->type() == 'p')
            {
                pawn* pwn = (pawn*) chessmen[i][j];
                pwn->set_passant();
            }
        }
    }
}

void clearMoves(int moves[][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            moves[i][j] = 0;
        }
    }   
}

bool in_check(piece* chessmen[8][8], int x, int y, bool w, int x0, int y0, int kx, int ky)
{
    piece* temp = chessmen[x][y], *temp2 = chessmen[x0][y0];
    chessmen[x][y] = temp2, chessmen[x0][y0] = nullptr;
    bool check = in_check(chessmen, kx, ky, w, kx, ky);
    chessmen[x][y] = temp; 
    chessmen[x0][y0] = temp2;
    return check;
}

bool in_check(piece* chessmen[8][8], int x, int y, bool w, int x0, int y0)
{
    piece* temp = chessmen[x0][y0];
    chessmen[x0][y0] = nullptr;
    bool check = false;
    int forward = (w ? - 1 : 1);
    // King
    for (int i = (0 > x - 1 ? 0 : x - 1); !check && i < x + 2 && i < 8; i++)
    {
        for (int j = (0 > y - 1 ? 0 : y - 1); !check && j < y + 2 && j < 8; j++)
        {
            if (j == y && i == x)
                continue;
            if (chessmen[i][j] != nullptr && chessmen[i][j]->type() == 'k')
            {   
                check = true;
            }
        }
    }
    // Check if not last row
    int l_row = (temp->White() ? 0 : 7);
    
    // Pawn
    if (y != l_row && x > 0 && chessmen[x - 1][y + forward] != nullptr && 
        chessmen[x - 1][y + forward]->type() == 'p' && chessmen[x - 1][y + forward]->White() != w)
    {
        check = true;
    }
    else if (y != l_row && x < 7 && chessmen[x + 1][y + forward] != nullptr && 
    chessmen[x + 1][y + forward]->type() == 'p' && chessmen[x + 1][y + forward]->White() != w)
    {
        check = true;
    }
    // Knight
    int arr[][2] = 
    {
        {- 2, - 1},
        {- 2, 1},
        {- 1, - 2}, 
        {- 1, 2},
        {1, - 2},
        {1, 2},
        {2, - 1},
        {2, 1}
    };
    for (int i = 0; !check && i < 8; i++)
    {
        if (x + arr[i][0] <= 7 && x + arr[i][0] >= 0 
            && y + arr[i][1] <= 7 && y + arr[i][1] >= 0)
        {
            if(chessmen[x + arr[i][0]][y + arr[i][1]] != nullptr && chessmen[x + arr[i][0]][y + arr[i][1]]->type() == 'n')
            {
                if (chessmen[x + arr[i][0]][y + arr[i][1]]->White() != w)
                    check = true;                 
            }
        }
    }
    // Straight (Rook / Queen)
    bool blocked = false;
    // Check left
    for (int i = y - 1; !check && !blocked && i >= 0; i--)
    {
        if (chessmen[x][i] != nullptr)
        {
            if (chessmen[x][i]->White() != w && 
            (chessmen[x][i]->type() == 'r' || chessmen[x][i]->type() == 'q'))
            {
                check = true;
            }
            blocked = true;
        }
    } 
    // Check Right
    blocked = false;
    for (int i = y + 1; !check && !blocked && i < 8; i++)
    {
        if (chessmen[x][i] != nullptr)
        {
            if (chessmen[x][i]->White() != w && 
            (chessmen[x][i]->type() == 'r' || chessmen[x][i]->type() == 'q'))
            {
                check = true;
            }
            blocked = true;
        }
    }
    // Check Up
    blocked = false;
    for (int i = x - 1; !check && !blocked && i >= 0; i--)
    {
        if (chessmen[i][y] != nullptr)
        {
            if (chessmen[i][y]->White() != w && 
            (chessmen[i][y]->type() == 'r' || chessmen[i][y]->type() == 'q'))
            {
                check = true;
            }
            blocked = true;
        }
    }    
    // Check Down
    blocked = false;
    for (int i = x + 1; !check && !blocked && i < 8; i++)
    {
        if (chessmen[i][y] != nullptr)
        {
            if (chessmen[i][y]->White() != w && 
            (chessmen[i][y]->type() == 'r' || chessmen[i][y]->type() == 'q'))
            {
                check = true;
            }
            blocked = true;
        }
    }

    // Diagonals (Bishop / Queen)
    blocked = false;
    // Left Up
    for (int i = x - 1, j = y - 1; !check && !blocked && i >= 0 && j >= 0; i--, j--)
    {
        if (chessmen[i][j] != nullptr)
        {
            if (chessmen[i][j]->White() != w &&
            (chessmen[i][j]->type() == 'b' || chessmen[i][j]->type() == 'q'))
            {
                check = true;
            }                     
            blocked = true;
        }
    }
    // Left Down
    blocked = false;
    for (int i = x - 1, j = y + 1; !check && !blocked && i >= 0 && j < 8; i--, j++)
    {
        if (chessmen[i][j] != nullptr)
        {
            if (chessmen[i][j]->White() != w &&
            (chessmen[i][j]->type() == 'b' || chessmen[i][j]->type() == 'q'))
            {
                check = true;
            }                     
            blocked = true;
        }
    }
    // Right Up
    blocked = false;
    for (int i = x + 1, j = y - 1; !blocked && i < 8 && j >= 0; i++, j--)
    {
        if (chessmen[i][j] != nullptr)
        {
            if (chessmen[i][j]->White() != w &&
            (chessmen[i][j]->type() == 'b' || chessmen[i][j]->type() == 'q'))
            {
                check = true;
            }                     
            blocked = true;
        }
    }
    // Right Down
    blocked = false;
    for (int i = x + 1, j = y + 1; !blocked && i < 8 && j < 8; i++, j++)
    {
        if (chessmen[i][j] != nullptr)
        {
            if (chessmen[i][j]->White() != w &&
            (chessmen[i][j]->type() == 'b' || chessmen[i][j]->type() == 'q'))
            {
                check = true;
            }                     
            blocked = true;        
        }
    }   
    chessmen[x0][y0] = temp;
    return check;
}

int valid_moves(piece* chessmen[8][8], bool turn, int kx, int ky)
{
    /*
    0: no check
    1: check
    2: checkmate
    3: stalemate 
    */
    int end_type = 0;
    int moves[8][8] = {};
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (chessmen[i][j] != nullptr && chessmen[i][j]->White() == turn)
            {
                chessmen[i][j]->showMoves(chessmen, moves, kx, ky);
                for (int k = 0; k < 8; k++)
                {
                    for (int l = 0; l < 8; l++)
                    {
                        if (moves[k][l] != 0)
                        {
                            king* k = (king*) chessmen[kx][ky];
                            if (k->attack(chessmen))
                            {
                                end_type = 1;
                            }
                            return end_type;
                        }
                    }
                }
            }
        }
    }
    king* k = (king*) chessmen[kx][ky];
    if (k->attack(chessmen))
    {
        end_type = 2;
    }
    else
    {
        end_type = 3;
    }
    return end_type;   
}

void record_game(std::string s, std::vector<std::string> record, int t_move, int turn)
{
    std::ofstream outfile(s, std::ios::trunc);
    if (!outfile)
    {
        std::cout << "Error creating file\n";
        return; 
    }
    std::string tmp;
    for (int i = 0; i < record.size(); i++)
    {
        if (i % 2 == 0)
        {
            tmp = std::to_string(i / 2 + 1) + ". " + record.at(i);
        }
        else
        {
            tmp += "\t\t" + record.at(i) + '\n';
            outfile << tmp;
        }
    }
    if (record.size() % 2)
        outfile << tmp << "\n";
    if (t_move == 2)
    {
        if (turn)
            outfile << "0-1\n";
        else 
            outfile << "1-0\n";
    }
    else if (t_move == 3)
    {
        outfile << "1/2 - 1/2\n";
    }
    outfile.close();
}
