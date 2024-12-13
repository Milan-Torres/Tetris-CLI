# Tetris-CLI
 A Tetris Game running in the command line made using ncurse

 https://github.com/user-attachments/assets/2a82e0bc-705a-4330-8def-ff470b19d39d


## Features
 ### Stash
  You can stash dropping pieces, they are then shown in the "Stashed" tab, for more information see [Stashing](#stashing)
  
  ![image](https://github.com/user-attachments/assets/69cbcd52-072b-4eb4-a289-df57b75741e9)

 ### Score
  The score is displayed and updated in real time.
  For more information about scoring see [Scoring](#scoring)
  
  ![image](https://github.com/user-attachments/assets/fc7d5535-6e59-44c3-802d-067f4f9aa5fd)

 ### Highscore 
  The global highscore is saved locally and displayed/updated in real time when playing.
 
  ![image](https://github.com/user-attachments/assets/d9f3a2a4-9356-4986-b659-764411415299)

### Speed management
 - You can ajust the gamespeed by passing a number after the executable name when launching it.
 - The default speed is at about 1.18 pixel/s (= 0.85s between each fall step)
 - When you provide a specific speed x, the time between each fall step is set as 5/x (so the speed is of x/5 pixel/s)
 - The time between each fall step is capped at 1 second
 - For that reason, passing 5 or below as an argument froze the game (which is useful for debugging)  

## Gameplay  
 ### Basic movement
  - You can press \<A\> to make your current piece go left and \<D\> to make it go right
  - You can press \<S\> to fast drop and \<W\> to instant drop
  - To rotate a piece (counter-clockwise), press \<Q\>
  - You can quit the game at anytime by pressing \<P\>
 ### Stashing
   - While a piece is dropping you can stash it by clicking \<E\>
  - You can stash at any time, the piece you're currently will then disappear and a new piece will spawn at the top.
  - This new piece is either your last stashed piece or the next piece if no piece as been stashed yet.
  - You can "stash" even if your stash contains the piece you're currently holding, in that case your piece is simply sent at to the top.
  - ⚠ You can only stash a piece once, afterwards you are forced to let down the piece currently falling.

 ### Scoring
  You get 5 points every time a block sucessfully fall.
  Every time you clear lines you get a specific amount of points which depends on the number of lines cleared in one move.

  If you just cleared a line a combo starts, it ends as soon as you drop a piece without clearing any line.
  The amount of points won by line clear is multiplied by the current combo length
  
  | Lines Cleared        | 1     | 2     | 3     | 4     |
  | :---:                | :---: | :---: | :---: | :---: |
  | Points (no combo)    |   40  | 100   |  300  | 1200  | 
  | Points (1 combo)     |   80  | 200   |  600  | 2400  | 
  | Points (2 combo)     |   120 | 300   |  900  | 3600  | 
  | Points (3 combo)     |   160 | 400   |  1200 | 4800  |
  | Points (X combo)     |  40×X | 100×X |  300×X| 1200×X|

## Debugging
 Pressing \<L\> allows you to change the current piece type, this is useful for testing things!
