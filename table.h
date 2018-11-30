#pragma once

#include <QColor>
#include <QPainter>

#include "pocket.h"

class Ball;

class Table {
protected:
    int m_width;
    int m_height;
    QBrush m_brush;
    double m_friction;
public:
    virtual ~Table() {}

    //declaring copy constructor
    Table(Table &tbl)
    {
        m_width = tbl.m_width;
        m_height = tbl.m_height;
        m_brush = tbl.m_brush;
        m_friction = tbl.m_friction;
    }

    Table(int width, int height, QColor colour, double friction) :
        m_width(width), m_height(height),
        m_brush(colour), m_friction(friction) {}
    /**
     * @brief render - draw the table to screen using the specified painter
     * @param painter - painter to use
     */
    virtual void render(QPainter& painter, const QVector2D& offset) = 0;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    double getFriction() const { return m_friction; }

    virtual bool sinks(Ball*) { return false; }

    virtual Table *cloneTable() = 0;
};

class StageOneTable : public Table
{
public:
    StageOneTable(int width, int height, QColor colour, double friction) :
        Table(width, height, colour, friction) {}
    StageOneTable(StageOneTable &tbl) : Table(tbl) {}
    /**
     * @brief render - draw the stageonetable to screen using the specified painter
     * @param painter - painter to use
     */
    void render(QPainter &painter, const QVector2D& offset) override;

    virtual Table* cloneTable()
    {
        return new StageOneTable(*this);
    }

};

class StageTwoTable : public Table {
protected:
    std::vector<Pocket*> m_pockets;

public:
    StageTwoTable(int width, int height, QColor colour, double friction) :
        Table(width, height, colour, friction) {}

    StageTwoTable(StageTwoTable &tbl) : Table(tbl) {
        for(auto p : tbl.m_pockets){
            m_pockets.push_back(new Pocket(*p));
        }
    }

    ~StageTwoTable();

    virtual Table* cloneTable()
    {
        return new StageTwoTable(*this);
    }

    /**
     * @brief render - draw the stageonetable to screen using the specified painter
     * @param painter - painter to use
     */
    void render(QPainter &painter, const QVector2D& offset) override;

    // sinky winky ball
    virtual bool sinks(Ball* b) override;

    /* self explanatory */
    void addPocket(Pocket* p) { m_pockets.push_back(p); }
};
