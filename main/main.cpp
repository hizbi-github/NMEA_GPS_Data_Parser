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
    char altitudeUnit;
    double geoidalSeparation;
    char geoidalSeparationUnit;
    double ageOfDGPSData;
    int dgpsStationID;
    int checksum;
}; 

void app_main() 
{
    string gpsRawData = "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E\r\n";
    //string gpsRawData = "$GPGGA,001043.00,4404.14036,N,12118.85961,W,1,12,0.98,1113.0,M,-21.3,M,,*47\r\n";

    int rawDataLength = gpsRawData.length(); 
    int nmeaMessageLength = rawDataLength - 6; // Not accounting for $, *, two HEX chars for checksum and CRLF chars
    int nmeaMessageStartIndex = 1;
    string embeddedChecksumHex = "";
    int embeddedChecksum = 0;
    int calculatedChecksum = 0;
    int numberOfCommas = 0;
    int commaPostions[15];
    int commaIndex = 0;
    bool isRawDataVerified = true;

    printf("\n> Raw data detected of length %d", rawDataLength);

    for (int index = 0; index < rawDataLength; index++)
    {
        //printf("\n> Loop index: %d, char: %d", index, gpsRawData[index]);

        if (gpsRawData[index] == ',' || gpsRawData[index] == '*')
        {
            numberOfCommas = numberOfCommas + 1;
            commaPostions[commaIndex] = index;
            commaIndex = commaIndex + 1; 
        }
    }

    if (rawDataLength <= 82 && numberOfCommas == 15 && gpsRawData[0] == '$' && gpsRawData[rawDataLength - 5] == '*' && gpsRawData[rawDataLength - 2] == '\r' && gpsRawData[rawDataLength - 1] == '\n')
    {
        embeddedChecksumHex = string{gpsRawData[rawDataLength - 4], gpsRawData[rawDataLength - 3]};
        embeddedChecksum = stoi(embeddedChecksumHex, 0, 16);

        printf("\n> Numeber of commas: %d", numberOfCommas);
        printf("\n> Embedded checksum: %s (HEX) or %d (INT)", embeddedChecksumHex.c_str(), embeddedChecksum);
        printf("\n> Calculating checksum...");

        for (int index = nmeaMessageStartIndex; index <= nmeaMessageLength; index++)
        {
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

        isRawDataVerified = true;
    }
    else
    {
        printf("\n> [ERROR] The raw data doesn't resemble the NEMA-0183 format!");
        isRawDataVerified = false;
    }

    if (isRawDataVerified == true)
    {
        printf("\n> Parsing the message contents...");
        printf("\n");
        printf("\n> Comma positions: ");

        for (int index = 0; index < 14; index++)\
        {
            if (index < 13)
            {
                printf("%d, ", commaPostions[index]);
            }
            else
            {
                printf("%d", commaPostions[index]);
            }
        }

        GPS_PARSED_DATA gpsData;

        gpsData.talkerID = string{gpsRawData[1], gpsRawData[2]};
        gpsData.messageID = string{gpsRawData[3], gpsRawData[4], gpsRawData[5]};
        gpsData.utcTimeStamp = stod(gpsRawData.substr(commaPostions[0] + 1, commaPostions[1] - commaPostions[0] - 1));
        gpsData.latitude = stod(gpsRawData.substr(commaPostions[1] + 1, commaPostions[2] - commaPostions[1] - 1));
        gpsData.northOrSouthIndicator = gpsRawData.substr(commaPostions[2] + 1, commaPostions[3] - commaPostions[2] - 1)[0];
        gpsData.longitude = stod(gpsRawData.substr(commaPostions[3] + 1, commaPostions[4] - commaPostions[3] - 1));
        gpsData.eastOrWestIndicator = gpsRawData.substr(commaPostions[4] + 1, commaPostions[5] - commaPostions[4] - 1)[0];
        gpsData.qualityIndicator = stoi(gpsRawData.substr(commaPostions[5] + 1, commaPostions[6] - commaPostions[5] - 1));
        gpsData.numberOfSatellites = stoi(gpsRawData.substr(commaPostions[6] + 1, commaPostions[7] - commaPostions[6] - 1));
        gpsData.horizontalDilutionPrecision = stod(gpsRawData.substr(commaPostions[7] + 1, commaPostions[8] - commaPostions[7] - 1));
        gpsData.altitude = stod(gpsRawData.substr(commaPostions[8] + 1, commaPostions[9] - commaPostions[8] - 1));
        gpsData.altitudeUnit = gpsRawData.substr(commaPostions[9] + 1, commaPostions[10] - commaPostions[9] - 1)[0];
        gpsData.geoidalSeparation = stod(gpsRawData.substr(commaPostions[10] + 1, commaPostions[11] - commaPostions[10] - 1));
        gpsData.geoidalSeparationUnit = gpsRawData.substr(commaPostions[11] + 1, commaPostions[12] - commaPostions[11] - 1)[0];
        //gpsData.ageOfDGPSData = stod(gpsRawData.substr(commaPostions[12] + 1, commaPostions[13] - commaPostions[12] - 1));
        //gpsData.dgpsStationID = stoi(gpsRawData.substr(commaPostions[13] + 1, commaPostions[14] - commaPostions[13] - 1));
        gpsData.ageOfDGPSData = 0.0;
        gpsData.dgpsStationID = 0;

        printf("\n\n");
        printf(R"(> Raw Data: $GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E\r\n)");
        printf("\n");

        printf("\n> gpsData.talkerID: %s", gpsData.talkerID.c_str());
        printf("\n> gpsData.messageID: %s", gpsData.messageID.c_str());
        printf("\n> gpsData.utcTimeStamp: %f", gpsData.utcTimeStamp);
        printf("\n> gpsData.latitude: %f", gpsData.latitude);
        printf("\n> gpsData.northOrSouthIndicator: %c", gpsData.northOrSouthIndicator);
        printf("\n> gpsData.longitude: %f", gpsData.longitude);
        printf("\n> gpsData.eastOrWestIndicator: %c", gpsData.eastOrWestIndicator);
        printf("\n> gpsData.qualityIndicator: %d", gpsData.qualityIndicator);
        printf("\n> gpsData.qualityIndicator: %d", gpsData.numberOfSatellites);
        printf("\n> gpsData.horizontalDilutionPrecision: %f", gpsData.horizontalDilutionPrecision);
        printf("\n> gpsData.horizontalDilutionPrecision: %f", gpsData.altitude);
        printf("\n> gpsData.altitudeUnit: %c", gpsData.altitudeUnit);
        printf("\n> gpsData.geoidalSeparation: %f", gpsData.geoidalSeparation);
        printf("\n> gpsData.geoidalSeparationUnit: %c", gpsData.geoidalSeparationUnit);
        printf("\n> gpsData.ageOfDGPSData: %f", gpsData.ageOfDGPSData);
        printf("\n> gpsData.dgpsStationID: %d\n", gpsData.dgpsStationID);
    }
}