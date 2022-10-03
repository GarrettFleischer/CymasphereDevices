/*
  ==============================================================================

    MidiDeviceList.cpp
    Created: 25 Sep 2022 3:53:10pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#include "MidiDeviceList.h"

MidiDeviceList::MidiDeviceList(juce::MidiInputCallback* callback)
{
    this->callback = callback;
}

void MidiDeviceList::refresh()
{
    updateDeviceList();
}

int MidiDeviceList::size() const
{
    return devices.size();
}

MidiDeviceListEntry::Ptr MidiDeviceList::get(int index) const
{
    return devices[index];
}

bool MidiDeviceList::isEnabled(bool input, int index) const
{
    return input
               ? devices[index]->inDevice != nullptr
               : devices[index]->outDevice != nullptr;
}

void MidiDeviceList::setActive(bool input, int index, bool active)
{
    if (active)
    {
        openDevice(input, index);
    }
    else
    {
        closeDevice(input, index);
    }
}

void MidiDeviceList::iterate(void visit(MidiDeviceListEntry* entry))
{
    for (const auto device : devices)
    {
        visit(device);
    }
}

juce::ReferenceCountedArray<MidiDeviceListEntry>* MidiDeviceList::getDevices()
{
    return &devices;
}

void MidiDeviceList::updateDeviceList()
{
    const auto inputDevices = juce::MidiInput::getAvailableDevices();
    const auto outputDevices = juce::MidiOutput::getAvailableDevices();
    auto availableDevices = inputDevices;
    for (const auto& outputDevice : outputDevices)
    {
        if (!availableDevices.contains(outputDevice))
        {
            availableDevices.add(outputDevice);
        }
    }

    if (hasDeviceListChanged(availableDevices))
    {
        closeUnpluggedDevices(availableDevices);

        juce::ReferenceCountedArray<MidiDeviceListEntry> newDeviceList;

        // add all currently plugged-in devices to the device list
        for (const auto& newDevice : availableDevices)
        {
            MidiDeviceListEntry::Ptr entry = findDevice(newDevice);

            if (entry == nullptr)
                entry = new MidiDeviceListEntry(newDevice);

            entry->hasInput = inputDevices.contains(newDevice);
            entry->hasOutput = outputDevices.contains(newDevice);

            newDeviceList.add(entry);
        }

        devices = newDeviceList;
    }
}

bool MidiDeviceList::hasDeviceListChanged(const juce::Array<juce::MidiDeviceInfo>& availableDevices) const
{
    if (availableDevices.size() != devices.size())
        return true;

    for (auto i = 0; i < availableDevices.size(); ++i)
        if (availableDevices[i] != devices[i]->deviceInfo)
            return true;

    return false;
}

void MidiDeviceList::closeUnpluggedDevices(const juce::Array<juce::MidiDeviceInfo>& currentlyPluggedInDevices)
{
    for (auto i = devices.size(); --i >= 0;)
    {
        auto& d = *devices[i];

        if (! currentlyPluggedInDevices.contains(d.deviceInfo))
        {
            closeDevice(true, i);
            closeDevice(false, i);
            devices.remove(i);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MidiDeviceList::closeDevice(const bool input, const int index)
{
    if (input)
    {
        if (devices[index]->inDevice == nullptr)
        {
            return;
        }

        devices[index]->inDevice->stop();
        devices[index]->inDevice.reset();
    }
    else
    {
        if (devices[index]->outDevice == nullptr)
        {
            return;
        }

        devices[index]->outDevice.reset();
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool MidiDeviceList::openDevice(const bool input, const int index)
{
    if (input)
    {
        if (devices[index]->inDevice != nullptr)
        {
            return true;
        }

        if (devices[index]->outDevice != nullptr)
        {
            closeDevice(false, index);
        }

        devices[index]->inDevice = juce::MidiInput::openDevice(devices[index]->deviceInfo.identifier, callback);

        if (devices[index]->inDevice == nullptr)
        {
            DBG("MidiDemo::openDevice: open input device for index = " << index << " failed!");
            return false;
        }

        devices[index]->inDevice->start();
        return true;
    }

    if (devices[index]->outDevice != nullptr)
    {
        return true;
    }

    if (devices[index]->inDevice != nullptr)
    {
        closeDevice(true, index);
    }

    devices[index]->outDevice = juce::MidiOutput::openDevice(devices[index]->deviceInfo.identifier);

    if (devices[index]->outDevice == nullptr)
    {
        DBG("MidiDemo::openDevice: open output device for index = " << index << " failed!");
        return false;
    }

    return true;
}


juce::ReferenceCountedObjectPtr<MidiDeviceListEntry> MidiDeviceList::findDevice(
    const juce::MidiDeviceInfo& device) const
{
    for (auto& d : devices)
        if (d->deviceInfo == device)
            return d;

    return nullptr;
}
