/************************************************************************
 * Copyright (C) 2011 Lars Wolff <lars.wolff@student.kit.edu>           *
 *                                                                      *
 *   LENlib.h     07.12.2011                                            *
 *                                                                      *
 * This file is part of the LENlib project.                             *
 *                                                                      *
 * LENlib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * LENlib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with LENlib. If not, see <http://www.gnu.org/licenses/>.       *
 ***********************************************************************/

#ifndef LENlib_LENlib_h
#define LENlib_LENlib_h

#define CH1     3
#define CH2     12
#define CH1A    1
#define CH1B    2
#define CH2A    4
#define CH2B    8

#define VDIV_05  0
#define VDIV_1   1
#define VDIV_2   2
#define VDIV_10  3

class lenboard {
public:
    lenboard();
    ~lenboard();

    int     openport(char* port);
    int     closeport();

    char*   getid();

    int     setsinusfrequency(unsigned int frequency);
    unsigned int getsinusfrequency();
    
    int     setsquarefrequency(unsigned int frequency);
    int     setsquareratio(int ratio);
    unsigned int getsquarefrequency();
    int     getsquareratio();

    int     setactivechannels(int channels);
    int     activatechannel(int channel, bool active);
    int     setvoltagedivision(int channel, int voltagedevision);
    int     setoffsetchannel(int channel, bool active);
    int     setoffset(int channels);
    int     getvoltagedevision(int channel);
    int     getoffset(int channel);
    int     getchannelactive(int channel);

    int     setsamplerate(long unsigned int samplerate);
    long unsigned int getsamplerate();

    int     measure();
    void    stopmeasure();
    int     getvalue(int number, int channel);
    int     getvaluecount();
    unsigned char* getrawmeasurement();

    void    freeme(bool checkrange);

private:
    int     portsend(char* data, int length);
    unsigned char flagstonum(bool* flagarray);
    void    numtoflags(bool* flagarray, unsigned char number);

    int     hserial;
    bool    checkrange;
    char    idstring[50];
    unsigned int sinusfrequency;
    unsigned int squarefrequency;
    int     squareratio;
    long unsigned int samplerate;
    bool    activechannels[4];
    int     vdivision[2];
    bool    offsetchannels[4];

    unsigned char measurement[16500];
    unsigned char* measuredvalues;
    unsigned int measurementlenght;
};

class portlist{
public:
    portlist();
    int     refresh();
    int     count();
    char*   portbynumber(int number);
    bool    portwalk(char* string);

private:
    int listcount;
    int listpos;
    char list[30][50];
};

#endif