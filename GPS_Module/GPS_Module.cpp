//
// Created by Max on 04.03.2025.
//
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "GPS_Module.h"

#ifdef SYNTHETIC_SENTENCE
    #ifdef IS_NOT_EMPTY_DATA
    unsigned char rx_buff[] = {
            '$', 'G', 'P', 'G', 'G', 'A', ',', '1', '2', '3', '5', '1', '9', ',',
            '4', '8', '0', '7', '.', '0', '3', '8', ',', 'N', ',', '0', '1', '1',
            '3', '1', '.', '0', '0', '0', ',', 'E', ',', '1', ',', '0', '8', ',',
            '0', '.', '9', ',', '5', '4', '5', '.', '4', ',', 'M', ',', '4', '6',
            '.', '9', ',', 'M', ',', ',', '*', '4', '7'
    };
    #endif //IS_NOT_EMPTY_DATA

    #ifndef IS_NOT_EMPTY_DATA
    unsigned char rx_buff[] = {
            '$', 'G', 'P', 'G', 'G', 'A', ',', ',',
            ',', ',', '0', '1', '1',',',',', '0', '8', ',',
            '0', '.', '9', ',', '5', '4', '5', '.', '4', ',', 'M', ',', '4', '6',
            '.', '9', ',', 'M', ',', ',', ',', '*', '4', '7'
    };
    #endif //IS_NOT_EMPTY_DATA
#endif // SYNTHETIC_SENTENCE

unsigned char calculateChecksum(const unsigned char *sentence)
{
    unsigned char checksum = 0;

    for (int i = 1; sentence[i] != '*' && sentence[i] != '\0'; i++) {
        checksum ^= sentence[i];  // XOR
    }
    return checksum;
}

bool checkSentence(const unsigned char *sentence)
{
    int index = 0;
    unsigned char calculatedChecksum;
    bool result = false;
    char* currentChecksum = (char*)malloc(3 * sizeof (char));
    char* calculatedChecksumStr = (char*)malloc(3 * sizeof (char));

    while (sentence[index] != '*' && sentence[index] != '\0') {
        index++;
    }

    if(sentence[index] == '*') {
        currentChecksum[0] = sentence[index+1];
        currentChecksum[1] = sentence[index+2];
        currentChecksum[2] = '\0';
    } else {
        return result;
    }

    calculatedChecksum = calculateChecksum(sentence);
    sprintf(calculatedChecksumStr, "%02X", calculatedChecksum);
    result = strcmp(calculatedChecksumStr, currentChecksum) == 0;

    free(currentChecksum);
    return result;
}

int parseGGA(unsigned char* buff, ggaData* gps)
{
    uint8_t index = START_SENTENCE_DATA_INDEX;
    uint8_t inc = 0;

    if (!checkSentence(buff)) {
#ifdef GPS_DEBUG_MODE
        printf("Error 001. Checksum isn't correct.\n");
#endif
        return 0;
    }

    /*------------Time------------*/
    while (buff[index] != ',') {
        gps->time[inc++] = buff[index++];
        if(inc == 2 || inc == 5){
            gps->time[inc++] = ':';
        }
    }
    gps->time[inc] = '\0';
    inc = 0;
    index++;

    /*------------Latitude------------*/
    while (buff[index] != ',') {
        gps->latitude[inc++] = buff[index++];
        if(inc == 2){
            gps->latitude[inc++] = '\xC2';   // First byte of °
            gps->latitude[inc++] = '\xB0';   // Second byte of °
        }
    }

    /// Parse cardinal direction
    gps->latitude[inc++] = '\'';
    index++;
    while (buff[index] != ',') {
        gps->latitude[inc++] = buff[index++];
    }
    gps->latitude[inc++] = '\0';
    index++;
    inc = 0;

    /*------------Longitude------------*/
    while (buff[index] != ',') {
        gps->longitude[inc++] = buff[index++];
        if(inc == 2){
            gps->longitude[inc++] = '\xC2';  // First byte of °
            gps->longitude[inc++] = '\xB0';  // Second byte of °
        }
    }
    gps->longitude[inc++] = '\'';
    index++;

    /// Parse cardinal direction
    while (buff[index] != ',') {
        gps->longitude[inc++] = buff[index++];
    }
    gps->longitude[inc++] = '\0';
    index++;
    inc = 0;

    /*------------Fix status------------*/
    while (buff[index] != ',') {
        gps->fixStatus = buff[index++];
    }
    index++;

    /*------------Satellites in use------------*/
    while (buff[index] != ',') {
        gps->satellitesInUse[inc++] = buff[index++];
    }
    gps->satellitesInUse[inc++] = '\0';
    index++;
    inc = 0;

    /*------------Horizontal Dilution of Precision------------*/
    while (buff[index] != ',') {
        gps->hdop[inc++] = buff[index++];
    }
    gps->hdop[inc++] = '\0';
    index++;
    inc = 0;

    /*------------Altitude------------*/
    while (buff[index] != ',') {
        gps->altitude[inc++] = buff[index++];
    }
    index++;
    while (buff[index] != ',') {
        gps->altitude[inc++] = buff[index++];
    }
    gps->altitude[inc++] = '\0';

#ifdef GPS_DEBUG_MODE
    printf("-----------$GPGGA----------\n");
    printf("Time: %s\n", gps->time);
    printf("Latitude: %s\n", gps->latitude);
    printf("Longitude: %s\n",  gps->longitude);
    printf("Altitude: %s\n",  gps->altitude);
    printf("Fix status: %c\n",  gps->fixStatus);
    printf("Satellites in use: %s\n",  gps->satellitesInUse);
    printf("HDOP: %s\n",  gps->hdop);
    printf("--------------------------\n");
#endif

    return 1;
}