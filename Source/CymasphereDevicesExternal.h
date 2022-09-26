/*
  ==============================================================================

    CymasphereDevicesExternal.h
    Created: 25 Sep 2022 5:30:29pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#pragma once
#include "CymasphereDevices.h"

#if JUCE_MSVC
#define UNITY_INTERFACE_API __stdcall
#define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#else
 #define UNITY_INTERFACE_API
 #define UNITY_INTERFACE_EXPORT __attribute__ ((visibility("default")))
#endif

// Expose External
extern "C" UNITY_INTERFACE_EXPORT DeviceManagement* UNITY_INTERFACE_API DeviceManagementCreate();
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementDelete(const DeviceManagement* management);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementRegisterCallback(
    DeviceManagement* management, ExternalMidiInputCallback callback);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementRegisterLogger(
    DeviceManagement* management, ExternalLogger logger);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementRefresh(
    DeviceManagement* management);

extern "C" UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API DeviceManagementInputDeviceCount(
    const DeviceManagement* management);
extern "C" UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API DeviceManagementOutputDeviceCount(
    const DeviceManagement* management);

extern "C" UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API DeviceManagementInputDeviceIsEnabled(
    const DeviceManagement* management, int id);
extern "C" UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API DeviceManagementOutputDeviceIsEnabled(
    const DeviceManagement* management, int id);
extern "C" UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API DeviceManagementSetInputDeviceEnabled(
    DeviceManagement* management, int id, bool enabled);
extern "C" UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API DeviceManagementSetOutputDeviceEnabled(
    DeviceManagement* management, int id, bool enabled);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementGetInputDeviceName(
    const DeviceManagement* management, int id, char* str, int strlen);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementGetInputDeviceIdentifier(
    const DeviceManagement* management, int id, char* str, int strlen);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementGetOutputDeviceName(
    const DeviceManagement* management, int id, char* str, int strlen);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementGetOutputDeviceIdentifier(
    const DeviceManagement* management, int id, char* str, int strlen);


extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementSendMessage3(
    const DeviceManagement* management, juce::uint8 byte1, juce::uint8 byte2, juce::uint8 byte3, int id);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementSendMessage2(
    const DeviceManagement* management, juce::uint8 byte1, juce::uint8 byte2, int id);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementSendMessage1(
    const DeviceManagement* management, juce::uint8 byte1, int id);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementSendSysEx(
    const DeviceManagement* management, const void* data, int dataSize, int id = -1);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementNoteOn(
    const DeviceManagement* management, int channel, int midi, float velocity, int id);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementNoteOff(
    const DeviceManagement* management, int channel, int midi, int id);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementAllNotesOff(
    const DeviceManagement* management, int channel, int id);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementAllSoundOff(
    const DeviceManagement* management, int channel, int id);
