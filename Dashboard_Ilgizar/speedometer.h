#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include <QObject>
#include <QQuickPaintedItem>


class Speedometer : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(qreal speedometerSize READ getSpeedometerSize WRITE setSpeedometerSize NOTIFY speedometerSizeChanged)
    Q_PROPERTY(qreal startAngle READ getStartAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(qreal alignAngle READ getAlignAngle WRITE setAlignAngle NOTIFY alignAngleChanged)
    Q_PROPERTY(qreal lowestRange READ getLowestRange WRITE setLowestRange NOTIFY lowestRangeChanged)
    Q_PROPERTY(qreal highestRange READ getHighestRange WRITE setHighestRange NOTIFY highestRangeChanged)
    Q_PROPERTY(qreal speed READ getSpeed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(unsigned int battery READ getBattery WRITE setBattery NOTIFY batteryChanged)
    Q_PROPERTY(int arcWidth READ getArcWidth WRITE setArcWidth NOTIFY arcWidthChanged)
    Q_PROPERTY(QColor outerColor READ getOuterColor WRITE setOuterColor NOTIFY outerColorChanged)
    Q_PROPERTY(QColor innerColor READ getInnerColor WRITE setInnerColor NOTIFY innerColorChanged)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

public:
    Speedometer(QQuickItem *parent = 0);
    virtual void paint(QPainter *painter);
    void draw_battery_level(unsigned int level, QPainter *painter,  QRectF *rect, QPen *pen);

    qreal           getSpeedometerSize();
    qreal           getStartAngle();
    qreal           getAlignAngle();
    qreal           getLowestRange();
    qreal           getHighestRange();
    qreal           getSpeed();
    unsigned int    getBattery();
    int             getArcWidth();
    QColor          getOuterColor();
    QColor          getInnerColor();
    QColor          getTextColor();
    QColor          getBackgroundColor();

    void            setSpeedometerSize(qreal size);
    void            setStartAngle(qreal startAngle);
    void            setAlignAngle(qreal angle);
    void            setLowestRange(qreal losbwestRange);
    void            setHighestRange(qreal highestRange);
    void            setSpeed(qreal speed);
    void            setBattery(unsigned int battery);
    void            setArcWidth(int arcWidth);
    void            setOuterColor(QColor outerColor);
    void            setInnerColor(QColor innerColor);
    void            setTextColor(QColor textColor);
    void            setBackgroundColor(QColor backgroundColor);

signals:
    void            speedometerSizeChanged();
    void            startAngleChanged();
    void            alignAngleChanged();
    void            lowestRangeChanged();
    void            highestRangeChanged();
    void            speedChanged();
    void            batteryChanged();
    void            arcWidthChanged();
    void            outerColorChanged();
    void            innerColorChanged();
    void            textColorChanged();
    void            backgroundColorChanged();

private:
    qreal           m_SpeedometerSize;
    qreal           m_StartAngle;
    qreal           m_AlignAngle;
    qreal           m_LowestRange;
    qreal           m_HighestRange;
    qreal           m_Speed;
    unsigned int    m_Battery;
    int             m_ArcWidth;
    QColor          m_OuterColor;
    QColor          m_InnerColor;
    QColor          m_TextColor;
    QColor          m_BackgroundColor;

};

#endif // SPEEDOMETER_H
