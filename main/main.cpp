//#include <esp_log.h>
#include <stdio.h>
#include <string>

using namespace std;

extern "C" 
{    
    void app_main(void);
}

struct GPS_PARSED_DATA
{
    string talkerID;
    string messageID;
    double utcTimeStamp;
    double latitude;
    char northOrSouthIndicator;
    double longitude;
    char eastOrWestIndicator;
    int qualityIndicator;
    int numberOfSatellites;
    double horizontalDilutionPrecision;
    double altitude;
    double geoidalSeparation;
    string dgpsStationID;
    int checksum;
}; 

void app_main() 
{
    //ESP_LOGI("MAIN", "Hello World!");

    string gpsRawData = "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E";

    int rawDataLength = gpsRawData.length(); 
    int nmeaMessageLength = rawDataLength - 4; // Not accounting for $, *, and two HEX chars for checksum 
    int nmeaMessageStartIndex = 1;
    
    if (gpsRawData[0] == '$' && gpsRawData[rawDataLength - 1 - 2] == '*' && rawDataLength <= 82)
    {
        printf("\n> Raw data detected of length %d", rawDataLength);
        printf("\n> Calculating checksum...");

        string embeddedChecksumHex = string{gpsRawData[rawDataLength - 2], gpsRawData[rawDataLength - 1]};
        int embeddedChecksum = stoi(embeddedChecksumHex, 0, 16);
        int calculatedChecksum = 0;

        printf("\n> Embedded checksum: %s (HEX) or %d (INT)", embeddedChecksumHex.c_str(), embeddedChecksum);

        for (int index = nmeaMessageStartIndex; index <= nmeaMessageLength; index++)
        {
            //printf("\n> Loop index: %d, char: %c", index, gpsRawData[index]);

            if (index == nmeaMessageStartIndex)
            {
                calculatedChecksum = int(gpsRawData[index]);
            }
            else
            {
                calculatedChecksum = calculatedChecksum ^ int(gpsRawData[index]);
            }
        }

        printf("\n> Calculated checksum: %d (HEX) or %d (INT)", calculatedChecksum, calculatedChecksum);
        
        if (calculatedChecksum == embeddedChecksum)
        {
            printf("\n> The calculated checksum '%d' matches the embedded checksum '%d'", calculatedChecksum, embeddedChecksum);
            printf("\n> Raw data integrity verified!");
        }

        printf("\n> Parsing the message contents...");
        //printf("\n> Parsing the message contents...");
        //printf("\n> Parsing the message contents...");
    }


    //string testString = "\n> " + gpsRawData + "\n";

    //printf("\n> Hello World!\n");
    //printf(testString.c_str());
}