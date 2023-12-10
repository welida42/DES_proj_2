
#include <QPainter>
#include "speedometer.h"


Speedometer::Speedometer(QQuickItem *parent)
    :QQuickPaintedItem(parent),
    m_SpeedometerSize(400), // touch screen is 1280 x 400
    m_StartAngle(50),
    m_AlignAngle(260),
    m_LowestRange(0),
    m_HighestRange(200),
    m_Speed(0),
    m_Battery(100),
    m_ArcWidth(20),
    m_OuterColor(QColor(12,16,247)),
    m_InnerColor(QColor(51,88,255,80)),
    m_TextColor(QColor("silver")),
    m_BackgroundColor(Qt::transparent)
{

}

// drawing battery level by using rectangles: red < 20 < yellow < 40 < green < 100
void Speedometer::draw_battery_level(unsigned int level, QPainter *painter,  QRectF *rect, QPen *pen)

{
    painter->save();
    painter->setPen(Qt::NoPen);
    if (level <= 20)
    {
        painter->setBrush(QBrush(Qt::red));
        painter->drawRect(150, 320, 100, 10);
    }
    else if (20 < level and level <= 40)
    {
        painter->setBrush(QBrush(Qt::yellow));
        painter->drawRect(150, 320, 100, 10);
        painter->drawRect(150, 305, 100, 10);
    }
    else if (40 < level and level <= 60)
    {
        painter->setBrush(QBrush(Qt::green));
        painter->drawRect(150, 320, 100, 10);
        painter->drawRect(150, 305, 100, 10);
        painter->drawRect(150, 290, 100, 10);
    }
    else if (60 < level and level <= 80)
    {
        painter->setBrush(QBrush(Qt::green));
        painter->drawRect(150, 320, 100, 10);
        painter->drawRect(150, 305, 100, 10);
        painter->drawRect(150, 290, 100, 10);
        painter->drawRect(150, 275, 100, 10);
    }
    else if (80 < level and level <= 100)
    {
        painter->setBrush(QBrush(Qt::green));
        painter->drawRect(150, 320, 100, 10);
        painter->drawRect(150, 305, 100, 10);
        painter->drawRect(150, 290, 100, 10);
        painter->drawRect(150, 275, 100, 10);
        painter->drawRect(150, 260, 100, 10);
    }
    else
        return;

    // drawing value of battery level
    pen->setColor("silver");
    QFont font2("Halvetica",14,QFont::Bold);
    painter->setFont(font2);
    painter->setPen(*pen);
    painter->drawText(rect->adjusted(-100, 280, 100, 20), Qt::AlignCenter, QString::number(level,'i', 0) + "%");

    QFont font3("Halvetica",10,QFont::Bold);
    painter->setFont(font3);
    painter->setPen(*pen);
    painter->drawText(rect->adjusted(-100, 310, 100, 20), Qt::AlignCenter, "battery level");
    painter->restore();
}

void Speedometer::paint(QPainter *painter)
{

    QRectF rect = this->boundingRect(); // which returns an estimate of the area painted by the item
    painter->setRenderHint(QPainter::Antialiasing); // Indicates that the engine should antialias edges of primitives if possible.
    QPen pen = painter->pen(); // Returns the painter's current pen.
    pen.setCapStyle(Qt::FlatCap); // The cap style defines how the end points of lines are drawn using QPainter.

    double startAngle;
    double spanAngle;

    startAngle = m_StartAngle - 40;
    spanAngle = 0 - m_AlignAngle;

    //drawing the arch
    painter->save();
    pen.setWidth(m_ArcWidth);
    pen.setColor(m_InnerColor);
    painter->setPen(pen);
    painter->drawArc(rect.adjusted(m_ArcWidth, m_ArcWidth, -m_ArcWidth, -m_ArcWidth), startAngle * 16, spanAngle * 16);
    painter->restore();


    //drawing inner rectrangle
    painter->save();
    pen.setWidth(m_ArcWidth/2);
    pen.setColor(m_OuterColor);
    painter->setBrush(m_InnerColor);
    painter->setPen(pen);
    painter->drawRect(100, 100, 200, 100);
    painter->restore();
    //value of speed inside rect
    painter->save();
    QFont font("Halvetica",52,QFont::Bold);
    painter->setFont(font);
    pen.setColor(m_TextColor);
    painter->setPen(pen);
    painter->drawText(rect.adjusted(m_SpeedometerSize/40, m_SpeedometerSize/40, -m_SpeedometerSize/40, -m_SpeedometerSize/4), Qt::AlignCenter  ,QString::number((m_Speed),'i', 0));
    QFont font3("Halvetica",10,QFont::Bold);
    painter->setFont(font3);
    pen.setColor(m_TextColor);
    painter->setPen(pen);
    painter->drawText(rect.adjusted(m_SpeedometerSize/30 + 135, m_SpeedometerSize/30 + 60, -m_SpeedometerSize/30, -m_SpeedometerSize/4), Qt::AlignCenter, "cm/sec");

    painter->restore();

    //current active progress of speed
    painter->save();
    pen.setWidth(m_ArcWidth);
    pen.setColor(m_OuterColor);
    qreal valueToAngle = ((m_Speed - m_LowestRange)/(m_HighestRange - m_LowestRange)) * spanAngle;
    painter->setPen(pen);
    painter->drawArc(rect.adjusted(m_ArcWidth, m_ArcWidth, -m_ArcWidth, -m_ArcWidth), startAngle * 16, valueToAngle * 16);
    painter->restore();



    // Battery level bar chart
    painter->save();
//    int battery_level = 72;
    draw_battery_level(getBattery(), painter, &rect, &pen);
    painter->restore();
}


