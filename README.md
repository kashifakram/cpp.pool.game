# cpp.poolgame.stage3



Object Oriented Design Patterns
Assignment 3
Kashif Akram – 460458380

Extensions
I used two extensions, one is different modes of game and other is complete the game.

Single Player Mode will be just current game with complete status where game will have strip and full color balls and upon pushing all balls to pockets or cueue ball in pocket game will finish (completed).

Two players mode will be for two players, where each player can hit cueue ball on their turn only and player name will be displayed on game header, each player will be assigned to specific ball type and foul will be given if one player touches ball of other ball, in case of foul other player will have two turns instead of one and upon pushing cueue ball to pocket other player will win by default and game will finish, other condition of completing game is one player push all of his balls in pockets following by black ball number 8 at end, very last.

Received Code:
1.	Code Design
I worked on Code B and found that code was designed well for extension, proper implementation of factory, builder and decorator design patterns

Though there were some bugs initially which were fixed soon after receiving code and proper explanation were given regarding those bugs and their fix.

2.	Hindered Elements
I found many decorators implemented on ball as hindering in extension as we have to implement Memento design pattern as core part of this extension and it required to save and return current state of each object of game which means I have to save all balls states, table states and additionally all decorators states implemented on balls as well. And same goes to revert to previous state.

3.	Documentation
There was no external documentation provided about this code, so no idea about that but I found internal documentation is written for every method, very concise and easily understandable. Doxygen comments and additional one-line comments were provided where required.

4.	Code Implementation
Coding is done professionally and easy to extend. This code is implemented in very hierarchical way which makes is easy to understand and extend.

5.	What would I have done different?
I think SOLID principles were not well implemented, for example each class should have its own purpose, but I found many different classes implementations in single class file. For example, ball file has ball abstract class, stageone concrete class and composite concrete classes and builder has both stages builders as well as director’s implementation.

I would have separated files with proper file names for stageoneball, composite ball, abstract ball, stage one builder, stage two builder and director.

6.	Code Style
I think files, variable, class, function names are consistent and make sense, you can understand the purpose of each of them by their names. Code was written as if it was written by professional and experienced developer, without any unused functions and variable.

Documentation: Doxygen comments for methods/classes as well as in-code comments where appropriate. (10%) 
The marking (out of 5 marks) for the essay section is as follows 

Design Patterns Used:
1.	Memento
Application of Memento design pattern:
•	Memento is used to save objects state and to retrieve back  
•	Useful for undoing application last state
•	No need to originate object to revert to its previous state
Memento implementation in my code:
I used Memento design pattern to save current state of game and to undo recent move by players. 
Advantages of Memento in my code:
•	I used memento to save game objects state on start of game and every time player move cue ball 
•	I reverted back objects states saved by memento on the event of key release, when player click r key at any stage of game they will move to previous state of game


Disadvantages of Memento in my code:
•	I had to deal with overhead of saving a lot of data for large objects (all balls, table, players info etc)
•	There can be unseen costs related to save heavy objects memento if this game gets extended and make use of images for example

2.	Mediator
Application of Mediator design pattern:
•	This design pattern manage interaction between different type of objects and establish loose coupling between them.
•	Very useful to implement in situations where reusing objects is difficult and will make coding very complex.

Mediator implementation in my code:
Mediator design pattern is used to make communication possible between game engine and cue ball to make save state possible by saving players turns in multi game mode and saving complete state of game after cue ball hits other balls

Advantages of Mediator in my code:
•	I used mediator design pattern to manage interaction between cue ball object type and game object type which are very different in nature.
•	This saved me a lot of time and effort to change the cue ball class and game engine class which are very complex and cue ball has decorator implementation which means I had to deal with those decorator as well, but using mediator I didn’t have to touch any of  these classes.
Disadvantages of Mediator in my code:
•	I think the only disadvantage of mediator could be that it can become complex itself consequently hard to manage in later stages of project, which is not the case for me (at least for now).








 
