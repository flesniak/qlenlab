/************************************************************************
 * Copyright (C) 2011 Fabian Lesniak <fabian.lesniak@student.kit.edu>   *
 *                                                                      *
 * This file is part of the QLenLab project.                            *
 *                                                                      *
 * QLenLab is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * QLenLab is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with QLenLab. If not, see <http://www.gnu.org/licenses/>.      *
 ***********************************************************************/

#ifndef META_H
#define META_H

namespace meta {
static const char version[] = "0.1";

enum channel { ch1a = 1, ch1b = 2, ch2a = 4, ch2b = 8, ch1 = ch1a|ch1b, ch2 = ch2a|ch2b, ch12ab = ch1|ch2 };

enum colorindex { background = 4, grid = 5, channel1 = 0, channel2 = 1, channel3 = 2, channel4 = 3 };

enum triggermode { deactivated = 0, both = 1, rising = 2, falling = 3 };
}

class signaldata;
struct dataset {
    signaldata* channel1;
    signaldata* channel2;
    signaldata* channel3;
    signaldata* channel4;
};

#endif // META_H