qreal Speedometer::getSpeedometerSize()
{
    return m_SpeedometerSize;
}

qreal Speedometer::getStartAngle()
{
    return m_StartAngle;
}


qreal Speedometer::getAlignAngle()
{
    return m_AlignAngle;
}


qreal Speedometer::getLowestRange()
{
    return m_LowestRange;
}

qreal Speedometer::getHighestRange()
{
    return m_HighestRange;
}

qreal Speedometer::getSpeed()
{
    return m_Speed;
}

unsigned int Speedometer::getBattery()
{
    return m_Battery;
}


int Speedometer::getArcWidth()
{
    return m_ArcWidth;
}

QColor Speedometer::getOuterColor()
{
    return m_OuterColor;
}

QColor Speedometer::getInnerColor()
{
    return m_InnerColor;
}

QColor Speedometer::getTextColor()
{
    return m_TextColor;
}

QColor Speedometer::getBackgroundColor()
{
    return m_BackgroundColor;
}



void Speedometer::setSpeedometerSize(qreal size)
{
    if(m_SpeedometerSize == size)
        return;
    m_SpeedometerSize = size;

    emit speedometerSizeChanged();
}

void Speedometer::setStartAngle(qreal startAngle)
{
    if(m_StartAngle == startAngle)
        return;

    m_StartAngle = startAngle;

    emit startAngleChanged();
}

void Speedometer::setAlignAngle(qreal angle)
{
    if(m_StartAngle == angle)
        return;

    m_StartAngle = angle;

    emit alignAngleChanged();
}

void Speedometer::setLowestRange(qreal lowestRange)
{
    if(m_LowestRange == lowestRange)
        return;

    m_LowestRange = lowestRange;

    emit lowestRangeChanged();
}

void Speedometer::setHighestRange(qreal highestRange)
{
    if(m_HighestRange == highestRange)
        return;

    m_HighestRange = highestRange;

    emit highestRangeChanged();
}

void Speedometer::setSpeed(qreal speed)
{
    if(m_Speed == speed)
        return;
    if(speed > getHighestRange())
        m_Speed = getHighestRange();
    else
        m_Speed = speed;

    update();
    emit speedChanged();
}

void Speedometer::setArcWidth(int arcWidth)
{
    if(m_ArcWidth == arcWidth)
        return;

    m_ArcWidth = arcWidth;

    emit arcWidthChanged();
}

void Speedometer::setOuterColor(QColor outerColor)
{
    if(m_OuterColor == outerColor)
        return;

    m_OuterColor = outerColor;

    emit outerColorChanged();
}

void Speedometer::setInnerColor(QColor innerColor)
{
    if(m_InnerColor == innerColor)
        return;

    m_InnerColor = innerColor;

    emit innerColorChanged();
}

void Speedometer::setTextColor(QColor textColor)
{
    if(m_TextColor == textColor)
        return;

    m_TextColor = textColor;

    emit textColorChanged();
}

void Speedometer::setBackgroundColor(QColor backgroundColor)
{
    if(m_BackgroundColor == backgroundColor)
        return;

    m_BackgroundColor = backgroundColor;

    emit backgroundColorChanged();
}

void Speedometer::setBattery(unsigned int battery)
{
    if(m_Battery == battery)
        return;

    m_Battery = battery;
    update();
    emit batteryChanged();
}
