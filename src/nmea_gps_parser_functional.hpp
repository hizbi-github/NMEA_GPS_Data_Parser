#include <stdio.h>
#include <string>

using namespace std;

struct NMEA_GPS_Parsed_Data
{
    string gpsRawData;
    string talkerID;
    string messageID;
    string utcTimeStamp;
    int utcTimeStampHours;
    int utcTimeStampMinutes;
    double utcTimeStampSeconds;
    string latitude;
    int latitudeDegrees;
    double latitudeMinutes;
    char northOrSouthIndicator;
    string longitude;
    int longitudeDegrees;
    double longitudeMinutes;
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
    int embeddedChecksum;
    bool parsingSuccessful;
};

NMEA_GPS_Parsed_Data parseGPSRawData(string rawData);
NMEA_GPS_Parsed_Data initializeParams();
int findNumberOfCommas(string rawData, int commaPositions[]);
bool verifyNMEACompliance(string rawData);
int extractEmbeddedChecksum(string rawData);
int calculateChecksum(string rawData);
void extractGPSData(string rawData, NMEA_GPS_Parsed_Data &parsedData, int commaPositions[]);
void printParsedData(NMEA_GPS_Parsed_Data parsedData);