#include "stdio.h"
#include "GPS_Module/GPS_Module.h"

int main() {
    ggaData gps;

    printf("Simple test of GPGGA sentence.\n");
    parseGGA(rx_buff, &gps);
    return 0;
}