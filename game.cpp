#include "game.h"
#include "utils.h"

#include <QJsonArray>
#include <stdexcept>
#include <cmath>
#include <exception>
#include <iostream>
#include <QDebug>
#include <QMessageBox>

Game::~Game() {
    // cleanup ya boi
    for (auto b : *m_balls) delete b;
    delete m_balls;
    delete m_table;
}


//Memento Methods
Game *Game::newStageThreeGame(bool isMP){
    if (isMP){


    }
    //build balls
    std::vector<Ball*> *balls = new std::vector<Ball*>();
    StageTwoTable* table = new StageTwoTable(900, 500, "darkgreen", 0.5);

    balls->push_back(new StageOneBall("red", QVector2D(200, 200), QVector2D(0, 0), 1, 10));
    balls->push_back(new StageOneBall("green", QVector2D(250, 200), QVector2D(0, 0), 1, 10));
    balls->push_back(new StageOneBall("blue", QVector2D(200, 250), QVector2D(0, 0), 1, 10));
    balls->push_back(new StageOneBall("yellow", QVector2D(225, 275), QVector2D(0, 0), 1, 10));
    balls->push_back(new StageOneBall("orange", QVector2D(275, 225), QVector2D(0, 0), 1, 10));
    balls->push_back(new StageOneBall("lightgreen", QVector2D(200, 225), QVector2D(0, 0), 1, 10));
    balls->push_back(new StageOneBall("purple", QVector2D(250, 225), QVector2D(0, 0), 1, 10));

    balls->push_back(new StrippedBallDecorator(new StageOneBall("red", QVector2D(300, 300), QVector2D(0, 0), 1, 10)));
    balls->push_back(new StrippedBallDecorator(new StageOneBall("green", QVector2D(350, 300), QVector2D(0, 0), 1, 10)));
    balls->push_back(new StrippedBallDecorator(new StageOneBall("blue", QVector2D(300, 350), QVector2D(0, 0), 1, 10)));
    balls->push_back(new StrippedBallDecorator(new StageOneBall("yellow", QVector2D(325, 375), QVector2D(0, 0), 1, 10)));
    balls->push_back(new StrippedBallDecorator(new StageOneBall("orange", QVector2D(375, 325), QVector2D(0, 0), 1, 10)));
    balls->push_back(new StrippedBallDecorator(new StageOneBall("lightgreen", QVector2D(300, 325), QVector2D(0, 0), 1, 10)));
    balls->push_back(new StrippedBallDecorator(new StageOneBall("purple", QVector2D(350, 325), QVector2D(0, 0), 1, 10)));

    balls->push_back(new StageOneBall("black", QVector2D(400, 450), QVector2D(0, 0), 1, 10));
    balls->push_back(new CueBall(new StageOneBall("white", QVector2D(table->getWidth()-10, table->getHeight()/2), QVector2D(0, 0), 1, 15)));

    CueBall *cb = dynamic_cast<CueBall *>(balls->back());

    // build table
    table->addPocket(new Pocket(30, QVector2D(0, 0)));
    table->addPocket(new Pocket(30, QVector2D(table->getWidth(), 0)));
    table->addPocket(new Pocket(30, QVector2D(0, table->getHeight())));
    table->addPocket(new Pocket(30, QVector2D(table->getWidth(), table->getHeight())));
    table->addPocket(new Pocket(30, QVector2D(table->getWidth()/2, 0)));
    table->addPocket(new Pocket(30, QVector2D(table->getWidth()/2, table->getHeight())));

    Game* game = new Game(balls, table);
    game->addMouseFunctions(cb->getEvents());
    return game;
}


void Game::saveCurrentGameState()
{
//    clear old game state and to save new state
    qDebug() << "saving game";
    delete m_gameState;

//    declaring memento to save new state
    m_gameState = new Memento();
    m_gameState->saveGameSate(this->makeClone());

}

Game* Game::getOldGameState()
{
    qDebug() << "getting old game";
    if(m_gameState)
        return m_gameState->getGameState();
}

