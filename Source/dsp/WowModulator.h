#pragma once

class WowModulator
{
public:
    void prepare (double sampleRate);
    void reset();

    void setAmount (float amount);
    float getNextOffsetSamples();
};
