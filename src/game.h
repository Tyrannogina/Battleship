#ifndef GAME_H
#define GAME_H
 
class Game
{
private:
  int menuSelection;

  void readMenuSelection();
  void displayWinner();
  void displayMenu();

public:
  Game();
 
  void startGame();
  void endGame();
};
 
#endif