Game *Game::makeClone()
{
    //declaring new game object
    qDebug() << "cloning game";
    Game* new_game = new Game();
    new_game->m_balls = new std::vector<Ball*>();
    for (Ball *b:*(this->m_balls)){
        new_game->m_balls->push_back(b->cloneBall());
        Ball* newBall = new_game->m_balls->back();
        if(newBall->isCueueBall())
        {
            new_game->addMouseFunctions(dynamic_cast<CueBall *>(newBall)->getEvents());
        }
    }
    new_game->m_table = this->m_table->cloneTable();
    new_game->setPlayerScore(1, this->getPlayerScore(1));
    new_game->setPlayerScore(2, this->getPlayerScore(2));
    if(getPlayer1Turn())
        new_game->setPlayer1Turn();
    else
        new_game->switchPlayersTurn();


    return new_game;
}

void Game::setTwoPlayer(){m_is_two_players_mode = true;}

void Game::render(QPainter &painter) const {
    // table is rendered first, as its the lowest
    m_table->render(painter, m_screenshake);
    // then render all the balls
    for (Ball* b : *m_balls) b->render(painter, m_screenshake);

    if (m_show_labels)
    {
        QPen oldPen = painter.pen();
        QBrush oldBrush = painter.brush();
        QFont oldFont = painter.font();
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        if(m_is_player1_turn){
            painter.drawText(m_table->getWidth() + 15, 50, "Player 1");
            painter.drawText(m_table->getWidth() + 15, 70, "Turn");
            painter.setBrush(QBrush(Qt::GlobalColor::red));
            painter.drawEllipse(QPoint(m_table->getWidth() + 40, 80), 10, 10);
        }else{
            painter.drawText(m_table->getWidth() + 15, 50, "Player 2");
            painter.drawText(m_table->getWidth() + 15, 70, "Turn");
            painter.setBrush(QBrush(Qt::GlobalColor::red));
            painter.drawEllipse(QPoint(m_table->getWidth() + 40, 80), 10, 10);
            QPainterPath ballPath, maskPath;
            ballPath.addEllipse(QPoint(m_table->getWidth() + 40, 80), 10, 10);
            maskPath.addRect(QRect(QPoint(m_table->getWidth() + 40, 80) - QPoint(10, 5),
                                   QPoint(m_table->getWidth() + 40, 80) + QPoint(10, 5)));
            painter.setPen(Qt::NoPen);
            painter.setBrush(Qt::white);
            painter.drawPath(ballPath.subtracted(maskPath));
        }
        painter.setPen(oldPen);
        painter.setBrush(oldBrush);
        painter.setFont(oldFont);
    }

}

