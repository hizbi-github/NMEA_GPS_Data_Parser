#include <nmea_gps_parser.hpp>

NMEA_GPS_Parser::NMEA_GPS_Parser()
{
    initializeParams();
}

NMEA_GPS_Parser::NMEA_GPS_Parser(string gpsRawData)
{
    initializeParams();
    parseGPSRawData(gpsRawData);
}

void NMEA_GPS_Parser::initializeParams()
{
    gpsRawData = "";
    talkerID = "";
    messageID = "";
    utcTimeStamp = "";
    utcTimeStampHours = 0;
    utcTimeStampMinutes = 0;
    utcTimeStampSeconds = 0.0;
    latitude = "";
    latitudeDegrees = 0;
    latitudeMinutes = 0.0;
    northOrSouthIndicator = '\0';
    longitude = "";
    longitudeDegrees = 0;
    longitudeMinutes = 0.0;
    eastOrWestIndicator = '\0';
    qualityIndicator = 0;
    numberOfSatellites = 0;
    horizontalDilutionPrecision = 0.0;
    altitude = 0.0;
    altitudeUnit = '\0';
    geoidalSeparation = 0.0;
    geoidalSeparationUnit = '\0';
    ageOfDGPSData = 0.0;
    dgpsStationID = -1; // Valid data range is from 0000 - 1023 for DGPS Station ID
    embeddedChecksum = 0;
    parsingSuccessful = false;
}

string NMEA_GPS_Parser::getGPSRawData()
{
    return(gpsRawData);
}

void NMEA_GPS_Parser::parseGPSRawData()
{
    int commaPostions[15];
    int numberOfCommasPerStandard = 15; // Also including the asterik in the total number of commas

    if (verifyNMEACompliance(gpsRawData) == true && findNumberOfCommas(gpsRawData, commaPostions) == numberOfCommasPerStandard)
    {
        embeddedChecksum = extractEmbeddedChecksum(gpsRawData);

        if (calculateChecksum(gpsRawData) == embeddedChecksum)
        {
            extractGPSData(gpsRawData, commaPostions);
            parsingSuccessful = true;
        }
        else
        {
            printf("\n> [ERROR] The NEMA-0183 GPS raw data failed integrity check!\n");
            parsingSuccessful = false;
        }
    }
    else
    {
        printf("\n> [ERROR] The raw data doesn't resemble the NEMA-0183 GPS format!\n");
        parsingSuccessful = false;
    }
}

void NMEA_GPS_Parser::parseGPSRawData(string rawData)
{
    gpsRawData = rawData;

    int commaPostions[15];
    int numberOfCommasPerStandard = 15; // Also including the asterik in the total number of commas

    if (verifyNMEACompliance(gpsRawData) == true && findNumberOfCommas(gpsRawData, commaPostions) == numberOfCommasPerStandard)
    {
        embeddedChecksum = extractEmbeddedChecksum(gpsRawData);

        if (calculateChecksum(gpsRawData) == embeddedChecksum)
        {
            extractGPSData(gpsRawData, commaPostions);
            parsingSuccessful = true;
        }
        else
        {
            printf("\n> [ERROR] The NEMA-0183 GPS raw data failed integrity check!\n");
            parsingSuccessful = false;
        }
    }
    else
    {
        printf("\n> [ERROR] The raw data doesn't resemble the NEMA-0183 GPS format!\n");
        parsingSuccessful = false;
    }
}

int NMEA_GPS_Parser::findNumberOfCommas(string rawData, int commaPositions[])
{
    int numberOfCommas = 0;
    int commaIndex = 0;
    int rawDataLength = rawData.length(); 

    for (int index = 0; index < rawDataLength; index++)
    {
        if (gpsRawData[index] == ',' || gpsRawData[index] == '*') // Only need the asterik's position to extract the last data field
        {
            numberOfCommas = numberOfCommas + 1;
            commaPositions[commaIndex] = index;
            commaIndex = commaIndex + 1; 
        }
    }

    return(numberOfCommas);
}

