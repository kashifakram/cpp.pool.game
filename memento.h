#ifndef MEMENTO_H
#define MEMENTO_H

#include <vector>

//to avoid circular reference

class Game;
class Ball;

class Memento
{

public:
    /**
      Constructor
    */
    Memento(){}

    /**
      Destructor
    */
    ~Memento();

    void saveGameSate(Game *game);
    Game* getGameState();

private:
    Game* m_game;
};

#endif // MEMENTO_H
