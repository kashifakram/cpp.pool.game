#include "memento.h"


Memento::~Memento(){
    if(m_game)
        delete m_game;

}

void Memento::saveGameSate(Game *game)
{
    m_game = game;
}

Game* Memento::getGameState()
{
    Game* oldGameSate = m_game;
    m_game = nullptr;
    return oldGameSate;
}
