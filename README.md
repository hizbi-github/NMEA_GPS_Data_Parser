# NMEA GPS Data Parser

## Cowlar Design Studio Assessment (WIP)

### TODO: 

#### ✅ Functional Implementation
#### ✅ OOP Implementation
#### ✅ Basic Testing 
#### ⭕ Update Readme
#### ⭕ Add Unit Tests

<br>

### Example:

TODO: Brief explanation with pseudo code...

<br>

### Example:

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

<br>

### Member Methods: 

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

<br>

### Building and Running: 

<p align="center">
    Building for NodeMCU-32S from PlatformIO (ESP-IDF v5.0.2):
<p/>

<div align="center">
    <img src="./docs/screenshots/oop_test_build_nodemcu32s.png" style="width: 50%">
<div/>

<br>

<p align="center">
    Parsing GPS data (printing over serial interface):
<p/>

<div align="center">
    <img src="./docs/screenshots/oop_test_run_over_serial.png" style="width: 50%">
<div/>

<br>

<p align="center">
    Detailed output when using the functional version:
<p/>

<div align="center">
    <img src="./docs/screenshots/functional_test_run_over_serial.png" style="width: 50%">
<div/>

<br>

