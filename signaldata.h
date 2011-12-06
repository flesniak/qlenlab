#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include <qwt/qwt_series_data.h>

//class QRectF;
//class QVector;

class signaldata : public QwtSeriesData<QPointF>
{
public:
    signaldata();
    QPointF sample(size_t i) const;
    size_t size() const;
    QRectF boundingRect() const;

public slots:
    void append(const QPointF &pos);
    void clear();

private:
    QRectF cachedRect;
    QVector<QPointF> data;
};

#endif // SIGNALDATA_H
