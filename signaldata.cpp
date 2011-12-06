#include "signaldata.h"

signaldata::signaldata()
{
    cachedRect = QRectF(0.0,0.0,0.0,0.0)
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

// We need absolute data here, means we have to rescale the char-values before calling signaldata::append!
void signaldata::append(const QPointF &pos)
{
    data.append(pos);
    if( !cachedRect.contains(pos) )
        cachedRect.
}

void signaldata::clear();
