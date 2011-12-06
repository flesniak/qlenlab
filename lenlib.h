//
//  LENlib.h
//  LENlib
//
//  Created by Lars Wolff on 03.12.11.
//

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
    char list[25][50];
};

#endif
