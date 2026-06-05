#ifndef BAR_H
#define BAR_H

#include <QObject>
#include <QColor>


class Bar : public QObject
{
    Q_OBJECT
        Q_PROPERTY(int x READ x WRITE setX NOTIFY changed)

public:
    Bar(int value, int x, int baseY, QObject* parent = nullptr)
        : QObject(parent), value(value), m_x(x), m_baseY(baseY) 
    {
        color = QColor(100, 150, 255);  
    }

    int x() const { return m_x; }

    void setX(int x)
    {
        if (m_x == x) return;
        m_x = x; 
        emit changed();
    }

    int value;
    QColor color;

signals:
    void changed();

private:
    int m_x;
    int m_baseY;
};

#endif
