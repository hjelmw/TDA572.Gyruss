# Gyruss
Gyruss is an arcade game by Konami from the 1980s. This is my implementation of the game in C++ and SDL2 . The game was built using a _component_ pattern.

The engine, `AvancezLib`, was built as part of the course _TDA572 - Game Engine Architecture_ at Chalmers University of Technology and features:

* Sprite rendering and rotation
* Audio playback
* Basic collision detection


## Gameplay description (short)
* Shoot aliens 
* Don't get hit by the aliens
* The aliens get more aggresive every level
* Powerups give life and makes your shots pierce aliens
* Aliens 👽

## Gameplay description (longer)
The enemies are spaceships which the player must destroy to progress to the next level. The aliens appear near the middle of the screen and move in swirling patterns before landing in the middle and start to shoot at the player. The player has 4 lives and gains invulnerability for a small time if hit directly by an alien or anything from its arsenal. On each level the aliens increase in number and get more aggresive. There are also powerups that spawn during gameplay. Picking one up grants the player one life, invulnerability for a short time and causes their shots to pierce aliens.

## Gameplay
![Gyruss gif](https://github.com/hjelmw/TDA572.Gyruss/blob/master/img/Gyruss%20-%20Gameplay.gif)


## Setup
Setup SDL2 [here](https://www.wikihow.com/Set-Up-SDL-with-Visual-Studio)

## Credits
See [CREDITS.txt](https://github.com/hjelmw/TDA572.Gyruss/blob/master/CREDITS.txt)
