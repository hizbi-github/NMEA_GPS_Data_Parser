#include <stdio.h>
#include <string>

using namespace std;

class NMEA_GPS_Parser
{
public:
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

    NMEA_GPS_Parser();
    NMEA_GPS_Parser(string rawData);
    void initializeParams();
    string getGPSRawData();
    void parseGPSRawData();
    void parseGPSRawData(string rawData);
    int findNumberOfCommas(string rawData, int commaPositions[]);
    bool verifyNMEACompliance(string rawData);
    int extractEmbeddedChecksum(string rawData);
    int calculateChecksum(string rawData);
    void extractGPSData(string rawData, int commaPositions[]);
    bool isParsingSuccessful();
    void printParsedData();

private:
    string gpsRawData;
    bool parsingSuccessful;
};
