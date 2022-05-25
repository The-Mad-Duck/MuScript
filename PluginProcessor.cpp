#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MuScriptAudioProcessor::MuScriptAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

MuScriptAudioProcessor::~MuScriptAudioProcessor()
{
    
}

//==============================================================================
const juce::String MuScriptAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MuScriptAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MuScriptAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MuScriptAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MuScriptAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MuScriptAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MuScriptAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MuScriptAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MuScriptAudioProcessor::getProgramName(int index)
{
    return {};
}

void MuScriptAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void MuScriptAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MuScriptAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MuScriptAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void MuScriptAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    if (updateNeeded == true)
    {
        parse.resetTable();
        parse.updateLogic(s + " ");
        updateNeeded = false;
    }

    for (int channel = 0; (channel < totalNumInputChannels && !updateNeeded); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            parse.alphaVals[26] = channelData[sample];
            for (int i = 0; (i < parse.values.size() && i >= 0 && !updateNeeded); i++)   //this is so you can exit with a negative number
            {
                switch (parse.values[i].op)
                {
                    case 0:             //assignment
                        (parse.values[i].type) ? (parse.alphaVals[parse.values[i].rt] = parse.values[i].getFloat()) : (parse.alphaVals[parse.values[i].rt] = parse.alphaVals[parse.values[i].a2]);
                        break;
                    case 1:             //add
                        (parse.values[i].type) ? (parse.alphaVals[parse.values[i].rt] = parse.alphaVals[parse.values[i].a1] + parse.values[i].getFloat()) : (parse.alphaVals[parse.values[i].rt] = parse.alphaVals[parse.values[i].a1] + parse.alphaVals[parse.values[i].a2]);
                        break;
                    case 2:             //subtract
                        (parse.values[i].type) ? (parse.alphaVals[parse.values[i].rt] = parse.alphaVals[parse.values[i].a1] - parse.values[i].getFloat()) : (parse.alphaVals[parse.values[i].rt] = parse.alphaVals[parse.values[i].a1] - parse.alphaVals[parse.values[i].a2]);
                        break;
                    case 3:             //mult
                        (parse.values[i].type) ? parse.alphaVals[parse.values[i].rt] = parse.alphaVals[parse.values[i].a1] * parse.values[i].a2f : parse.alphaVals[parse.values[i].rt] = parse.alphaVals[parse.values[i].a1] * parse.alphaVals[parse.values[i].a2];
                        break;
                    case 4:             //divide
                        if(abs((parse.values[i].type) ? parse.values[i].getFloat() : parse.alphaVals[parse.values[i].a2]) > 0)
                        (parse.values[i].type) ? (parse.alphaVals[parse.values[i].rt] = parse.alphaVals[parse.values[i].a1] / parse.values[i].getFloat()) : (parse.alphaVals[parse.values[i].rt] = parse.alphaVals[parse.values[i].a1] / parse.alphaVals[parse.values[i].a2]);
                        break;
                    case 5:             //modulo
                        if (abs((parse.values[i].type) ? parse.values[i].getFloat() : parse.alphaVals[parse.values[i].a2]) > 0)
                            (parse.values[i].type) ? (parse.alphaVals[parse.values[i].rt] = fmod(parse.alphaVals[parse.values[i].a1], parse.values[i].getFloat())) : (parse.alphaVals[parse.values[i].rt] = fmod(parse.alphaVals[parse.values[i].a1], parse.alphaVals[parse.values[i].a2]));
                        break;
                    case 6:             //absolute value
                        (parse.values[i].type) ? (parse.alphaVals[parse.values[i].rt] = abs(parse.values[i].getFloat())) : (parse.alphaVals[parse.values[i].rt] = abs(parse.alphaVals[parse.values[i].a2]));
                        break;
                    case 7:             //sine
                        (parse.values[i].type) ? (parse.alphaVals[parse.values[i].rt] = std::sin(parse.values[i].getFloat())) : (parse.alphaVals[parse.values[i].rt] = std::sin(parse.alphaVals[parse.values[i].a2]));
                        break;
                    case 8:             //cosine
                        (parse.values[i].type) ? (parse.alphaVals[parse.values[i].rt] = std::cos(parse.values[i].getFloat())) : (parse.alphaVals[parse.values[i].rt] = std::cos(parse.alphaVals[parse.values[i].a2]));
                        break;
                    case 9:             //tangent
                        (parse.values[i].type) ? (parse.alphaVals[parse.values[i].rt] = std::tan(parse.values[i].getFloat())) : (parse.alphaVals[parse.values[i].rt] = std::tan(parse.alphaVals[parse.values[i].a2]));
                        if(i < 0)
                        break;
                    case 10:            //power
                        (parse.values[i].type) ? (parse.alphaVals[parse.values[i].rt] = std::pow(parse.alphaVals[parse.values[i].a1], parse.values[i].getFloat())) : (parse.alphaVals[parse.values[i].rt] = std::pow(parse.alphaVals[parse.values[i].a1], parse.alphaVals[parse.values[i].a2]));
                        break;
                    case 11:            //jump if less than
                        if (parse.values[i].type) {
                            if (parse.alphaVals[parse.values[i].a1] < parse.values[i].getFloat())
                                i = parse.values[i].rt;}
                        else
                            if (parse.alphaVals[parse.values[i].a1] < parse.alphaVals[parse.values[i].a2])
                                i = parse.values[i].rt;
                        break;
                    case 12:            //jump if greater than
                        if (parse.values[i].type){
                            if (parse.alphaVals[parse.values[i].a1] > parse.values[i].getFloat())
                                i = parse.values[i].rt;}
                        else
                            if (parse.alphaVals[parse.values[i].a1] > parse.alphaVals[parse.values[i].a2])
                                i = parse.values[i].rt;
                        break;
                        
                    default:            //nothing lmfao
                        break;
                }
            }
            channelData[sample] = parse.alphaVals[26];
        }
    }
}

//==============================================================================
bool MuScriptAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MuScriptAudioProcessor::createEditor()
{
    return new MuScriptAudioProcessorEditor(*this);
}

//==============================================================================
void MuScriptAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ValueTree tree("MyXML");
    var v(s);
    DBG("Storing value " << s << "\n");
    tree.setProperty("TextValue", v, nullptr);
    auto xml = tree.createXml();
    copyXmlToBinary(*xml, destData);
}

void MuScriptAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr)
    {
        auto tree = ValueTree::fromXml(*xml);
        var v = tree.getProperty("TextValue");
        s = v.toString().toStdString();
        DBG("my value is " << s << "from tree" << v.toString() <<"\n");
        this->createEditorIfNeeded()->repaint();
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MuScriptAudioProcessor();
}