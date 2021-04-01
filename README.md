# Battleship

## Challenge outline
### Summary
The goal of this repository is to create a battleship game, playable from the terminal and developed in C++. The game will be defined by a config file, which will contain information of the board size and the ships that will be placed on the board. In the 'Player vs. computer' implementation, the player can select how to place the ships on the board, or place them automatically if they choose. When all ships are placed, they can play against the computer, and fire torpedoes into the enemy board. Whoever sinks all of the other player's ships wins. 

[A running version can be found in Repl.it](https://replit.com/@SaraLatorre/Battleship)

### Initial working plan
First I drafted a [plan in pseudocode](pseudocode.md) regarding the general structure and implementation. Please check the plan before continuing reading this document.
 
 If I had known the language better, it could have been a more accurate plan, but I ended up applying several changes to the initial draft as I became acquainted with the in and outs of C++ and its limitations. This first draft is broken down into small tasks, as it defines classes and methods. 

### Overall approach and development strategy
For organising the development of the project, I created a list of epics divided in user stories. I developed the first epic on a separate branch, `game-v1`, that was then merged through a pull request into main. I made commits often, at least 1 per day. 

#### Epics:
- [x] First version of the game working
    ##### User stories:
    - [x] User can see the menu and select “One player vs Computer” or “Quit”
    - [x] User can start the game and place their ships (defined in the .ini file).
    - [x] User can skip placing their ships and ships will be automatically placed.
    - [x] User can see their board and the computer’s board 
    - [x] User can “shoot” to the other board and hits get displayed on the enemy board
    - [x] User can get hit by enemy torpedoes ant that gets displayed on the user’s board
    - [x] User can see score in every turn
    - [x] When one of the players loses all their ships, the game ends and displays a victory (or not) message
    - [x] User can see error messages and have choices for any unrecognised inputs.
- [ ] Two player game
    ##### User stories:
    - [ ] User can play the game with another player and each turn will clearly display whose turn is it.
    - [ ] Second player will be able to place ships and see updates to their board same as first player.
- [ ] One player v computer (salvo) game
    ##### User stories:
    - [ ] User can play salvo game, where they can shoot a torpedo for each non-destroyed ship on their board.
    - [ ] User can clearly see the updates for all shots on the boards 
- [ ] Two player game (salvo) game
    ##### User stories:
    - [ ] Second player will be able to do as first player in a salvo vs computer game.
  

