#pragma once

#include <QPoint>
#include <cmath>
#include <QPainter>
#include <QVector2D>

enum BallTypes
{
    BALLTYPE_STAGEONEBALL = 0x01,
    BALLTYPE_COMPOSITEBALL = 0x02,
    BALLTYPE_STRIPPEDBALL = 0x04,
    BALLTYPE_CUEUEBALL = 0x08,
    BALLTYPE_BALLSMASH = 0x10,
    BALLTYPE_BALLSPARKLE = 0x20
};

class Ball {
protected:
    QBrush m_brush;
    QVector2D m_pos;
    QVector2D m_velocity;
    double m_mass;
    int m_radius;

    // if movement is slower than this, then we're considered at a stand-still
    static constexpr double MovementEpsilon = 1;
public:

    virtual ~Ball() {}

    //new clone ball method to be used for cloning game balls for state saving
    virtual Ball* cloneBall() = 0;

    Ball(QColor colour, QVector2D position,
         QVector2D velocity, double mass, int radius) :
        m_brush(colour), m_pos(position), m_velocity(velocity),
        m_mass(mass), m_radius(radius) {}
    Ball() {}
    Ball(const Ball& other) : m_brush(other.m_brush), m_pos(other.m_pos),
        m_mass(other.m_mass), m_velocity(other.m_velocity), m_radius(other.m_radius){}
    /**
     * @brief render - draw the ball to the screen
     * @param painter - QPainter that is owned by the dialog
     */
    virtual void render(QPainter &painter, const QVector2D& offset) = 0;
    /**
     * @brief translate - Move the ball's position by provided vector
     * @param vec - vector
     */
    virtual void translate(QVector2D vec) { m_pos += vec; }

    virtual QVector2D getVelocity() const { return m_velocity; }
    virtual void setVelocity(QVector2D v) { m_velocity = v; }
    /**
     * @brief changeVelocity - modify speed by a constant amount
     * @param delta - change in velocity (x,y)
     */
    virtual void changeVelocity(const QVector2D& delta) { m_velocity += delta; }
    /**
     * @brief multiplyVelocity - apply vector multiplicatively
     * @param vel - vector
     */
    virtual void multiplyVelocity(const QVector2D& vel) { m_velocity *= vel; }

    virtual double getMass() const { return m_mass; }
    virtual double getRadius() const { return m_radius; }
    virtual QVector2D getPosition() const { return m_pos; }
    virtual void setPosition(QVector2D p) { m_pos = p; }

    // whether the ball will break, and handle accordingly
    // for base ball, do nothing. insert into rhs if necessary
    virtual bool applyBreak(const QVector2D&, std::vector<Ball*>&) { return false; }

    //RETURNING BALL TYPE
    virtual int BallType() = 0;

    virtual bool isCueueBall(){return false;}

    bool isBlack(){return m_brush.color() == QColor("black");}
};

class StageOneBall : public Ball {
public:
    bool m_is_stripped = false;
    StageOneBall(QColor colour, QVector2D position,
                 QVector2D velocity, double mass, int radius) :
        Ball(colour, position, velocity, mass, radius) {}

    StageOneBall(StageOneBall &ball) : Ball(ball) {}

    /**
     * @brief render - draw the ball to the screen
     * @param painter - QPainter that is owned by the dialog
     */
    void render(QPainter &painter, const QVector2D& offset) override;

    virtual Ball *cloneBall() override;

    //returning stage one ball to be used in memento
    virtual int BallType() override;
};

class CompositeBall : public Ball {
protected:
    std::vector<Ball*> m_children;
    bool m_renderChildren = true;
    void recursiveRender(QPainter& painter, const QVector2D& offset);
    // default is unbreakable (i.e. inf str)
    double m_strength = std::numeric_limits<double>::max();

public:
    CompositeBall(QColor colour, QVector2D position,
                 QVector2D velocity, double mass, int radius, double strength) :
        Ball(colour, position, velocity, mass, radius), m_strength(strength) { }

    CompositeBall(CompositeBall &ball) : Ball(ball), m_renderChildren(ball.m_renderChildren), m_strength(ball.m_strength)
    {
        for(auto b : ball.m_children){
            m_children.push_back(b->cloneBall());
        }
    }

    /**
     * @brief render - draw the ball to the screen
     * @param painter - QPainter that is owned by the dialog
     */
    void render(QPainter &painter, const QVector2D& offset) override;

    /* add a child ball to this composite ball */
    void addChild(Ball* b) { m_children.push_back(b); }

    /**
     * @brief applyBreak - check and resolve breaking balls
     * @param deltaV - the change in velocity
     * @param parentlist - the list of balls that we'll need to add to if we break anything
     * @return whether the ball broke or not
     */
    virtual bool applyBreak(const QVector2D& deltaV, std::vector<Ball*>& parentlist) override;

    virtual Ball *cloneBall() override {return new CompositeBall(*this);}

    //returning stage one ball to be used in memento
    virtual int BallType() override
    {
        return BALLTYPE_COMPOSITEBALL;
    }
};



//class StageThreeBall : public Ball {
//public:
//    StageThreeBall(int number) {
//        is_striped = number > 8;
//        m_velocity = QVector2D(0, 0);
//        switch(number % 8){
//        case 0:
//            m_brush = QBrush(QColor("black"));
//            break;
//        case 1:
//            m_brush = QBrush(QColor("red"));
//            break;
//        case 2:
//            m_brush = QBrush(QColor("green"));
//            break;
//        case 3:
//            m_brush = QBrush(QColor("blue"));
//            break;
//        case 4:
//            m_brush = QBrush(QColor("yellow"));
//            break;
//        case 5:
//            m_brush = QBrush(QColor("orange"));
//            break;
//        case 6:
//            m_brush = QBrush(QColor("lightgreen"));
//            break;
//        case 7:
//            m_brush = QBrush(QColor("purple"));
//            break;
//        }

//        if(is_striped){
//            QPainterPath ballPath, maskPath;
//            ballPath.addEllipse((offset + m_pos).toPointF(), m_radius, m_radius);
//            maskPath.addRect(QRect((offset + m_pos).toPointF() - QVector2D(m_radius, m_radius/2),
//                                   (offset + m_pos).toPointF() + QVector2D(m_radius, m_radius/2)));
//            side = ballPath.subtracted(maskPath);
//        }
//    }

//    StageThreeBall(const StageThreeBall& other) : Ball(other){}

//    Ball* cloneStageThreeBall() override
//    {
//        return new StageThreeBall(*this);
//    }
//    /**
//     * @brief render - draw the ball to the screen
//     * @param painter - QPainter that is owned by the dialog
//     */
////    void render(QPainter &painter, const QVector2D& offset) override;
//protected:
//    bool is_striped = false;
//    QPainterPath side;
//};