void Game::animate(double dt) {
    // keep track of the removed balls (they're set to nullptr during loop)
    // clean up afterwards
    std::vector<Ball*> toBeRemoved;
    // add these balls to the list after we finish
    std::vector<Ball*> toBeAdded;

    // (test) collide the ball with each other ball exactly once
    // to achieve this, balls only check collisions with balls "after them"
    for (auto it = m_balls->begin(); it != m_balls->end(); ++it) {
        Ball* ballA = *it;
        if (ballA == nullptr) continue;

        // correct ball velocity if colliding with table
        QVector2D tableBallDeltaV = resolveCollision(m_table, ballA);
        // test and resolve breakages with balls bouncing off table
        if (ballA->applyBreak(tableBallDeltaV, toBeAdded)) {
            // mark this ball to be deleted
            toBeRemoved.push_back(ballA);
            incrementShake();
            // nullify this ball
            *it = nullptr;

            continue;
        }

        // check whether ball should be swallowed
        if (m_table->sinks(ballA)) {

            //two players game rule if any player puts cueue ball or black ball in pocket before clearing other balls opponent player will by default and game ends

            if(m_show_labels){
                if(ballA->isBlack()){
                    bool solidBallCleared = true;
                    bool strippedBallCleared = true;
                    for(Ball *b : *m_balls){
                        if(b == nullptr || b == ballA) continue;
                        if(b->BallType() & BALLTYPE_STRIPPEDBALL){
                            strippedBallCleared = false;
                        }else if(!(b->BallType() & BALLTYPE_CUEUEBALL)){
                            solidBallCleared = false;
                        }
                    }
                    qDebug() << "Solid clear" << solidBallCleared;
                    qDebug() << "Stripped clear" << strippedBallCleared;
                    if((!m_is_player1_turn && solidBallCleared) ||
                            (m_is_player1_turn && !strippedBallCleared)){
                        // player 1 win
                        QMessageBox::information(nullptr, "Congratulations", "Player 1 won!", QMessageBox::Ok);
                        exit(0);
                    }else{
                        QMessageBox::information(nullptr, "Congratulations", "Player 2 won!", QMessageBox::Ok);
                        exit(0);
                    }
                }
            }


            //single player game rule if player puts cueue ball or black ball in pocket before clearing other balls he lost, otherwise if he puts all balls in pocket and black at end he ll win

            if(!m_is_two_players_mode){
                if(ballA->isBlack()){
                    bool solidBallCleared = true;
                    bool strippedBallCleared = true;
                    for(Ball *b : *m_balls){
                        if(b == nullptr || b == ballA) continue;
                        if(b->BallType() & BALLTYPE_STRIPPEDBALL){
                            strippedBallCleared = false;
                        }else if(!(b->BallType() & BALLTYPE_CUEUEBALL)){
                            solidBallCleared = false;
                        }
                    }
                    qDebug() << "Solid clear" << solidBallCleared;
                    qDebug() << "Stripped clear" << strippedBallCleared;
                    if((!m_is_player1_turn && solidBallCleared) ||
                            (m_is_player1_turn && !strippedBallCleared)){
                        // player 1 win
                        QMessageBox::information(nullptr, "Congratulations", "You won!", QMessageBox::Ok);

                        exit(0);
                    }else{
                        QMessageBox::information(nullptr, "Alas!", "You lost!", QMessageBox::Ok);
                        exit(0);
                    }
                }
            }

            //scores && assign player ball type
            // deal with black 8 sink
//            QMessageBox::information("Game Over", "Player x won" );
//            QApplication::quit();
            // defer swallowing until later (messes iterators otherwise)
            toBeRemoved.push_back(ballA);
            // nullify this ball
            *it = nullptr;
            continue;
        }

        // check collision with all later balls
        for (auto nestedIt = it + 1; nestedIt != m_balls->end(); ++nestedIt) {
            Ball* ballB = *nestedIt;
            if (ballB == nullptr) continue;
            if (isColliding(ballA, ballB)) {
                // retrieve the changes in velocities for each ball and resolve collision
                QVector2D ballADeltaV,ballBDeltaV;
                std::tie(ballADeltaV, ballBDeltaV) = resolveCollision(ballA, ballB);

                // add screenshake, remove ball, and add children to table vector if breaking
                if (ballA->applyBreak(ballADeltaV, toBeAdded)) {
                    toBeRemoved.push_back(ballA);
                    incrementShake();
                    // nullify this ball
                    *it = nullptr;
                    break;
                }
                // add screenshake, remove ball, and add children to table vector if breaking
                if (ballB->applyBreak(ballBDeltaV, toBeAdded)) {
                    toBeRemoved.push_back(ballB);
                    incrementShake();
                    // nullify this ball
                    *nestedIt = nullptr;
                    continue;
                }
            }
        }
        // we marked this ball as deleted, so skip
        if (*it == nullptr) continue;

        // move ball due to speed
        ballA->translate(ballA->getVelocity() * dt);
        // apply friction
        ballA->changeVelocity(-ballA->getVelocity() * m_table->getFriction() * dt);

    }

    // clean up them trash-balls
    for (Ball* b : toBeRemoved) {
        delete b;
        // delete all balls marked with nullptr
        m_balls->erase(std::find(m_balls->begin(), m_balls->end(), nullptr));
    }
    for (Ball* b: toBeAdded) m_balls->push_back(b);

    updateShake(dt);
}

void Game::setPlayerScore(int player, int score)
{
    switch(player){
    case 1:
        m_player1_score = score;
        break;
    case 2:
        m_player2_score = score;
        break;
    }

}

int Game::getPlayerScore(int player)
{
    switch(player){
    case 1:
        return m_player1_score;

    case 2:
        return m_player2_score;
    }
}

void Game::assignStripedBalls(bool isPlayer1)
{
    if(isPlayer1)
    {
        m_striped_assigned_player1 = 0;
    } else if(!isPlayer1) {
        m_striped_assigned_player1 = 1;
    } else {
        m_striped_assigned_player1 = -1;
    }
}

