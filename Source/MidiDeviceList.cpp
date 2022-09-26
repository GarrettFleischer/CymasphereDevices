/*
  ==============================================================================

    MidiDeviceList.cpp
    Created: 25 Sep 2022 3:53:10pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#include "MidiDeviceList.h"

MidiDeviceList::MidiDeviceList(bool input, juce::MidiInputCallback* callback)
{
    isInput = input;
    this->callback = callback;
}

MidiDeviceList::~MidiDeviceList() = default;

void MidiDeviceList::refresh()
{
    updateDeviceList();
}

int MidiDeviceList::size()
{
    return devices.size();
}

MidiDeviceListEntry::Ptr MidiDeviceList::get(int index)
{
    return devices[index];
}

bool MidiDeviceList::isEnabled(int index)
{
    return isInput
               ? devices[index]->inDevice.get() != nullptr
               : devices[index]->outDevice.get() != nullptr;
}

void MidiDeviceList::setActive(int index, bool active)
{
    if (active)
    {
        openDevice(index);
    }
    else
    {
        closeDevice(index);
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
    auto availableDevices = isInput
                                ? juce::MidiInput::getAvailableDevices()
                                : juce::MidiOutput::getAvailableDevices();

    if (hasDeviceListChanged(availableDevices))
    {
        closeUnpluggedDevices(availableDevices);

        juce::ReferenceCountedArray<MidiDeviceListEntry> newDeviceList;

        // add all currently plugged-in devices to the device list
        for (auto& newDevice : availableDevices)
        {
            MidiDeviceListEntry::Ptr entry = findDevice(newDevice);

            if (entry == nullptr)
                entry = new MidiDeviceListEntry(newDevice);

            newDeviceList.add(entry);
        }

        devices = newDeviceList;
    }
}

bool MidiDeviceList::hasDeviceListChanged(const juce::Array<juce::MidiDeviceInfo>& availableDevices)
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
            closeDevice(i);
            devices.remove(i);
        }
    }
}

void MidiDeviceList::closeDevice(int index)
{
    if (isInput)
    {
        if(devices[index]->inDevice.get() == nullptr)
        {
            return;
        }
        
        devices[index]->inDevice->stop();
        devices[index]->inDevice.reset();
    }
    else
    {
        if(devices[index]->outDevice.get() == nullptr)
        {
            return;
        }
        
        devices[index]->outDevice.reset();
    }
}

bool MidiDeviceList::openDevice(int index)
{
    if (isInput)
    {
        if (devices[index]->inDevice.get() != nullptr)
        {
            return true;
        }

        devices[index]->inDevice = juce::MidiInput::openDevice(devices[index]->deviceInfo.identifier, callback);

        if (devices[index]->inDevice.get() == nullptr)
        {
            DBG("MidiDemo::openDevice: open input device for index = " << index << " failed!");
            return false;
        }

        devices[index]->inDevice->start();
        return true;
    }
    if (devices[index]->outDevice.get() != nullptr)
    {
        return true;
    }

    devices[index]->outDevice = juce::MidiOutput::openDevice(devices[index]->deviceInfo.identifier);

    if (devices[index]->outDevice.get() == nullptr)
    {
        DBG("MidiDemo::openDevice: open output device for index = " << index << " failed!");
        return false;
    }

    return true;
}


juce::ReferenceCountedObjectPtr<MidiDeviceListEntry> MidiDeviceList::findDevice(
    juce::MidiDeviceInfo device) const
{
    for (auto& d : devices)
        if (d->deviceInfo == device)
            return d;

    return nullptr;
}
