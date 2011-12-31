#ifndef META_H
#define META_H

static const char* version = "0.1alpha1";

enum channel { ch1a, ch1b, ch2a, ch2b };

class signaldata;
struct dataset {
    signaldata* channel1;
    signaldata* channel2;
    signaldata* channel3;
    signaldata* channel4;
};

#endif // META_H
