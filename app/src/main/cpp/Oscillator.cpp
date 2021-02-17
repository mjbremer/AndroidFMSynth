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

#include "Oscillator.h"
#include <math.h>
#include <android/log.h>

#define TWO_PI (3.14159 * 2)
#define AMPLITUDE 0.3
#define FREQUENCY 440.0
#define MODULATOR 440.0

void Oscillator::setSampleRate(int32_t sampleRate) {
    sampleRate_ =  sampleRate;
    phaseIncrement_ = (TWO_PI * frequency_) / (double) sampleRate;
    phaseIncrement2_ = (TWO_PI * P6) / (double) sampleRate;
}

void Oscillator::setWaveOn() {
    isWaveOn_.store(!isWaveOn_.load());
}

void Oscillator::setFreq(double val) {
    frequency_ = val;
    phaseIncrement_ = (TWO_PI * frequency_) / (double) sampleRate_;
}

void Oscillator::setMod(double val) {
    P6= val;
    phaseIncrement2_ = (TWO_PI * P6) / (double) sampleRate_;
}

void Oscillator::setADSR(double val, int index){
    ADSR[index]=val;
}

float* Oscillator::genSample(float adsr) {
    float* ret;
    float fmpamp =(float) (P7 + (P8-P7)*adsr);
    float fmpart = (float) (sin(phase2_));
    float fm = fmpamp*fmpart;
    // Calculates the next sample value for the sine wave.
    ret = (float) (sin(phase_ + fm) * adsr;

    // Increments the phase, handling wrap around.
    phase_ += phaseIncrement_;
    phase2_ += phaseIncrement2_;
    if (phase_ > TWO_PI) phase_ -= TWO_PI;
    if (phase2_ > TWO_PI) phase2_ -= TWO_PI;
    time_++;
    return ret;
}

void Oscillator::render(float *audioData, int32_t numFrames) {

    int32_t framesProcessed = 0;
    float adsr;

    if (state == OFF) {

        // Output silence
        for (int i = 0; i < numFrames; i++) {
            audioData[i] = 0;
        }

        // Make sure osc is reset for when we attack
        phase_ = 0;
        time_ = 0;

        // Check if we want to start playing next render
        if (isWaveOn_.load()) {
            state = ATTACK;
        }
        return;
    }

    if (state == ATTACK) {
        for (int i = framesProcessed; i < numFrames; i++) {
            // If wave has stopped playing, go straight to release stage
            if (!isWaveOn_.load()) {
                state = RELEASE;
                note_off_time = time_;
                break;
            }

            // Otherwise, calculate a sample and level it
            adsr = time_ / attack_time_samples;
            audioData[i] = genSample(adsr);
            framesProcessed++;

            // If we've gotten all the way to the end of attack, move to decay stage
            if (time_ == attack_time_samples) {
                state = DECAY;
                break;
            }
        }
    }

    if (state == DECAY) {
        for (int i = framesProcessed; i < numFrames; i++) {
            // If wave has stopped playing, go to decay stage
            if (!isWaveOn_.load()) {
                state = RELEASE;
                note_off_time = time_;
                break;
            }

            // Otherwise, generate a sample and level it
            adsr = 1.0 - (( 1 - sustain_level) * ((time_ + attack_time) / (time_ + attack_time + decay_time)));
            audioData[i] = genSample(adsr);
            framesProcessed++;
            if (time_ == attack_time_samples + decay_time_samples) {
                state = SUSTAIN;
                break;
            }
        }
    }

    if (state == SUSTAIN) {
        for (int i = framesProcessed; i < numFrames; i++) {
            // If wave has stopped playing, go to decay stage
            if (!isWaveOn_.load()) {
                state = RELEASE;
                note_off_time = time_;
                break;
            }

            // Otherwise, generate a sample and level it
            adsr = sustain_level;
            audioData[i] = genSample(adsr);
            framesProcessed++;
        }
    }

    if (state == RELEASE) {
        if (isWaveOn_.load()) {
                state = OFF;
                for (int i = 0; i < numFrames; i++) {
                    audioData[i] = 0;
                }
                return;
        }

        for (int i = framesProcessed; i < numFrames; i++) {

            if (time_ > note_off_time + release_time) {
                state = OFF;
                audioData[i] = 0;
            }

            // Otherwise, generate a sample and level it
            adsr = sustain_level *  1.0 - ((time_ - note_off_time) / (release_time)));
            audioData[i] = genSample(adsr);
            framesProcessed++;
        }
    }
}
