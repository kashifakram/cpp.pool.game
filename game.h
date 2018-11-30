#pragma once
#include <QJsonObject>
#include <functional>
#include <QMouseEvent>
#include <QtDebug>

#include "abstractstagefactory.h"
#include "mediator.h"
#include "table.h"
#include "ball.h"
#include "balldecorator.h"
#include "utils.h"
#include "memento.h"

class Game {
    std::vector<Ball*>* m_balls;
    Table* m_table;

    // screenshake stuff
    QVector2D m_screenshake;
    double m_shakeRadius = 0.0;
    double m_shakeAngle = 0;
    static constexpr double SCREENSHAKEDIST = 10.0;

    /* increase the amount of screen shake */
    void incrementShake(double amount=SCREENSHAKEDIST) { m_shakeRadius += amount; }

    //player 1 and 2 scores getters and setters funcitons



private:
    // store the functions that get scanned through whenever a mouse event happens
    MouseEventable::EventQueue m_mouseEventFunctions;

    //players turns variables
    bool m_is_player1_turn = true;

    /**
     * @brief updateShake - update the screenshake radius (make it smaller)
     * @param dt - the timestep change
     */
    void updateShake(double dt);
    bool m_show_labels = false;
    bool m_is_two_players_mode = false;

    //this variable is to identify striped balls assigned to which players -1 not assigned yet, 0 assigned to p1 and 1 assigned to p2
    int m_striped_assigned_player1 = -1;

    int m_player1_score = 0;
    int m_player2_score = 0;

    //declaring memento member to get or save game state
    Memento* m_gameState = nullptr;
public:



    ~Game();
    Game(std::vector<Ball*>* balls, Table* table) :
        m_balls(balls), m_table(table) {}

    //default empty constructor to use in saving and returning game methods
    Game(){}

    static Game* newStageThreeGame(bool isMP);


    void setPlayerScore(int player, int score);
    int getPlayerScore(int player);
    void updateScore();
    void assignStripedBalls(bool isPlayer1);
    int getStripedBallsAssignment();
    void setPlayer1Turn();
    bool getPlayer1Turn();

    void switchPlayersTurn();
    void onCueBallHit(){
        qDebug() << "[before]Player 1 turn" << m_is_player1_turn;
        saveCurrentGameState();
        switchPlayersTurn();
        qDebug() << "[after]Player 1 turn" << m_is_player1_turn;
    }

    void set_show_label(bool status);
    bool show_label();

    //method will be called on r key press to save game state and return to previous game state
    void saveCurrentGameState();

    Game* getOldGameState();

    Game* makeClone();

    void setTwoPlayer();

    /**
     * @brief Draws all owned objects to the screen (balls and table)
     * @param painter - qtpainter to blit to screen with
     */
    void render(QPainter& painter) const;

    /**
     * @brief Updates the positions of all objects within, based on how much time has changed
     * @param dt - time elapsed since last frame in seconds
     */
    void animate(double dt);

    /* how large the window's width should at least be */
    int getMinimumWidth() const { return m_table->getWidth(); }

    /* how large the window's height should at least be */
    int getMinimumHeight() const { return m_table->getHeight(); }

    /**
     * @brief resolveCollision - modify the ball's velocity if it is colliding with the table
     * @param table - the table to be bounds checked
     * @param ball - the ball to move
     * @return velocity - the change of velocity that a ball underwent
     */
    QVector2D resolveCollision(const Table* table, Ball* ball);

    /**
     * @brief resolveCollision - resolve both ball's velocity whether these balls collide
     * @param ballA - first ball
     * @param ballB - second ball
     * @param pair<deltaVelocityA, deltaVelocityB> - the change of velocities for each ball
     */
    std::pair<QVector2D, QVector2D> resolveCollision(Ball* ballA, Ball* ballB);

    /**
     * @brief isColliding - returns whether two balls are touching each other
     * @param ballA
     * @param ballB
     * @return whether the two balls are touching each other
     */
    bool isColliding(const Ball* ballA, const Ball* ballB) {
        QVector2D collisionVector = ballB->getPosition() - ballA->getPosition();
        return !(collisionVector.length() > ballA->getRadius() + ballB->getRadius());
    }

    /**
     * @brief addMouseFunctions - append all of the specified functions to be
     *  our eventqueue - these will be cycled through onclick, etc
     * @param fns
     */
    void addMouseFunctions(MouseEventable::EventQueue fns) {
        std::copy(fns.begin(), fns.end(), std::back_inserter(m_mouseEventFunctions));
    }

    /**
     * @brief getEventFns - get all of our event functions (mouseclicksfns, etc)
     * @return event queue of event functions
     */
    MouseEventable::EventQueue& getEventFns() { return m_mouseEventFunctions; }
};
