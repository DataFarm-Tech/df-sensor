#include "eeprom2.h"

eeprom::tsEEPROMContent eeprom::eepromData;

eeprom::eeprom()
{
    EEPROM.begin(sizeof(tsEEPROMContent));
}

void eeprom::setNodeId(const String & nodeId)
{
    nodeId.toCharArray(eepromData.nodeId, sizeof(eepromData.nodeId));
}

bool eeprom::isNodeId()
{
    std::string nodeId = eeprom::getNodeId();
    if (nodeId.empty())
    {
        return false;
    }
    return true;
}

std::string eeprom::getNodeId()
{
    std::string nodeId = std::string(eepromData.nodeId);
    return nodeId;
}

void eeprom::clearEEPROM() 
{
    
    for (int i = 0; i < sizeof(tsEEPROMContent); i++) 
    {
        EEPROM.write(i, 0xFF);
    }
    EEPROM.commit();
}

void eeprom::restoreEEPROM()
{
    EEPROM.readBytes(0, (void *)&eepromData, sizeof(tsEEPROMContent));
}

void eeprom::saveEEPROM()
{
    EEPROM.writeBytes(0, (void *)&eepromData, sizeof(tsEEPROMContent));
    EEPROM.commit();
}
