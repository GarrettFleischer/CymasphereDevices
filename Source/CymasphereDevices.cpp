/*
  ==============================================================================

    CymasphereDevices.cpp
    Created: 25 Sep 2022 1:47:29pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#include "CymasphereDevices.h"

DeviceManagement::DeviceManagement()
    : inputDevices(true, this),
      outputDevices(false, this)
{
}

DeviceManagement::~DeviceManagement()
{
    callbacks.clear();
    loggers.clear();
}

void DeviceManagement::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    auto index = 0;
    for (; index < inputDevices.size(); ++index)
    {
        if (inputDevices.get(index)->inDevice.get() == source)
        {
            break;
        }
    }

    // logExternal(message.getDescription());

    for (const auto callback : callbacks)
    {
        const auto raw = message.getRawData();
        callback(index, raw[0], raw[1], raw[2]);
    }
}

void DeviceManagement::logExternal(const juce::String& message)
{
    const auto len = message.length() + 1;
    const auto logMessage = new char[len];
    message.copyToUTF8(logMessage, len);
    for (const auto logger : loggers)
    {
        logger(logMessage, len);
    }
    delete[] logMessage;
}

void DeviceManagement::sendMidiMessage(const int id, const juce::MidiMessage& message) const
{
    if (id > -1)
    {
        if (outputDeviceIsEnabled(id))
        {
            outputDevices.get(id)->outDevice->sendMessageNow(message);
        }
    }
    else
    {
        for (int i = 0; i < outputDevices.size(); ++i)
        {
            if (outputDeviceIsEnabled(i))
            {
                outputDevices.get(i)->outDevice->sendMessageNow(message);
            }
        }
    }
}

void DeviceManagement::registerCallback(const ExternalMidiInputCallback callback)
{
    callbacks.add(callback);
}

void DeviceManagement::registerLogger(const ExternalLogger log)
{
    loggers.add(log);
}

void DeviceManagement::refresh()
{
    inputDevices.refresh();
    outputDevices.refresh();
}

int DeviceManagement::inputDeviceCount() const
{
    return inputDevices.size();
}

int DeviceManagement::outputDeviceCount() const
{
    return outputDevices.size();
}

bool DeviceManagement::inputDeviceIsEnabled(const int id) const
{
    return inputDevices.isEnabled(id);
}

bool DeviceManagement::outputDeviceIsEnabled(const int id) const
{
    return outputDevices.isEnabled(id);
}

bool DeviceManagement::setInputDeviceEnabled(const int id, const bool enabled)
{
    inputDevices.setActive(id, enabled);
    return inputDevices.isEnabled(id) == enabled;
}

bool DeviceManagement::setOutputDeviceEnabled(const int id, const bool enabled)
{
    outputDevices.setActive(id, enabled);
    return outputDevices.isEnabled(id) == enabled;
}

void DeviceManagement::getInputDeviceName(const int id, char* str, const int strlen) const
{
    inputDevices.get(id)->deviceInfo.name.copyToUTF8(str, strlen);
}

void DeviceManagement::getInputDeviceIdentifier(const int id, char* str, const int strlen) const
{
    inputDevices.get(id)->deviceInfo.identifier.copyToUTF8(str, strlen);
}

void DeviceManagement::getOutputDeviceName(const int id, char* str, const int strlen) const
{
    outputDevices.get(id)->deviceInfo.name.copyToUTF8(str, strlen);
}

void DeviceManagement::getOutputDeviceIdentifier(const int id, char* str, const int strlen) const
{
    outputDevices.get(id)->deviceInfo.identifier.copyToUTF8(str, strlen);
}

void DeviceManagement::sendMessage(const juce::uint8 byte1, const juce::uint8 byte2, const juce::uint8 byte3, const int id) const
{
    const auto message = juce::MidiMessage(byte1, byte2, byte3);
    sendMidiMessage(id, message);
}

void DeviceManagement::sendMessage(const juce::uint8 byte1, const juce::uint8 byte2, const int id) const
{
    const auto message = juce::MidiMessage(byte1, byte2);
    sendMidiMessage(id, message);
}

void DeviceManagement::sendMessage(const juce::uint8 byte1, const int id) const
{
    const auto message = juce::MidiMessage(byte1);
    sendMidiMessage(id, message);
}

void DeviceManagement::sendSysEx(const void* data, const int dataSize, const int id) const
{
    const auto message = juce::MidiMessage::createSysExMessage(data, dataSize);
    sendMidiMessage(id, message);
}

void DeviceManagement::sendNoteOn(const int channel, const int midi, const float velocity, const int id) const
{
    const auto message = juce::MidiMessage::noteOn(channel, midi, velocity);
    sendMidiMessage(id, message);
}

void DeviceManagement::sendNoteOff(const int channel, const int midi, const int id) const
{
    const auto message = juce::MidiMessage::noteOff(channel, midi);
    sendMidiMessage(id, message);
}

void DeviceManagement::sendAllNotesOff(const int channel, const int id) const
{
    const auto message = juce::MidiMessage::allNotesOff(channel);
    sendMidiMessage(id, message);
}

void DeviceManagement::sendAllSoundOff(const int channel, const int id) const
{
    const auto message = juce::MidiMessage::allSoundOff(channel);
    sendMidiMessage(id, message);
}
