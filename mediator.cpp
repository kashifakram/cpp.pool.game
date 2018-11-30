#include "mediator.h"

#include "game.h"

Mediator &Mediator::instance(){
    static Mediator mediator;
    return mediator;
}

void Mediator::setGame(Game *game){
    m_game = game;
}

void Mediator::cueBallHit(){
    if(m_game != nullptr) m_game->onCueBallHit();
}
