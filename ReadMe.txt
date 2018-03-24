//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

The exe should work on any windows platform. For other platforms, you must compile it yourself and create a makefile. 

To compile:

There are two projects.
RakNet
TBT

RakNet is a STATIC library. TBT depends on RakNet for the game to work.
Build RakNet, then build TBT.
The RakNet.lib is outputted to solution directory\lib\configuration directory
Included should be the game compiled for release. To play the game, OpenGLUT.dll must be in the same directory as the exe. The exe requires OpenGLUT and no other files.

To play the game, one player must create the game, and one player must join the game.
It is currently a two player only game.

When a player has joined the game, the game creator will see the player show up on their menu. The other player will not know if they're in the game lobby or not.

This game currently only works over LAN, it has no functionality to play over internet.
It finds local game hosts by pinging the local subnet. A game ready for a player will ping back it's information. 

In the game, use WASD to move around. When it is your turn, you will have an aim box. This is the current direction your player is facing.
When it is not your turn, you will lose the aim box.

In the game, use RIGHT MOUSE button to lay a sentry. The sentry will be layed in the box adjacent to your current position in the aim direction.

All sentries have an aim box which shows their range and firing direction. 

Any player going into range of a sentry, friendly or foe, will be attacked for one damage.

The player will be knocked back by the summation of all previous damage recieved.

The last remaining player in the arena is the winner. 

Upon win/lose, output will be written to the console telling the player if they have won or lost, and they will be returned to the menu system.

To play again, the game must be restarted. 
