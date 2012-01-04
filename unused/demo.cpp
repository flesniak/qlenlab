/************************************************************************
 * Copyright (C) 2011 Lars Wolff <lars.wolff@student.kit.edu>           *
 *                                                                      *
 * This file is part of the LENlib project.                             *
 *                                                                      *
 * LENlib is free software: you can redistribute it and/or modify       *
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

#include <iostream>
#include <string.h>
#include "LENlib.h"     // include class header file

int main (int argc, const char * argv[])
{
    //////////////////////////////////
    // select and open a port:

    portlist ports;     // create object that can list the ports
    lenboard board;     // create object which we want to use to communicate with the LENlab module

    while(1){
        // print out portlist using portlist::portbynumber();
        for(int i=0; i < ports.count(); i++){
            std::cout << i << ": " << ports.portbynumber(i) << "\n";
        }

        // alternative: print out portlist using portlist::portwalk()
        /*
         char string[50];
         while (ports.portwalk(string)) {
         std::cout << string << "\n";
         }
         */

        std::cout << "\nSchnittstelle für LENlab Modul auswählen: ";
        int entry;
        std::cin >> entry;
        std::cout << "\n";

        if(entry > ports.count() || entry < 0){   // check if user entry is a valid number
            std::cout << "Bitte eine Zahl zwischen 0 und " << ports.count() << " eingeben\n";
            continue;
        }

        // open the port
        if(board.openport(ports.portbynumber(entry)) < 0){
            std::cout << "Schnittstelle konnte nicht geöffnet werden\n\n";
            continue;
        }else{
            std::cout << "Schnittstelle erfolgreich geöffnet\n";
            std::cout << "Modul: " << board.getid() << "\n";
            break;
        }

    }


    //////////////////////////////////
    // set parameters:

    // note:
    // if you define Channels use:
    // CH1A     = pin Con2/IN1 on the board
    // CH1B     = pin Con2/IN2 on the board
    // CH2A     = pin Con2/IN3 on the board
    // CH2B     = pin Con2/IN4 on the board
    //
    // CH1A and CH1B are grouped to CH1; CH2A and CH2B are grouped to CH2

    board.setsinusfrequency(2000);              // set sinus signal frequency to 2000 Hz

    board.setsquarefrequency(5000);             // set sqare signal frequency to 5000 Hz
    board.setsquareratio(25);                   // on-off ratio for sqare signal to 1/4: (1-99) where 1 is the shortest on-time

    board.setoffset(CH1A | CH1B | CH2A | CH2B); // if the signal offset is deactivated only positive voltages can be measured
    board.setoffsetchannel(CH2A, false);        // deactivate offset for Channel 2A

    board.setvoltagedivision(CH1, VDIV_2);      // set the voltage division (3.3V AD-range * 2 = 6.6V range) for Channel 1 (CH1A and CH1B)
    board.setvoltagedivision(CH2, VDIV_10);     // voltage division can only be set for input groups CH1 and CH2 not for i.e. CH1A sepeartely.

    board.setsamplerate(100000);                // set the samplerate to 100000 samples/sec
    board.setactivechannels(CH1A | CH1B);       // record on Channels CH1A and CH1B
    board.activatechannel(CH2B, true);          // activate also CH2B

    // note:
    // you don't need to call all these functions on every startup.
    // There are some default values:
    // voltage division:    1 (3.3V range) for both Channels
    // offset channels:     offset enabled on all channels
    // samplerate:          100000
    // active channels:     CH1A activated
    // sqareratio:          50
    //
    // so a minimum of what you got to do is:
    // set sinus frequency; set sqare frequency; measure;

    // some getters:
    std::cout << "Sinus-Frequenz:   " << board.getsinusfrequency() << "\n";
    std::cout << "Recheck-Frequenz: " << board.getsquarefrequency() << "\n";
    std::cout << "Tastverhältnis:   " << board.getsquareratio() << "\n\n";
    std::cout << "Offset für Kanal 1A ist " << (board.getoffset(CH1A) ? "aktiv" : "nicht aktiv") << "\n";
    char vdiv[10] = "??";
    switch(board.getvoltagedevision(CH1)){
        case VDIV_05:
            strcpy(vdiv, "1,6 V");
            break;
        case VDIV_1:
            strcpy(vdiv, "3,3 V");
            break;
        case VDIV_2:
            strcpy(vdiv, "6,6 V");
            break;
        case VDIV_10:
            strcpy(vdiv, "33 V");
            break;
    }
    std::cout << "Kanal 1A/B hat einen Spannungbereich von: " << vdiv << "\n";
    std::cout << "Kanal 1A ist für die Messung " << (board.getchannelactive(CH1A) ? "aktiv" : "nicht aktiv") << "\n";
    std::cout << "Es wird mit " << board.getsamplerate() << " Samples/Sekunde gemessen\n\n";

    std::cout << "Enter drücken um die Messung zu starten\n";
    std::cin.clear();
    std::cin.ignore(255, '\n');
    std::cin.get();

    //////////////////////////////////
    // measurement:

    board.measure();            // measure. This method will take some time to finish, depending on samplerate and active channels

    for(int i=0; i < board.getvaluecount(); i++){   // getvaluecount gives you the number of values measured for each channel

        int value = board.getvalue(i, CH1A);        // get a measured value by index (values are in the range of 0...255)
        for(int j=0; j < value; j += 4){            // present the data to the user
            std::cout << " ";
        }
        std::cout << "+\n";
    }

    board.closeport();      // close the port so other programms can access the port.


    //////////////////////////////////
    // other functions:

    // lenboard::freeme(true)           // deactivates value checking - i.e. enables you to set a samplerate of > 400000
    //                                  // the effects of such unusual operations are untested

    // lenboard::getrawmeasurement()    // provides to you a pointer to the 'raw' measurement data - not seperated in channels
    //                                  // usually you won't need this function

    // lenboard::stopmeasure()          // sends a stop measurement message to the controller board. Might become interesting when
    //                                  // starting a measurement in a sperate thread

    return 0;
}
