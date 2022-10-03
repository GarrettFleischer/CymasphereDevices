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

    bool hasInput{false};
    bool hasOutput{false};

    using Ptr = juce::ReferenceCountedObjectPtr<MidiDeviceListEntry>;
};

class MidiDeviceList final
{
public:
    MidiDeviceList(juce::MidiInputCallback* callback);

    void refresh();
    [[nodiscard]] int size() const;

    [[nodiscard]] MidiDeviceListEntry::Ptr get(int index) const;

    [[nodiscard]] bool isEnabled(bool input, int index) const;
    void setActive(bool input, int index, bool active);

    void iterate(void visit(MidiDeviceListEntry* entry));

    juce::ReferenceCountedArray<MidiDeviceListEntry>* getDevices();

private:
    juce::ReferenceCountedArray<MidiDeviceListEntry> devices;
    juce::MidiInputCallback* callback;

    void updateDeviceList();
    [[nodiscard]] bool hasDeviceListChanged(const juce::Array<juce::MidiDeviceInfo>& availableDevices) const;
    void closeUnpluggedDevices(const juce::Array<juce::MidiDeviceInfo>& currentlyPluggedInDevices);
    void closeDevice(bool input, int index);
    bool openDevice(bool input, int index);
    [[nodiscard]] juce::ReferenceCountedObjectPtr<MidiDeviceListEntry> findDevice(
        const juce::MidiDeviceInfo& device) const;
};
