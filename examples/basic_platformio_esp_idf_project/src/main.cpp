#include <stdio.h>
#include <string>
#include "nmea_gps_parser.hpp"

using namespace std;

extern "C" 
{    
    void app_main(void);
}

void app_main() 
{
    string gpsRawData = "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E\r\n";

    printf("\n");
    printf(R"(> Raw Data: $GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E\r\n)");
	printf("\n");

    NMEA_GPS_Parser gpsData(gpsRawData);

    if (gpsData.isParsingSuccessful() == true)
    {
        gpsData.printParsedData();
    }
    else
    {
        printf("\n> [ERROR] GPS raw data parsing failed!\n");
    }
}