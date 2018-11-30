#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include <QTimer>
#include <iostream>
#include <QMouseEvent>
#include "utils.h"

#include <QVBoxLayout>
#include <QPushButton>

Dialog::Dialog(Game *game, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_game(game)
{
    ui->setupUi(this);

    // menu things
    QLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    m_configGameButton = new QPushButton("Config Game", this);
    m_singlePlayerButton = new QPushButton("Single Player Full Game", this);
    m_multiplayerButton = new QPushButton("Versus Full Game", this);
    connect(m_configGameButton, SIGNAL(clicked(bool)), this, SLOT(startGame()));
    connect(m_singlePlayerButton, SIGNAL(clicked(bool)), this, SLOT(singlePlayerFullGame()));
    connect(m_multiplayerButton, SIGNAL(clicked(bool)), this, SLOT(multiplayerFullGame()));
    layout->addWidget(m_configGameButton);
    layout->addWidget(m_singlePlayerButton);
    layout->addWidget(m_multiplayerButton);

    // for animating (i.e. movement, collision) every animFrameMS
    aTimer = new QTimer(this);
    connect(aTimer, SIGNAL(timeout()), this, SLOT(nextAnim()));

    // for drawing every drawFrameMS milliseconds
    dTimer = new QTimer(this);
    connect(dTimer, SIGNAL(timeout()), this, SLOT(tryRender()));

    // set the window size to be at least the table size
    this->resize(game->getMinimumWidth(), game->getMinimumHeight());
}

Dialog::~Dialog()
{
    delete aTimer;
    delete dTimer;
    delete m_game;
    delete ui;
}

void Dialog::tryRender() {
    this->update();
}

void Dialog::nextAnim() {
    m_game->animate(1.0/(double)animFrameMS);
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_R){}
}

void Dialog::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_R)
    {
        qDebug() << "reverse called";
        Game* prevgame = m_game->getOldGameState();
        if(prevgame != nullptr){
            delete m_game;
            m_game = prevgame;
            Mediator::instance().setGame(m_game);
        }
//        if(prevgame != nullptr)
//        {
//            Game* tmpGame = m_game;

//            int p1Score = prevgame->getPlayerScore(1);
//            int p2Score = prevgame->getPlayerScore(2);

//            if(prevgame->getPlayer1Turn())
//                m_game->setPlayer1Turn();
//            else
//                m_game->switchPlayersTurn();

//            m_game->setPlayerScore(1, p1Score);
//            m_game->setPlayerScore(2, p2Score);

//            delete tmpGame;
//        }

    }
}

void Dialog::paintEvent(QPaintEvent *)
{
    if(m_drawGame){
        QPainter painter(this);
        m_game->render(painter);
    }
}

void Dialog::mousePressEvent(QMouseEvent* event) {
    evalAllEventsOfTypeSpecified(MouseEventable::EVENTS::MouseClickFn, event);
}

void Dialog::mouseReleaseEvent(QMouseEvent* event) {
    evalAllEventsOfTypeSpecified(MouseEventable::EVENTS::MouseRelFn, event);
}
void Dialog::mouseMoveEvent(QMouseEvent* event) {
    evalAllEventsOfTypeSpecified(MouseEventable::EVENTS::MouseMoveFn, event);
}

void Dialog::singlePlayerFullGame(){
    delete m_game;
    m_game = Game::newStageThreeGame(false);
    startGame();
}

void Dialog::multiplayerFullGame(){
    delete m_game;
    m_game = Game::newStageThreeGame(true);
    m_game->set_show_label(true);
    startGame();
}

void Dialog::startGame(){
    delete m_configGameButton;
    delete m_singlePlayerButton;
    delete m_multiplayerButton;

    m_drawGame = true;
    aTimer->start(animFrameMS);
    dTimer->start(drawFrameMS);
    Mediator::instance().setGame(m_game);
    m_game->saveCurrentGameState();
}

void Dialog::evalAllEventsOfTypeSpecified(MouseEventable::EVENTS t, QMouseEvent *event) {
    // handle all the clicky events, and remove them if they've xPIRED
    MouseEventable::EventQueue& Qu = m_game->getEventFns();
    for (ssize_t i = Qu.size()-1; i >= 0; i--) {
        if (auto spt = (Qu.at(i)).lock()) {
            if (spt->second == t) {
                spt->first(event);
            }
        } else {
            // remove this element from our vector
            Qu.erase(Qu.begin() + i);
        }
    }
}
