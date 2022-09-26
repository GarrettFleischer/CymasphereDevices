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
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementDelete(DeviceManagement* management);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementRegisterCallback(
    DeviceManagement* management, ExternalMidiInputCallback callback);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementRefresh(
    DeviceManagement* management);

extern "C" UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API DeviceManagementInputDeviceCount(
    DeviceManagement* management);
extern "C" UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API DeviceManagementOutputDeviceCount(
    DeviceManagement* management);

extern "C" UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API DeviceManagementInputDeviceIsEnabled(
    DeviceManagement* management, int id);
extern "C" UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API DeviceManagementOutputDeviceIsEnabled(
    DeviceManagement* management, int id);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementSetInputDeviceEnabled(
    DeviceManagement* management, int id, bool enabled);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementSetOutputDeviceEnabled(
    DeviceManagement* management, int id, bool enabled);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementGetInputDeviceName(
    DeviceManagement* management, int id, char* str, int strlen);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementGetInputDeviceIdentifier(
    DeviceManagement* management, int id, char* str, int strlen);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementGetOutputDeviceName(
    DeviceManagement* management, int id, char* str, int strlen);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API DeviceManagementGetOutputDeviceIdentifier(
    DeviceManagement* management, int id, char* str, int strlen);