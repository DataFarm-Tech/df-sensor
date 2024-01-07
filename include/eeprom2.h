#ifndef EEPROM_H
#define EEPROM_H

#include <EEPROM.h>
#include <Arduino.h>

class eeprom {
public:
    typedef struct _tsEEPROMContent
    {
        char nodeId[33];
    } tsEEPROMContent;
    eeprom();
    static const int EEPROM_SIZE = 33;

    static tsEEPROMContent eepromData;

    static void setNodeId(const String & nodeId);
    static bool isNodeId();
    static std::string getNodeId();
    static void clearEEPROM();
    static void saveEEPROM();
    static void restoreEEPROM();

private:
};

#endif  // EEPROM_H
