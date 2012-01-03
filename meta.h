#ifndef META_H
#define META_H

static const char version[] = "0.1alpha1";

enum channel { ch1a = 1, ch1b = 2, ch2a = 4, ch2b = 8, ch1 = ch1a|ch1b, ch2 = ch2a|ch2b, ch12ab = ch1|ch2 };

class signaldata;
struct dataset {
    signaldata* channel1;
    signaldata* channel2;
    signaldata* channel3;
    signaldata* channel4;
};

#endif // META_H
