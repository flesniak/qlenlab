/************************************************************************
 * Copyright (C) 2011 Lars Wolff <lars.wolff@student.kit.edu>           *
 * Copyright (C) 2012 Fabian Lesniak <fabian.lesniak@student.kit.edu>   *
 *                                                                      *
 *   LENlib.cpp     07.12.2011                                          *
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

#include "LENlib.h"
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>

#if defined(__APPLE__)
#define OS_MAC
#elif defined(linux) || defined(__GNUC__)
#define OS_LINUX
#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#endif

lenboard::lenboard(){
    checkrange = true;
    hserial = -1;
    sinusfrequency = 1000;
    squarefrequency = 1000;
    squareratio = 50;
    samplerate = 100000;
    activechannels[0] = true;
    activechannels[1] = false;
    activechannels[2] = false;
    activechannels[3] = false;
    vdivision[0] = 1;
    vdivision[1] = 1;
    offsetchannels[0] = true;
    offsetchannels[1] = true;
    offsetchannels[2] = true;
    offsetchannels[3] = true;
    measuredvalues = NULL;
    measurementlength = 0;
    return;
}

lenboard::~lenboard(){
    closeport();
}

int lenboard::portsend(char* data, int length){
    long int result = write(hserial, data, length);
	if(result != length)
		return -1;

	return 0;
}

unsigned char lenboard::flagstonum(bool* flagarray) const {
    unsigned char number = 0;
    if(flagarray[0])
        number += CH1A;
    if(flagarray[1])
        number += CH1B;
    if(flagarray[2])
        number += CH2A;
    if(flagarray[3])
        number += CH2B;

    return number;
}

void lenboard::numtoflags(bool* flagarray, unsigned char number) const {
    if(number >= 8){
        number -= 8;
        flagarray[3] = true;
    }else
        flagarray[3] = false;

    if(number >= 4){
        number -= 4;
        flagarray[2] = true;
    }else
        flagarray[2] = false;

    if(number >= 2){
        number -= 2;
        flagarray[1] = true;
    }else
        flagarray[1] = false;

    if(number >= 1){
        number -= 1;
        flagarray[0] = true;
    }else
        flagarray[0] = false;

    return;
}


void lenboard::freeme(bool check){
    checkrange = !check;
    return;
}


int lenboard::openport(char* portname){

    char path[60] = "/dev/";
    strncat(path, portname, 50);

    hserial = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
    if(hserial < 0)
        return -1;

    if(fcntl(hserial, F_SETFL, 0) < 0){
        closeport();
        return -1;
    }

    if(fcntl(hserial, F_SETFL, FNDELAY) < 0){
        closeport();
        return -1;
    }

    struct termios options;
    tcgetattr(hserial, &options);   //Get the current options for the port
    cfsetispeed(&options, B115200);     //Set the baud rates to 115200
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD);    //Enable the receiver and set local mode
    if(tcsetattr(hserial, TCSANOW, &options) < 0){   //Set the new options for the port
        closeport();
        return -1;
    }

    char query[] = "id\x0D\x0A";
    portsend(query, 4);

    usleep(50000);  //controller has 50 msec to respond

    char buff[50];
    long int nbytes = read(hserial, &buff, 50);
    buff[nbytes] = '\0';

    if(strcmp(buff, "LEN-Hardwaremodul v3.1\x0D\x0A") != 0){
        closeport();
        return -2;
    }

    strcpy(idstring, buff);

    return 0;
}

int lenboard::closeport(){
    if(hserial >= 0)
        close(hserial);
    hserial = -1;
    return 0;
}


const char* lenboard::getid() const {
    if(hserial < 0)
        return NULL;
    return idstring;
}


int lenboard::setsinusfrequency(unsigned int freq){
    if(hserial < 0)
        return -5;
    if(freq > 10000 && checkrange)
        return -1;

    sinusfrequency = freq;

    char query[50];
    int length = sprintf(query, "sig_sin %d\x0D\x0A", sinusfrequency);
    portsend(query, length);

    return 0;
}

unsigned int lenboard::getsinusfrequency() const {
    return sinusfrequency;
}

int lenboard::setsquarefrequency(unsigned int freq){
    if(hserial < 0)
        return -5;
    if(freq > 10000 && checkrange)
        return -1;

    squarefrequency = freq;

    char query[50];
    int length = sprintf(query, "sig_squ %d %d\x0D\x0A", squarefrequency, squareratio);
    portsend(query, length);

    return 0;
}

unsigned int lenboard::getsquarefrequency() const {
    return squarefrequency;
}

int lenboard::setsquareratio(int ratio){
    if(hserial < 0)
        return -5;
    if((ratio > 99 || ratio < 1) && checkrange)
        return -1;

    squareratio = ratio;

    char query[50];
    int length = sprintf(query, "sig_squ %d %d\x0D\x0A", squarefrequency, squareratio);
    portsend(query, length);

    return 0;
}

int lenboard::getsquareratio() const {
    return squareratio;
}

int lenboard::setsamplerate(unsigned long int sampless){
    if(hserial < 0)
        return -5;
    if(sampless > 400000 && checkrange)
        return -1;
    if( sampless == 70000 || sampless == 132000 || sampless == 136000 ) //work around strange controller bug (?) channels swap on these samplerates
        sampless += 1000;

    samplerate = sampless;

    char query[50];
    int length = sprintf(query, "dso_cfg %d %lu\x0D\x0A", flagstonum(activechannels), samplerate);
    portsend(query, length);

    return 0;
}

long unsigned int lenboard::getsamplerate() const {
    return samplerate;
}


int lenboard::setactivechannels(int channels){
    if(hserial < 0)
        return -5;
    if(channels > 15 || channels < 0)
        return -1;

    numtoflags(activechannels, channels);

    char query[50];
    int length = sprintf(query, "dso_cfg %d %lu\x0D\x0A", flagstonum(activechannels), samplerate);
    portsend(query, length);

    return 0;
}

int lenboard::activatechannel(int channel, bool active){
    if(hserial < 0)
        return -5;
    if(channel > 8 || channel < 1)
        return -1;

    switch(channel){
        case CH1A: activechannels[0] = active;
            break;

        case CH1B: activechannels[1] = active;
            break;

        case CH2A: activechannels[2] = active;
            break;

        case CH2B: activechannels[3] = active;
            break;

        default:
            return -1;
            break;
    }

    char query[50];
    int length = sprintf(query, "dso_cfg %d %lu\x0D\x0A", flagstonum(activechannels), samplerate);
    portsend(query, length);

    return 0;
}

int lenboard::getchannelactive(int channel) const {
    if(channel > 8 || channel < 1)
        return -1;

    switch(channel){
        case CH1A: return activechannels[0];
            break;

        case CH1B: return activechannels[1];
            break;

        case CH2A: return activechannels[2];
            break;

        case CH2B: return activechannels[3];
            break;

        default:
            return -1;
            break;
    }
    return -1;
}

int lenboard::setoffsetchannel(int channel, bool active){
    if(hserial < 0)
        return -5;
    if(channel > 8 || channel < 1)
        return -1;

    switch(channel){
        case CH1A: offsetchannels[0] = active;
            break;

        case CH1B: offsetchannels[1] = active;
            break;

        case CH2A: offsetchannels[2] = active;
            break;

        case CH2B: offsetchannels[3] = active;
            break;

        default:
            return -1;
            break;
    }

    char query[50];
    int length = sprintf(query, "dso_oft %d\x0D\x0A", flagstonum(offsetchannels));
    portsend(query, length);

    return 0;
}

int lenboard::setoffset(int channels){
    if(hserial < 0)
        return -5;
    if(channels > 15 || channels < 0)
        return -1;

    numtoflags(offsetchannels, channels);

    char query[50];
    int length = sprintf(query, "dso_oft %d\x0D\x0A", flagstonum(offsetchannels));
    portsend(query, length);

    return 0;
}

int lenboard::getoffset(int channel) const {
    if(channel > 8 || channel < 1)
        return -1;

    switch(channel){
        case CH1A: return offsetchannels[0];
            break;

        case CH1B: return offsetchannels[1];
            break;

        case CH2A: return offsetchannels[2];
            break;

        case CH2B: return offsetchannels[3];
            break;

        default:
            return -1;
            break;
    }
    return -1;
}

int lenboard::setvoltagedivision(int channel, int div){
    if(hserial < 0)
        return -5;
    if((channel != CH1 && channel != CH2) || (div < 0 || div > 3))
        return -1;

    if(channel == CH1)
        vdivision[0] = div;
    if(channel == CH2)
        vdivision[1] = div;

    char query[50];
    int length = sprintf(query, "dso_div %u\x0D\x0A", (vdivision[0]+(vdivision[1]*4)));
    portsend(query, length);

    return 0;
}

int lenboard::getvoltagedevision(int channel) const {
    if(channel != CH1 && channel != CH2)
        return -1;

    switch(channel){
        case CH1: return vdivision[0];
            break;
        case CH2: return vdivision[1];
            break;
    }

    return -1;
}

void lenboard::stopmeasure(){
    if(hserial < 0)
        return;

    char query[] = "dso_stop\x0D\x0A";
    portsend(query, 10);
}

int lenboard::measure(){
    if(hserial < 0)
        return -5;

    setvoltagedivision(CH1, vdivision[0]);
    setoffsetchannel(CH1A, offsetchannels[0]);
    setsamplerate(samplerate);

    char query[] = "dso_arm\x0D\x0A";
    portsend(query, 9);

    //calculate measurement time and wait
    long unsigned int measuretime = (16400 * 1000) / ((activechannels[0] ? 1 : 0)+(activechannels[1] ? 1 : 0)+(activechannels[2] ? 1 : 0)+(activechannels[3] ? 1 : 0)) / (samplerate/1000);
    usleep((unsigned int)measuretime);

    unsigned int pointer = 0;
    unsigned int failcount = 0;

    do{
        if(read(hserial, measurement+pointer, 1) < 1){
            if(failcount > 100)
                break;
            usleep(500);
            failcount++;
        }else{
            failcount = 0;
            pointer++;
        }
    }while(pointer < 16500);

    if(pointer > 10){
        // check header
        if(!(measurement[0] == 0xDE && measurement[1] == 0xAD && measurement[2] == 0xBE && measurement[3] == 0xEF))
            return -2;
        if(samplerate != (unsigned long int)(measurement[4]*16777216 + measurement[5]*65536 + measurement[6]*256 + measurement[7]))
            return -2;
        if(measurement[8] != flagstonum(activechannels))
            return -2;

        measurementlength = pointer-9;
        measuredvalues = measurement+9;
        return 0;
    }else{
        stopmeasure();
        return -1;
    }

    stopmeasure();
    return -1;
}

unsigned char* lenboard::getrawmeasurement() const {
    return measuredvalues;
}

int lenboard::getrawvaluecount() const {
    if( vdivision[0] == 3 || vdivision[1] == 3 )
        return measurementlength-4;
    else
        return measurementlength;
}

int lenboard::getvaluecount() const {
    return measurementlength/((activechannels[0] ? 1 : 0)+(activechannels[1] ? 1 : 0)+(activechannels[2] ? 1 : 0)+(activechannels[3] ? 1 : 0));
}

int lenboard::getvalue(int count, int channel) const {
    int chnum = 0;
    switch(channel){
        case CH1A:
            if(!activechannels[0])
                return -1;
            chnum = 0;
            break;
        case CH1B:
            if(!activechannels[1])
                return -1;
            chnum = 1;
            break;
        case CH2A:
            if(!activechannels[2])
                return -1;
            chnum = 2;
            break;
        case CH2B:
            if(!activechannels[3])
                return -1;
            chnum = 3;
            break;
    }

    unsigned int offset = count*((activechannels[0] ? 1 : 0)+(activechannels[1] ? 1 : 0)+(activechannels[2] ? 1 : 0)+(activechannels[3] ? 1 : 0))+chnum;
    if(offset < measurementlength)
        return measuredvalues[offset];

    return -1;
}


portlist::portlist(){
    listcount = 0;
    listpos = 0;
    refresh();
    return;
}

int portlist::refresh(){
    DIR *pdir = NULL;
    pdir = opendir("/dev");
    struct dirent *pentry = NULL;
    if(pdir == NULL)
        return -1;

    listcount = 0;
    while((pentry = readdir(pdir))){
        #if defined(OS_MAC)
        if((pentry->d_name)[0] == 'c' && (pentry->d_name)[1] == 'u' && (pentry->d_name)[2] == '.' && listcount < 30)
        #elif defined(OS_LINUX)
        if((pentry->d_name)[0] == 't' && (pentry->d_name)[1] == 't' && (pentry->d_name)[2] == 'y' && (pentry->d_name)[3] >= 'A' && (pentry->d_name)[3] <= 'Z' && listcount < 30)
        #endif
        {
            strcpy(list[listcount], (pentry->d_name));
            listcount++;
        }
    }
    return 0;
}

int portlist::count(){
    return listcount;
}

char* portlist::portbynumber(int num){
    return list[num];
}

bool portlist::portwalk(char* dest){
    if(listpos >= listcount){
        listpos = 0;
        return false;
    }

    strcpy(dest, list[listpos]);
    listpos++;
    return true;
}
