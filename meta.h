/***************************************************************************
 * Copyright (C) 2011-2012 Fabian Lesniak <fabian.lesniak@student.kit.edu> *
 *                                                                         *
 * This file is part of the QLenLab project.                               *
 *                                                                         *
 * QLenLab is free software: you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * QLenLab is distributed in the hope that it will be useful, but WITHOUT  *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 * for more details.                                                       *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with QLenLab. If not, see <http://www.gnu.org/licenses/>.               *
 **************************************************************************/

#ifndef META_H
#define META_H

namespace meta {
static const char version[] = "0.4";

enum channel { ch1a = 1, ch1b = 2, ch2a = 4, ch2b = 8, ch1 = ch1a|ch1b, ch2 = ch2a|ch2b, ch12ab = ch1|ch2 };

enum colorindex { background = 4, grid = 5, channel1 = 0, channel2 = 1, channel3 = 2, channel4 = 3 };

enum triggermode { deactivated = 0, both = 1, rising = 2, falling = 3 };

enum runmode { none, measure, bode, connect };

enum connectstate { disconnected = 0, connectfail = 1, connecting = 2, connected = 3 };

enum plotmode { scope, fft };
}

class signaldata;
class QTime;
struct dataset {
    QTime* timestamp;
    signaldata* channel[4];
    unsigned char flags; //tracks how many threads are using this dataset, has to be 0 to be deleted by storage
};

struct bodestate {
    unsigned int progress;
    unsigned int frequency;
    double inputAmplitude;
    double outputAmplitude;
    double amplification;
    unsigned int samplerate;
};

inline unsigned int chan2num(meta::channel chan)
{
    unsigned int num = 0, ichan = (unsigned int)chan;
    while( !(ichan & 1) ) {
        num++;
        ichan >>= 1;
    }
    return num;
}

#endif // META_H
