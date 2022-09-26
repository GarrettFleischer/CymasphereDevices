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
    // sendToOutputs(message);
    auto index = 0;
    for (; index < inputDevices->size(); ++index)
    {
        if (inputDevices->get(index)->inDevice.get() == source)
        {
            break;
        }
    }

    const juce::ScopedLock s1(midiMonitorLock);
    incomingMessages.add(MidiInputData{index, message});
    triggerAsyncUpdate();
}

void DeviceManagement::handleAsyncUpdate()
{
    juce::Array<MidiInputData> messages;
    {
        const juce::ScopedLock s1(midiMonitorLock);
        messages.swapWith(incomingMessages);
    }

    for (auto& data : messages)
    {
        for (const auto callback : callbacks)
        {
            const auto raw = data.message.getRawData();
            callback(data.device, raw[0], raw[1], raw[2]);
        }
        
        DBG(data.message.getDescription());
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
