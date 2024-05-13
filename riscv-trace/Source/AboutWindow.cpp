/*
  ==============================================================================

    AboutWindow.cpp
    Created: 13 May 2024 4:16:13pm
    Author:  veselyaev

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AboutWindow.h"

//==============================================================================
AboutPanel::AboutPanel()
{
    setSize(getParentWidth(), getParentHeight());
}

AboutPanel::~AboutPanel()
{
}

void AboutPanel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("AboutWindow", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void AboutPanel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