int Game::getStripedBallsAssignment()
{
    return m_striped_assigned_player1;
}

void Game::setPlayer1Turn()
{
    m_is_player1_turn = true;
}

bool Game::getPlayer1Turn()
{
    return m_is_player1_turn;
}

void Game::switchPlayersTurn()
{
    m_is_player1_turn = !m_is_player1_turn;
}

void Game::set_show_label(bool status){m_show_labels = status;}

bool Game::show_label() {return m_show_labels;}

void Game::updateShake(double dt) {
    // <3 code lovingly taken from here: <3
    // https://gamedev.stackexchange.com/a/47565
    // slightly modified..

    // update the screen shake per time step
    m_shakeRadius *= (1-dt)*0.9;
    m_shakeAngle += (150 + rand()%60);
    m_screenshake = QVector2D(sin(m_shakeAngle)*m_shakeRadius, cos(m_shakeAngle)*m_shakeRadius);
}

QVector2D Game::resolveCollision(const Table* table, Ball* ball) {
    QVector2D bPos = ball->getPosition();

    QVector2D startingVel = ball->getVelocity();

    // resulting multiplicity of direction. If a component is set to -1, it
    // will flip the velocity's corresponding component
    QVector2D vChange(1,1);

    // ball is beyond left side of table's bounds
    if (bPos.x() - ball->getRadius() <= 0) {
        // flip velocity if wrong dir
        if (ball->getVelocity().x() <= 0) vChange.setX(-1);
    // ball is beyond right side of table's bounds
    } else if (bPos.x() + ball->getRadius() >= 0 + table->getWidth()) {
        // flip velocity if wrong dir
        if (ball->getVelocity().x() >= 0) vChange.setX(-1);
    }
    // ball is above top of the table's bounds
    if (bPos.y() - ball->getRadius() <= 0) {
        // flip iff we're travelling in the wrong dir
        if (ball->getVelocity().y() <= 0) vChange.setY(-1);
    // ball is beyond bottom of table's bounds
    } else if (bPos.y() + ball->getRadius() >= 0 + table->getHeight()) {
        // if we're moving down (we want to let the ball bounce up if its heading back)
        if (ball->getVelocity().y() >= 0) vChange.setY(-1);
    }

    ball->multiplyVelocity(vChange);

    // return the change in velocity
    return ball->getVelocity() - startingVel;
}

std::pair<QVector2D, QVector2D> Game::resolveCollision(Ball* ballA, Ball* ballB) {
    // SOURCE : ASSIGNMENT SPEC

    // if not colliding (distance is larger than radii)
    QVector2D collisionVector = ballB->getPosition() - ballA->getPosition();
    if (collisionVector.length() > ballA->getRadius() + ballB->getRadius()) {
       throw std::logic_error("attempting to resolve collision of balls that do not touch");
    }
    collisionVector.normalize();

    QVector2D ballAStartingVelocity = ballA->getVelocity();
    QVector2D ballBStartingVelocity = ballB->getVelocity();

    float mr = ballB->getMass() / ballA->getMass();
    double pa = QVector2D::dotProduct(collisionVector, ballA->getVelocity());
    double pb = QVector2D::dotProduct(collisionVector, ballB->getVelocity());

    if (pa <= 0 && pb >= 0) return std::make_pair(QVector2D(0,0), QVector2D(0,0));

    double a = -(mr + 1);
    double b = 2*(mr * pb + pa);
    double c = -((mr - 1)*pb*pb + 2*pa*pb);
    double disc = sqrt(b*b - 4*a*c);
    double root = (-b + disc)/(2*a);
    if (root - pb < 0.01) {
        root = (-b - disc)/(2*a);
    }

    ballA->changeVelocity(mr * (pb - root) * collisionVector);
    ballB->changeVelocity((root-pb) * collisionVector);

    // return the change in velocities for the two balls
    return std::make_pair(ballA->getVelocity() - ballAStartingVelocity, ballB->getVelocity() - ballBStartingVelocity);
}


//game rule methods

void Game::updateScore()
{

//    if(get)


}

