# Tetris-CLI
 A Tetris game running in the command line, made using ncurses

 https://github.com/user-attachments/assets/2a82e0bc-705a-4330-8def-ff470b19d39d


## Installation
 Go to the [Releases](https://github.com/Milan-Torres/Tetris-CLI/releases) section and download the latest binary. Afterwards, open a terminal and run the program

 > [!WARNING]  
> Don't forget to make the binary executable on your machine !
> You can do so by running `chmod +x tetris-cli`

## Features
 ### Stash
  You can stash dropping pieces, they are then shown in the "Stashed" section. For more information see [Stashing](#stashing).

  
  ![image](https://github.com/user-attachments/assets/69cbcd52-072b-4eb4-a289-df57b75741e9)

 ### Score
  The score is displayed and updated in real time. For more information see [Scoring](#scoring).

  
 ![image](https://github.com/user-attachments/assets/fc7d5535-6e59-44c3-802d-067f4f9aa5fd)

 ### Highscore
 The global high score is saved locally on game exit and displayed/updated in real time when playing.

 
  ![image](https://github.com/user-attachments/assets/d9f3a2a4-9356-4986-b659-764411415299)

### Speed management
 - You can adjust the gamespeed by passing a number after the executable name when launching it
 - The default speed is at about 1.18 pixel/s (= 0.85s between each fall step)
 - When you provide a specific speed S, the time between each fall step is set to 5/S (therefore the speed is S/5 pixel/s)
 - The time between each fall step is capped at 1 second
 - Thus, plugging 5 or below as an argument freezes the game (which is useful for debugging)

### Adaptability
 The UI size is fully responsive and adapts to the window size in REAL TIME while playing

## Gameplay
 ### Generalities
  - The board is 10 pixels wide and 20 pixels tall
  - Those properties can be easily changed in the game source code as they are just 2 constants, they might be added as parameters when launching the game in the future
 ### Basic movement
  - You can press \<A\> to make your current piece go left and \<D\> to make it go right
  - You can press \<S\> to fast drop and \<W\> to instant drop
  - To rotate a piece (counter-clockwise), press \<Q\>
  - You can quit the game at any time by pressing \<P\>
 ### Stashing
  - You can stash a dropping piece by pressing \<E\>
  - You can stash the currently falling piece at any time, it will then disappear, and a new piece will spawn at the top
  - The new piece is either your last stashed piece or the next piece if no piece has been stashed yet
  - You can "stash" even if your stash contains the piece you're already holding, in that case your piece is simply sent to the top
  - ⚠ You can only stash a piece once, afterwards, you have to wait for the currently falling piece to land

 ### Scoring
  - You get 5 points every time a block successfully lands
  - Every time you clear lines, you get a specific amount of points depending on the amount of lines cleared in one move.

    When you clear a line, a combo starts, it ends as soon as you drop a piece without clearing any line.
    The amount of points won by line clear is multiplied by the current combo length

    | Lines Cleared    | 1     | 2     | 3     | 4     |
    | :---:            | :---: | :---: | :---: | :---: |
    | Points (no combo)|  40   | 100   | 300   | 1200  |
    | Points (1 combo) |  80   | 200   | 600   | 2400  |
    | Points (2 combo) |  120  | 300   | 900   | 3600  |
    | Points (3 combo) |  160  | 400   | 1200  | 4800  |
    | Points (X combo) | 40×X  | 100×X | 300×X | 1200×X|

## Debugging
 Pressing \<L\> allows you to change the current piece type, this is useful for testing things!
