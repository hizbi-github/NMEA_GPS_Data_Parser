#include <stdio.h>
#include <string>
#include <nmea_gps_parser.hpp>

using namespace std;

extern "C" 
{    
    void app_main(void);
}

void app_main() 
{
    string gpsRawDataOne = "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E\r\n";
    string gpsRawDataTwo = "$GPGGA,001043.00,4404.14036,N,12118.85961,W,1,12,0.98,1113.0,M,-21.3,M,,*59\r\n";
    string gpsRawDataThree = "$GPGGA,123456.00,1234.1234,A,12345.12345,B,1,12,0.98,1234.0,M,-12.3,M,,*78\r\n";
    string gpsRawDataFour = "$GPGGA,,,,,,,,,,,,,,*56\r\n";
    string gpsRawDataFive = "$GPGGA,,,,,,,,,,,,,,*7B\r\n";

    string testDataArray[5] = {gpsRawDataOne, gpsRawDataTwo, gpsRawDataThree, gpsRawDataFour, gpsRawDataFive};

    for (int index = 0; index < 4; index++)
    {
        printf("\n");
        printf(R"(> Raw Data: %s)", testDataArray[index].c_str());

        NMEA_GPS_Parser gpsData(testDataArray[index]);

        printf("\n> [INFO] Embedded checksum: %d", gpsData.embeddedChecksum);
        printf("\n> [INFO] Calculated checksum: %d\n", gpsData.calculateChecksum(testDataArray[index]));

        if (gpsData.isParsingSuccessful() == true)
        {
            gpsData.printParsedData();
        }
        else
        {
            printf("\n> [ERROR] GPS raw data parsing failed!\n");
        }
    }
}