#ifndef BOARD_H
#define BOARD_H
 
class Board
{
private:
    int width;
    int height;
 
public:
    Board();
 
    void parseConfig();
    void setWidth();
    void setHeight();
};
 
#endif