//
// Created by Max on 04.03.2025.
//
#ifndef NEO6_PARSER_GPS_MODULE_H
#define NEO6_PARSER_GPS_MODULE_H

#define START_SENTENCE_DATA_INDEX 7
#define GPS_DEBUG_MODE

#ifdef GPS_DEBUG_MODE
    #define IS_NOT_EMPTY_DATA
    #define SYNTHETIC_SENTENCE
    #ifdef SYNTHETIC_SENTENCE
        extern unsigned  char rx_buff[];
    #endif // SYNTHETIC_SENTENCE
#endif //GPS_DEBUG_MODE

///@todo It should be uncommented in future.
/*extern char geoTag[] = "$GPGGA";
  extern char latTag[] = "$GPGLL"; */

typedef enum{
    GPGGA,    ///< GPS fix data (position, time, fix status).
    GPGLL,    ///< Latitude and longitude with time of position fix.
    GPGSA,    ///< GPS fix type, active satellites, and dilution of precision.
    GPGSV,    ///< Satellites in view (detailed satellite info).
    GPRMC,    ///< Recommended minimum navigation info (position, speed, course).
    GPVTG,    ///< Speed and course over ground.
    GPZDA,    ///< UTC time and date.
    GPBOD,    ///< Bearing and distance from origin to destination.
    GPWPL,    ///< Waypoint location.
    GPXTE,    ///< Cross track error.

}gpsMessageType;

typedef struct {
    unsigned char time[9];
    unsigned char latitude[14];
    unsigned char longitude[14];
    unsigned char altitude[7];
    unsigned char fixStatus;
    unsigned char satellitesInUse[2];
    unsigned char hdop[3];
} ggaData;

unsigned char calculateChecksum(const unsigned char *sentence);

bool checkSentence(const unsigned char *sentence);

int parseGGA(unsigned char* buff, ggaData* gps);

#endif //NEO6_PARSER_GPS_MODULE_H
