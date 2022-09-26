/*
  ==============================================================================

    CymasphereDevicesExternal.cpp
    Created: 25 Sep 2022 5:30:29pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#include "CymasphereDevicesExternal.h"


DeviceManagement* DeviceManagementCreate()
{
    return new DeviceManagement();
}

void DeviceManagementDelete(DeviceManagement* management)
{
    delete management;
}

void DeviceManagementRegisterCallback(DeviceManagement* management, ExternalMidiInputCallback callback)
{
    management->registerCallback(callback);
}

void DeviceManagementRegisterLogger(DeviceManagement* management, ExternalLogger logger)
{
    management->registerLogger(logger);
}

void DeviceManagementRefresh(DeviceManagement* management)
{
    management->refresh();
}

int DeviceManagementInputDeviceCount(DeviceManagement* management)
{
    return management->inputDeviceCount();
}

int DeviceManagementOutputDeviceCount(DeviceManagement* management)
{
    return management->outputDeviceCount();
}

bool DeviceManagementInputDeviceIsEnabled(DeviceManagement* management, int id)
{
    return management->inputDeviceIsEnabled(id);
}

bool DeviceManagementOutputDeviceIsEnabled(DeviceManagement* management, int id)
{
    return management->outputDeviceIsEnabled(id);
}

bool DeviceManagementSetInputDeviceEnabled(DeviceManagement* management, int id, bool enabled)
{
    return management->setInputDeviceEnabled(id, enabled);
}

bool DeviceManagementSetOutputDeviceEnabled(DeviceManagement* management, int id, bool enabled)
{
    return management->setOutputDeviceEnabled(id, enabled);
}

void DeviceManagementGetInputDeviceName(DeviceManagement* management, int id, char* str, int strlen)
{
    management->getInputDeviceName(id, str, strlen);
}

void DeviceManagementGetInputDeviceIdentifier(DeviceManagement* management, int id, char* str, int strlen)
{
    management->getInputDeviceIdentifier(id, str, strlen);
}

void DeviceManagementGetOutputDeviceName(DeviceManagement* management, int id, char* str, int strlen)
{
    management->getOutputDeviceName(id, str, strlen);
}

void DeviceManagementGetOutputDeviceIdentifier(DeviceManagement* management, int id, char* str, int strlen)
{
    management->getOutputDeviceIdentifier(id, str, strlen);
}

void DeviceManagementNoteOn(DeviceManagement* management, int channel, int midi, float velocity, int id)
{
    management->noteOn(channel, midi, velocity, id);
}

void DeviceManagementNoteOff(DeviceManagement* management, int channel, int midi, int id)
{
    management->noteOff(channel, midi, id);
}
