/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PART1_OSCILLATOR_H
#define PART1_OSCILLATOR_H

#include <atomic>
#include <stdint.h>

class Oscillator {
public:
    void setWaveOn();

    void setSampleRate(int32_t sampleRate);

    void render(float *audioData, int32_t numFrames);

    void setFreq(double val);

    void setMod(double val);

    void setADSR(double val, int index);

private:

    enum {ATTACK, DECAY, SUSTAIN, RELEASE, OFF} state;
    
    unsigned int attack_time_samples;
    unsigned int decay_time_samples;
    float sustain_level;
    unsigned int release_time_samples;


    unsigned int note_off_time;


    // We use an atomic bool to define isWaveOn_ because it is accessed from multiple threads.
    std::atomic<bool> isWaveOn_{false};
    double frequency_ = 440.0;

    double P5 = 440;
    double P6 = P5 * 1.0;
    double P7 = 0;
    double P8 = 5;

    double ADSR[48000];

    double phase_ = 0.0;
    double phaseIncrement_ = 0.0;
    double phase2_ = 0.0;
    double phaseIncrement2_ = 0.0;
    double time_ = 0;
    double sampleRate_;
    double duration_ = 1;
};



#endif //PART1_OSCILLATOR_H
