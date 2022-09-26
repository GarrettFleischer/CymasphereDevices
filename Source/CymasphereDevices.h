/*
  ==============================================================================

    CymasphereDevices.h
    Created: 25 Sep 2022 1:47:29pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "MidiDeviceList.h"



using ExternalMidiInputCallback = void(*)(int id, juce::uint8, juce::uint8, juce::uint8);

struct MidiInputData
{
    int device;
    juce::MidiMessage message;
};

class DeviceManagement final : juce::MidiInputCallback, juce::AsyncUpdater
{
public:
    DeviceManagement();
    ~DeviceManagement() override;

    void registerCallback(ExternalMidiInputCallback callback);

    void refresh();

    int inputDeviceCount();
    int outputDeviceCount();

    bool inputDeviceIsEnabled(int id);
    bool outputDeviceIsEnabled(int id);

    bool setInputDeviceEnabled(int id, bool enabled);
    bool setOutputDeviceEnabled(int id, bool enabled);

    void getInputDeviceName(int id, char* str, int strlen);
    void getInputDeviceIdentifier(int id, char* str, int strlen);

    void getOutputDeviceName(int id, char* str, int strlen);
    void getOutputDeviceIdentifier(int id, char* str, int strlen);

private:
    MidiDeviceList* inputDevices;
    MidiDeviceList* outputDevices;
    juce::Array<ExternalMidiInputCallback> callbacks;

    juce::CriticalSection midiMonitorLock;
    juce::Array<MidiInputData> incomingMessages;

    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    void handleAsyncUpdate() override;
    // void sendToOutputs (const juce::MidiMessage& msg);
};

