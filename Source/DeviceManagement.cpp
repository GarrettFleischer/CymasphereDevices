/*
  ==============================================================================

    DeviceManagement.cpp
    Created: 25 Sep 2022 1:47:29pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#include "DeviceManagement.h"

DeviceManagement::DeviceManagement()
    : devices(this)
{
    startTimer(5);
}

DeviceManagement::~DeviceManagement()
{
    callbacks.clear();
    loggers.clear();
}

void DeviceManagement::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    juce::ScopedLock s1(midiEventLock);
    events.add(MidiInputEvent{source, message});
}

void DeviceManagement::hiResTimerCallback()
{
    juce::Array<MidiInputEvent> lEvents;
    {
        juce::ScopedLock s1(midiEventLock);
        events.swapWith(lEvents);
    }

    for (auto [source, message] : lEvents)
    {
        auto index = 0;
        for (; index < devices.size(); ++index)
        {
            if (devices.get(index)->inDevice.get() == source)
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
            devices.get(id)->outDevice->sendMessageNow(message);
        }
    }
    else
    {
        for (int i = 0; i < devices.size(); ++i)
        {
            if (outputDeviceIsEnabled(i))
            {
                devices.get(i)->outDevice->sendMessageNow(message);
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
    devices.refresh();
}

int DeviceManagement::deviceCount() const
{
    return devices.size();
}

bool DeviceManagement::inputDeviceIsEnabled(const int id) const
{
    return devices.isEnabled(true, id);
}

bool DeviceManagement::outputDeviceIsEnabled(const int id) const
{
    return devices.isEnabled(false, id);
}

bool DeviceManagement::setInputDeviceEnabled(const int id, const bool enabled)
{
    devices.setActive(true, id, enabled);
    return devices.isEnabled(true, id) == enabled;
}

bool DeviceManagement::setOutputDeviceEnabled(const int id, const bool enabled)
{
    devices.setActive(false, id, enabled);
    return devices.isEnabled(false, id) == enabled;
}

bool DeviceManagement::deviceHasInput(const int id) const
{
    return devices.get(id)->hasInput;
}

bool DeviceManagement::deviceHasOutput(const int id) const
{
    return devices.get(id)->hasOutput;
}

void DeviceManagement::getDeviceName(const int id, char* str, const int strlen) const
{
    devices.get(id)->deviceInfo.name.copyToUTF8(str, strlen);
}

void DeviceManagement::getDeviceIdentifier(const int id, char* str, const int strlen) const
{
    devices.get(id)->deviceInfo.identifier.copyToUTF8(str, strlen);
}

void DeviceManagement::sendMessage(const juce::uint8 byte1, const juce::uint8 byte2, const juce::uint8 byte3,
                                   const int id) const
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
