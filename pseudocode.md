#Battleship pseudocode

Adaship_config.ini
Board: 10x10
Boat: Carrier, 5
Boat: Battleship, 4
Boat: Destroyer, 3
Boat: Submarine, 3
Boat: Patrol Boat, 2

## BoardClass
```diff
Extra
! Board ranges from 5 to 80 depending on .ini values.
```
Lines have numbers, columns have letters
- drawOwnBoard() shows own boats + hits (red) and misses (white)
- drawEnemyBoard() (shows hits (green) and misses (white) )

## Cell
- Type: W (water) or ship (CBDSP)
- State (bool): fired or not


## ShipsClass
Object that contains the ships
	placedShips: [CCCCC, BBBB], unplacedShips: [DDD, SSS, PP]
- updateShipsCounter() (if one full ship is down, ship counter -1 + display “ship whatever has been sunk”)
- notifySunkShip();
- positionShips()
  - displayAvailableShips()
  - User selects ship
  - User write starting coordinates
  - User select direction
    - If valid, place ship
    - If not valid, ask for new coordinates or quite
    - Repeat until all boats are placed.
  - If ships remaining, autoposition the rest 
- autoPositionShips()
  - For each ship
    - While !shipPositioned
      - Select random direction + random coordinate
      - Try to fit. If it fits, shipPositioned = true, move ship to placed
- initiatiateShipCounter() If ship counter == 0

## GameClass
- loadGame(from ini file)
- startPlayers() (user player, automated player)
- positionShips()
- initiatiateShipCounter() If ship counter == 0
- Menu
  - One player vs computer
  - Quit
```diff
Extra
!  - Two player game; essentially replacing the computer with a second player
!  - One player v computer (salvo) game
!  - Two player game (salvo) game
```
- gameStarts 
  - User can place their own ships (placing ships manually menu)
  - Loop through players turn/computer turn
    - Fire torpedo at (user input)

## PlayerClass
- createBoard()
- positionShips() //randomly 
- initiatiateShipCounter() If ship counter == 0 this player lost

```diff
Extra
! The salvo implementation updates the basic game play by allowing the current ‘player’ (player or computer) to ‘fire’ one torpedo per their remaining ships. For example, if the ‘player’ has three ‘non-destroyed’ ships instead of a single valid coordinate (e.g., F2) they could enter F2 E2 G2 (one coordinate per ship) – once entered, details on any ‘hits’ and/or ‘misses’ are clearly provided, and all appropriate boards are updated to reflect this salvo.
```
