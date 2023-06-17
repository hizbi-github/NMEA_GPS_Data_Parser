#include <nmea_gps_parser_functional.hpp>

NMEA_GPS_Parsed_Data initializeParams()
{
    NMEA_GPS_Parsed_Data parsedDataDefault;

    parsedDataDefault.gpsRawData = "";
    parsedDataDefault.talkerID = "";
    parsedDataDefault.messageID = "";
    parsedDataDefault.utcTimeStamp = "";
    parsedDataDefault.utcTimeStampHours = 0;
    parsedDataDefault.utcTimeStampMinutes = 0;
    parsedDataDefault.utcTimeStampSeconds = 0.0;
    parsedDataDefault.latitude = "";
    parsedDataDefault.latitudeDegrees = 0;
    parsedDataDefault.latitudeMinutes = 0.0;
    parsedDataDefault.northOrSouthIndicator = '\0';
    parsedDataDefault.longitude = "";
    parsedDataDefault.longitudeDegrees = 0;
    parsedDataDefault.longitudeMinutes = 0.0;
    parsedDataDefault.eastOrWestIndicator = '\0';
    parsedDataDefault.qualityIndicator = 0;
    parsedDataDefault.numberOfSatellites = 0;
    parsedDataDefault.horizontalDilutionPrecision = 0.0;
    parsedDataDefault.altitude = 0.0;
    parsedDataDefault.altitudeUnit = '\0';
    parsedDataDefault.geoidalSeparation = 0.0;
    parsedDataDefault.geoidalSeparationUnit = '\0';
    parsedDataDefault.ageOfDGPSData = 0.0;
    parsedDataDefault.dgpsStationID = -1; // Valid data range is from 0000 - 1023 for DGPS Station ID
    parsedDataDefault.embeddedChecksum = 0;
    parsedDataDefault.parsingSuccessful = false;

    return(parsedDataDefault);
}

NMEA_GPS_Parsed_Data parseGPSRawData(string rawData)
{
    NMEA_GPS_Parsed_Data parsedData = initializeParams();

    parsedData.gpsRawData = rawData;

    int commaPostions[14];
    int numberOfCommasPerStandard = 14;

    if (verifyNMEACompliance(rawData) == true && findNumberOfCommas(rawData, commaPostions) == numberOfCommasPerStandard)
    {
        parsedData.embeddedChecksum = extractEmbeddedChecksum(rawData);

        if (calculateChecksum(rawData) == parsedData.embeddedChecksum)
        {
            extractGPSData(rawData, parsedData, commaPostions);
            parsedData.parsingSuccessful = true;
        }
        else
        {
            printf("\n> [ERROR] The NEMA-0183 GPS raw message failed integrity check!\n");
            parsedData.parsingSuccessful = false;
        }
    }
    else
    {
        printf("\n> [ERROR] The raw message doesn't resemble the NEMA-0183 GPS format!\n");
        parsedData.parsingSuccessful = false;
    }

    return(parsedData);
}

int findNumberOfCommas(string rawData, int commaPositions[])
{
    int rawDataLength = rawData.length(); 
    int numberOfCommas = 0;
    int commaIndex = 0;

    for (int index = 0; index < rawDataLength; index++)
    {
        if (rawData[index] == ',') 
        {                                                      
            numberOfCommas = numberOfCommas + 1;
            commaPositions[commaIndex] = index;
            commaIndex = commaIndex + 1; 
        }
    }

    return(numberOfCommas);
}