bool NMEA_GPS_Parser::verifyNMEACompliance(string rawData)
{
    int rawDataLength = rawData.length(); 

    if (rawDataLength <= 82 && gpsRawData[0] == '$' && gpsRawData.substr(1, 5) == "GPGGA" 
        && gpsRawData[rawDataLength - 5] == '*' && gpsRawData[rawDataLength - 2] == '\r' 
        && gpsRawData[rawDataLength - 1] == '\n')
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

int NMEA_GPS_Parser::extractEmbeddedChecksum(string rawData)
{
    int rawDataLength = rawData.length(); 
    string embeddedChecksumHex = string{rawData[rawDataLength - 4], rawData[rawDataLength - 3]};
    int embeddedChecksum = stoi(embeddedChecksumHex, 0, 16); // Converting from Hexadecimal number in string to its equivalent in integer

    return(embeddedChecksum);
}

int NMEA_GPS_Parser::calculateChecksum(string rawData)
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

void NMEA_GPS_Parser::extractGPSData(string rawData, int commaPositions[])
{
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
    string dgpsStationIDTemp = rawData.substr(commaPositions[13] + 1, commaPositions[14] - commaPositions[13] - 1);

    if (talkerIDTemp != "") {talkerID = talkerIDTemp;}
    if (messageIDTemp != "") {messageID = messageIDTemp;}

    if (utcTimeStampTemp != "")
    {
        utcTimeStamp = utcTimeStampTemp;
        utcTimeStampHours = stoi(utcTimeStamp.substr(0, 2));
        utcTimeStampMinutes = stoi(utcTimeStamp.substr(2, 2));
        utcTimeStampSeconds = stod(utcTimeStamp.substr(4));
    }

    if (latitudeTemp != "")
    {
        latitude = latitudeTemp;
        latitudeDegrees = stoi(latitude.substr(0, 2));
        latitudeMinutes = stod(latitude.substr(2));
    }

    if (northOrSouthIndicatorTemp != "" && (northOrSouthIndicatorTemp == "N" || northOrSouthIndicatorTemp == "S")) 
    {
        northOrSouthIndicator = northOrSouthIndicatorTemp[0];
    }

    if (longitudeTemp != "")
    {
        longitude = longitudeTemp;
        longitudeDegrees = stoi(longitude.substr(0, 3));
        longitudeMinutes = stod(longitude.substr(3));
    }

    if (eastOrWestIndicatorTemp != "" && (eastOrWestIndicatorTemp == "E" || eastOrWestIndicatorTemp == "W")) 
    {
        eastOrWestIndicator = eastOrWestIndicatorTemp[0];
    }

    if (qualityIndicatorTemp != "") {qualityIndicator = stoi(qualityIndicatorTemp);}
    if (numberOfSatellitesTemp != "") {numberOfSatellites = stoi(numberOfSatellitesTemp);}
    if (horizontalDilutionPrecisionTemp != "") {horizontalDilutionPrecision = stod(horizontalDilutionPrecisionTemp);}

    if (altitudeTemp != "") {altitude = stod(altitudeTemp);}
    if (altitudeUnitTemp != "") {altitudeUnit = altitudeUnitTemp[0];}

    if (geoidalSeparationTemp != "") {geoidalSeparation = stod(geoidalSeparationTemp);}
    if (geoidalSeparationUnitTemp != "") {geoidalSeparationUnit = geoidalSeparationUnitTemp[0];}

    if (ageOfDGPSDataTemp != "") {ageOfDGPSData = stod(ageOfDGPSDataTemp);}
    if (dgpsStationIDTemp != "") {dgpsStationID = stoi(dgpsStationIDTemp);}
}

bool NMEA_GPS_Parser::isParsingSuccessful()
{
    return(parsingSuccessful);
}

void NMEA_GPS_Parser::printParsedData()
{
    printf("\n> Talker ID: %s", talkerID.c_str());
    printf("\n> Message ID: %s", messageID.c_str());

    printf("\n> UTC TimeStamp: %s", utcTimeStamp.c_str());
    printf("\n> UTC TimeStamp Hours: %d", utcTimeStampHours);
    printf("\n> UTC TimeStamp Minutes: %d", utcTimeStampMinutes);
    printf("\n> UTC TimeStamp Seconds: %f", utcTimeStampSeconds);

    printf("\n> Latitude: %s", latitude.c_str());
    printf("\n> Latitude Degrees: %d", latitudeDegrees);
    printf("\n> Latitude Minutes: %f", latitudeMinutes);
    printf("\n> North/South Indicator: %c", northOrSouthIndicator);

    printf("\n> Longitude: %s", longitude.c_str());
    printf("\n> Longitude Degrees: %d", longitudeDegrees);
    printf("\n> Longitude Minutes: %f", longitudeMinutes);
    printf("\n> East/West Indicator: %c", eastOrWestIndicator);

    printf("\n> Quality Indicator: %d", qualityIndicator);
    printf("\n> Number Of Satellites: %d", numberOfSatellites);
    printf("\n> Horizontal Dilution Precision: %f", horizontalDilutionPrecision);

    printf("\n> Altitude: %f", altitude);
    printf("\n> Altitude Unit: %c", altitudeUnit);

    printf("\n> Geoidal Separation: %f", geoidalSeparation);
    printf("\n> Geoidal Separation Unit: %c", geoidalSeparationUnit);

    printf("\n> Age Of DGPS Data: %f", ageOfDGPSData);
    printf("\n> DGPS Station ID: %d\n", dgpsStationID);
}