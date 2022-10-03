/*
  ==============================================================================

    Juce4Unity_External.cpp
    Created: 25 Sep 2022 5:30:29pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#include "External.h"


DeviceManagement* DeviceManagementCreate()
{
    return new DeviceManagement();
}

void DeviceManagementDelete(const DeviceManagement* management)
{
    delete management;
}

void DeviceManagementRegisterCallback(DeviceManagement* management, const ExternalMidiInputCallback callback)
{
    management->registerCallback(callback);
}

void DeviceManagementRegisterLogger(DeviceManagement* management, const ExternalLogger logger)
{
    management->registerLogger(logger);
}

void DeviceManagementRefresh(DeviceManagement* management)
{
    management->refresh();
}

int DeviceManagementDeviceCount(const DeviceManagement* management)
{
    return management->deviceCount();
}

bool DeviceManagementInputDeviceIsEnabled(const DeviceManagement* management, const int id)
{
    return management->inputDeviceIsEnabled(id);
}

bool DeviceManagementOutputDeviceIsEnabled(const DeviceManagement* management, const int id)
{
    return management->outputDeviceIsEnabled(id);
}

bool DeviceManagementSetInputDeviceEnabled(DeviceManagement* management, const int id, const bool enabled)
{
    return management->setInputDeviceEnabled(id, enabled);
}

bool DeviceManagementSetOutputDeviceEnabled(DeviceManagement* management, const int id, const bool enabled)
{
    return management->setOutputDeviceEnabled(id, enabled);
}

bool DeviceManagementDeviceHasInput(const DeviceManagement* management, int id)
{
    return management->deviceHasInput(id);
}

bool DeviceManagementDeviceHasOutput(const DeviceManagement* management, int id)
{
    return management->deviceHasOutput(id);
}

void DeviceManagementGetDeviceName(const DeviceManagement* management, int id, char* str, int strlen)
{
    management->getDeviceName(id, str, strlen);
}

void DeviceManagementGetDeviceIdentifier(const DeviceManagement* management, const int id, char* str,
                                         const int strlen)
{
    management->getDeviceIdentifier(id, str, strlen);
}

void DeviceManagementSendMessage3(const DeviceManagement* management, const juce::uint8 byte1, const juce::uint8 byte2,
                                  const juce::uint8 byte3,
                                  const int id)
{
    management->sendMessage(byte1, byte2, byte3, id);
}

void DeviceManagementSendMessage2(const DeviceManagement* management, const juce::uint8 byte1, const juce::uint8 byte2,
                                  const int id)
{
    management->sendMessage(byte1, byte2, id);
}

void DeviceManagementSendMessage1(const DeviceManagement* management, const juce::uint8 byte1, const int id)
{
    management->sendMessage(byte1, id);
}

void DeviceManagementSendSysEx(const DeviceManagement* management, const void* data, const int dataSize, const int id)
{
    management->sendSysEx(data, dataSize, id);
}

void DeviceManagementNoteOn(const DeviceManagement* management, const int channel, const int midi, const float velocity,
                            const int id)
{
    management->sendNoteOn(channel, midi, velocity, id);
}

void DeviceManagementNoteOff(const DeviceManagement* management, const int channel, const int midi, const int id)
{
    management->sendNoteOff(channel, midi, id);
}

void DeviceManagementAllNotesOff(const DeviceManagement* management, const int channel, const int id)
{
    management->sendAllNotesOff(channel, id);
}

void DeviceManagementAllSoundOff(const DeviceManagement* management, const int channel, const int id)
{
    management->sendAllSoundOff(channel, id);
}
