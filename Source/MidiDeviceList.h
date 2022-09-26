/*
  ==============================================================================

    MidiDeviceList.h
    Created: 25 Sep 2022 3:53:10pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


struct MidiDeviceListEntry : juce::ReferenceCountedObject
{
    MidiDeviceListEntry(juce::MidiDeviceInfo info) : deviceInfo(std::move(info))
    {
    }

    juce::MidiDeviceInfo deviceInfo;
    std::unique_ptr<juce::MidiInput> inDevice;
    std::unique_ptr<juce::MidiOutput> outDevice;

    using Ptr = juce::ReferenceCountedObjectPtr<MidiDeviceListEntry>;
};

class MidiDeviceList
{
public:
    MidiDeviceList(bool input, juce::MidiInputCallback* callback);
    ~MidiDeviceList();

    void refresh();
    int size() const;

    MidiDeviceListEntry::Ptr get(int index) const;

    bool isEnabled(int index) const;
    void setActive(int index, bool active);

    void iterate(void visit(MidiDeviceListEntry* entry));

    juce::ReferenceCountedArray<MidiDeviceListEntry>* getDevices();

private:
    bool isInput;
    juce::ReferenceCountedArray<MidiDeviceListEntry> devices;
    juce::MidiInputCallback* callback;

    void updateDeviceList();
    bool hasDeviceListChanged(const juce::Array<juce::MidiDeviceInfo>& availableDevices) const;
    void closeUnpluggedDevices(const juce::Array<juce::MidiDeviceInfo>& currentlyPluggedInDevices);
    void closeDevice(int index);
    bool openDevice(int index);
    juce::ReferenceCountedObjectPtr<MidiDeviceListEntry> findDevice(const juce::MidiDeviceInfo& device) const;
};