bool verifyNMEACompliance(string rawData)
{
    int rawDataLength = rawData.length(); 

    if (rawDataLength <= 82 && rawData[0] == '$' && rawData.substr(1, 5) == "GPGGA" 
        && rawData[rawDataLength - 5] == '*' && rawData[rawDataLength - 2] == '\r' 
        && rawData[rawDataLength - 1] == '\n')
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

int extractEmbeddedChecksum(string rawData)
{
    int rawDataLength = rawData.length(); 
    string embeddedChecksumHex = string{rawData[rawDataLength - 4], rawData[rawDataLength - 3]};
    int embeddedChecksum = stoi(embeddedChecksumHex, 0, 16); // Converting from Hexadecimal number in string to its equivalent in integer

    return(embeddedChecksum);
}

int calculateChecksum(string rawData)
{
    int rawDataLength = rawData.length(); 
    int nmeaMessageLength = rawDataLength - 6; // Not accounting for $, *, two HEX chars for checksum and CRLF chars
    int nmeaMessageStartIndex = 1; // Starting from the 2nd index since first index has '$' character
    int calculatedChecksum = 0;

    for (int index = nmeaMessageStartIndex; index <= nmeaMessageLength; index++)
    {
        if (index == nmeaMessageStartIndex)
        {
            calculatedChecksum = int(rawData[index]);
        }
        else
        {
            calculatedChecksum = calculatedChecksum ^ int(rawData[index]);
        }
    }

    return(calculatedChecksum);
}

void extractGPSData(string rawData, NMEA_GPS_Parsed_Data &parsedData, int commaPositions[])
{
    int asteriskPosition = rawData.find("*");

    string talkerIDTemp = string{rawData[1], rawData[2]};
    string messageIDTemp = string{rawData[3], rawData[4], rawData[5]};

    string utcTimeStampTemp = rawData.substr(commaPositions[0] + 1, commaPositions[1] - commaPositions[0] - 1);

    string latitudeTemp = rawData.substr(commaPositions[1] + 1, commaPositions[2] - commaPositions[1] - 1);
    string northOrSouthIndicatorTemp = rawData.substr(commaPositions[2] + 1, commaPositions[3] - commaPositions[2] - 1);

    string longitudeTemp = rawData.substr(commaPositions[3] + 1, commaPositions[4] - commaPositions[3] - 1);
    string eastOrWestIndicatorTemp = rawData.substr(commaPositions[4] + 1, commaPositions[5] - commaPositions[4] - 1);

    string qualityIndicatorTemp = rawData.substr(commaPositions[5] + 1, commaPositions[6] - commaPositions[5] - 1);
    string numberOfSatellitesTemp = rawData.substr(commaPositions[6] + 1, commaPositions[7] - commaPositions[6] - 1);
    string horizontalDilutionPrecisionTemp = rawData.substr(commaPositions[7] + 1, commaPositions[8] - commaPositions[7] - 1);

    string altitudeTemp = rawData.substr(commaPositions[8] + 1, commaPositions[9] - commaPositions[8] - 1);
    string altitudeUnitTemp = rawData.substr(commaPositions[9] + 1, commaPositions[10] - commaPositions[9] - 1);

    string geoidalSeparationTemp = rawData.substr(commaPositions[10] + 1, commaPositions[11] - commaPositions[10] - 1);
    string geoidalSeparationUnitTemp = rawData.substr(commaPositions[11] + 1, commaPositions[12] - commaPositions[11] - 1);

    string ageOfDGPSDataTemp = rawData.substr(commaPositions[12] + 1, commaPositions[13] - commaPositions[12] - 1);
    string dgpsStationIDTemp = rawData.substr(commaPositions[13] + 1, asteriskPosition - commaPositions[13] - 1);

    if (talkerIDTemp != "") {parsedData.talkerID = talkerIDTemp;}
    if (messageIDTemp != "") {parsedData.messageID = messageIDTemp;}

    if (utcTimeStampTemp != "")
    {
        parsedData.utcTimeStamp = utcTimeStampTemp;
        parsedData.utcTimeStampHours = stoi(parsedData.utcTimeStamp.substr(0, 2));
        parsedData.utcTimeStampMinutes = stoi(parsedData.utcTimeStamp.substr(2, 2));
        parsedData.utcTimeStampSeconds = stod(parsedData.utcTimeStamp.substr(4));
    }

    if (latitudeTemp != "")
    {
        parsedData.latitude = latitudeTemp;
        parsedData.latitudeDegrees = stoi(parsedData.latitude.substr(0, 2));
        parsedData.latitudeMinutes = stod(parsedData.latitude.substr(2));
    }

    if (northOrSouthIndicatorTemp != "" && (northOrSouthIndicatorTemp == "N" || northOrSouthIndicatorTemp == "S")) 
    {
        parsedData.northOrSouthIndicator = northOrSouthIndicatorTemp[0];
    }

    if (longitudeTemp != "")
    {
        parsedData.longitude = longitudeTemp;
        parsedData.longitudeDegrees = stoi(parsedData.longitude.substr(0, 3));
        parsedData.longitudeMinutes = stod(parsedData.longitude.substr(3));
    }

    if (eastOrWestIndicatorTemp != "" && (eastOrWestIndicatorTemp == "E" || eastOrWestIndicatorTemp == "W")) 
    {
        parsedData.eastOrWestIndicator = eastOrWestIndicatorTemp[0];
    }

    if (qualityIndicatorTemp != "") {parsedData.qualityIndicator = stoi(qualityIndicatorTemp);}
    if (numberOfSatellitesTemp != "") {parsedData.numberOfSatellites = stoi(numberOfSatellitesTemp);}
    if (horizontalDilutionPrecisionTemp != "") {parsedData.horizontalDilutionPrecision = stod(horizontalDilutionPrecisionTemp);}

    if (altitudeTemp != "") {parsedData.altitude = stod(altitudeTemp);}
    if (altitudeUnitTemp != "") {parsedData.altitudeUnit = altitudeUnitTemp[0];}

    if (geoidalSeparationTemp != "") {parsedData.geoidalSeparation = stod(geoidalSeparationTemp);}
    if (geoidalSeparationUnitTemp != "") {parsedData.geoidalSeparationUnit = geoidalSeparationUnitTemp[0];}

    if (ageOfDGPSDataTemp != "") {parsedData.ageOfDGPSData = stod(ageOfDGPSDataTemp);}
    if (dgpsStationIDTemp != "") {parsedData.dgpsStationID = stoi(dgpsStationIDTemp);}
}

void printParsedData(NMEA_GPS_Parsed_Data parsedData)
{
    printf("\n> Talker ID: %s", parsedData.talkerID.c_str());
    printf("\n> Message ID: %s", parsedData.messageID.c_str());

    printf("\n> UTC TimeStamp: %s", parsedData.utcTimeStamp.c_str());
    printf("\n> UTC TimeStamp Hours: %d", parsedData.utcTimeStampHours);
    printf("\n> UTC TimeStamp Minutes: %d", parsedData.utcTimeStampMinutes);
    printf("\n> UTC TimeStamp Seconds: %f", parsedData.utcTimeStampSeconds);

    printf("\n> Latitude: %s", parsedData.latitude.c_str());
    printf("\n> Latitude Degrees: %d", parsedData.latitudeDegrees);
    printf("\n> Latitude Minutes: %f", parsedData.latitudeMinutes);
    printf("\n> North/South Indicator: %c", parsedData.northOrSouthIndicator);

    printf("\n> Longitude: %s", parsedData.longitude.c_str());
    printf("\n> Longitude Degrees: %d", parsedData.longitudeDegrees);
    printf("\n> Longitude Minutes: %f", parsedData.longitudeMinutes);
    printf("\n> East/West Indicator: %c", parsedData.eastOrWestIndicator);

    printf("\n> Quality Indicator: %d", parsedData.qualityIndicator);
    printf("\n> Number Of Satellites: %d", parsedData.numberOfSatellites);
    printf("\n> Horizontal Dilution Precision: %f", parsedData.horizontalDilutionPrecision);

    printf("\n> Altitude: %f", parsedData.altitude);
    printf("\n> Altitude Unit: %c", parsedData.altitudeUnit);

    printf("\n> Geoidal Separation: %f", parsedData.geoidalSeparation);
    printf("\n> Geoidal Separation Unit: %c", parsedData.geoidalSeparationUnit);

    printf("\n> Age Of DGPS Data: %f", parsedData.ageOfDGPSData);
    printf("\n> DGPS Station ID: %d\n", parsedData.dgpsStationID);  
}