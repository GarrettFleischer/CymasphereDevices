/*
  ==============================================================================

    CymasphereDevices.cpp
    Created: 25 Sep 2022 1:47:29pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#include "CymasphereDevices.h"

DeviceManagement::DeviceManagement()
{
    inputDevices = new MidiDeviceList(true, this);
    outputDevices = new MidiDeviceList(false, this);
}

DeviceManagement::~DeviceManagement()
{
    delete inputDevices;
    delete outputDevices;
    callbacks.clear();
}

void DeviceManagement::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    auto index = 0;
    for (; index < inputDevices->size(); ++index)
    {
        if (inputDevices->get(index)->inDevice.get() == source)
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

void DeviceManagement::logExternal(juce::String message)
{
    auto len = message.length() + 1;
    auto logMessage = new char[len];
    message.copyToUTF8(logMessage, len);
    for (auto logger : loggers)
    {
        logger(logMessage, len);
    }
    delete[] logMessage;
}

void DeviceManagement::sendMidiMessage(int id, juce::MidiMessage& message)
{
    if (id > -1)
    {
        if (outputDeviceIsEnabled(id))
        {
            outputDevices->get(id)->outDevice->sendMessageNow(message);
        }
    }
    else
    {
        for (int i = 0; i < outputDevices->size(); ++i)
        {
            if (outputDeviceIsEnabled(i))
            {
                outputDevices->get(i)->outDevice->sendMessageNow(message);
            }
        }
    }
}

// void DeviceManagement::sendToOutputs(const juce::MidiMessage& msg)
// {
//     for (const auto midiOutput : *outputDevices->getDevices())
//         if (midiOutput->outDevice.get() != nullptr)
//             midiOutput->outDevice->sendMessageNow(msg);
// }

void DeviceManagement::registerCallback(ExternalMidiInputCallback callback)
{
    callbacks.add(callback);
}

void DeviceManagement::registerLogger(ExternalLogger log)
{
    loggers.add(log);
}

void DeviceManagement::refresh()
{
    inputDevices->refresh();
    outputDevices->refresh();
}

int DeviceManagement::inputDeviceCount()
{
    return inputDevices->size();
}

int DeviceManagement::outputDeviceCount()
{
    return outputDevices->size();
}

bool DeviceManagement::inputDeviceIsEnabled(int id)
{
    return inputDevices->isEnabled(id);
}

bool DeviceManagement::outputDeviceIsEnabled(int id)
{
    return outputDevices->isEnabled(id);
}

bool DeviceManagement::setInputDeviceEnabled(int id, bool enabled)
{
    inputDevices->setActive(id, enabled);
    return inputDevices->isEnabled(id) == enabled;
}

bool DeviceManagement::setOutputDeviceEnabled(int id, bool enabled)
{
    outputDevices->setActive(id, enabled);
    return outputDevices->isEnabled(id) == enabled;
}

void DeviceManagement::getInputDeviceName(int id, char* str, int strlen)
{
    inputDevices->get(id)->deviceInfo.name.copyToUTF8(str, strlen);
}

void DeviceManagement::getInputDeviceIdentifier(int id, char* str, int strlen)
{
    inputDevices->get(id)->deviceInfo.identifier.copyToUTF8(str, strlen);
}

void DeviceManagement::getOutputDeviceName(int id, char* str, int strlen)
{
    outputDevices->get(id)->deviceInfo.name.copyToUTF8(str, strlen);
}

void DeviceManagement::getOutputDeviceIdentifier(int id, char* str, int strlen)
{
    outputDevices->get(id)->deviceInfo.identifier.copyToUTF8(str, strlen);
}

void DeviceManagement::noteOn(int channel, int midi, float velocity, int id)
{
    auto message = juce::MidiMessage::noteOn(channel, midi, velocity);
    sendMidiMessage(id, message);
}

void DeviceManagement::noteOff(int channel, int midi, int id)
{
    auto message = juce::MidiMessage::noteOff(channel, midi);
    sendMidiMessage(id, message);
}
