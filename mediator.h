#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "balldecorator.h"

class Game;
/**
 * @brief The Mediator class is used to manage communcation between cueball decorator and game engine.
 */
class Mediator
{
public:
    /**
      * function to returning static mediator instance reference
    */

    static Mediator &instance();


    /**
      * function to m_game to given game object
    */
    void setGame(Game *game);


    /**
      * function to to call on cue  ball hit
    */
    void cueBallHit();

private:


    Mediator() = default;

    Game *m_game;
};

#endif // MEDIATOR_H
