#include "signaldata.h"

signaldata::signaldata() : cachedRect(0.0,0.0,0.0,0.0)
{
}

QPointF signaldata::sample(size_t i) const
{
    return data.value(i);
}

size_t signaldata::size() const
{
    return data.size();
}

QRectF signaldata::boundingRect() const
{
    return cachedRect;
}

// We need absolute data here, means we have to rescale the char-values before calling signaldata::append! (done in lenlib)
void signaldata::append(const QPointF &pos)
{
    data.append(pos);
    if( !cachedRect.contains(pos) ) {
        if( pos.y() > 0 )
            cachedRect.setTop(pos.y());
        else
            cachedRect.setBottom(pos.y());
    }
}

void signaldata::clear()
{
    data.clear();
    cachedRect = QRectF(0.0,0.0,0.0,0.0);
}
