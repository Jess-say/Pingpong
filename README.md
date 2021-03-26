# Classic Pingpong 

Author: \<[Jessie Lu](https://github.com/Jess-say)\>

## Equipment
* Large breadboard kit
* ATMEGA1284P 
* ATAMEL-ICE (microchip)
* 5x8 LED matrix
* LED lights
* Switch tactile (buttons)
* 330 ohms resistors
* Jumper kit (wires)

## Basic Requirements
There are two sets of paddles: user's paddle and AI's paddle. The user can control their paddle with buttons designated for movements going up or down. The AI is controlled by a random number generator that determines the direction it goes in. The ball will start off in the middle row and head towards the player's paddle. If the ball hits the center of the paddle it will bounce and continue going straight towards the opposite paddle. If the ball hits the top corner of the paddle it will bounce upwards and if the ball hits the bottom corner of the paddle then the ball will bounce downwards. When the ball misses the paddle it will reset back to the initial start.

Note: The upcoming parts are separate and do not contain elements of other parts
## Part 1
This part will contain a "spin" of the ball. The ball will speed up or slow down according to where it hits the paddle. The ball should slow down when it hits the center of the paddle and speed up when it hits the corners of the paddle. It should also take into account if the paddle gets moved right when the ball hits the paddle. 

## Part 2
A menu is added that allows the player to choose to play against an AI or a second player. A player must press the start button then choose an option for the opposing player before the game begins. The LED will light up differently depending on which option gets chosen. The second player can be controlled using a different set of buttons. There is also a LED scoreboard that allows you to see how many points have been accumulated. The first player to reach 3 points wins the game. Another button restarts the game once one player wins or anytime in between. 

## Part 3
A more complicated AI opponent is implemented. This means that it is harder for the user to win against the AI than it was in part 1.
