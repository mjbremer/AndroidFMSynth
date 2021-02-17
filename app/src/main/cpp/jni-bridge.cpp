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
#include <jni.h>
#include <android/input.h>
#include "AudioEngine.h"

static AudioEngine *audioEngine = new AudioEngine();

extern "C" {

JNIEXPORT void JNICALL
Java_com_example_wavemaker_MainActivity_touchEvent(JNIEnv *env, jobject obj, jint action) {
//    switch (action) {
//        case AMOTION_EVENT_ACTION_UP:
//            audioEngine->setToneOn();
//            break;
//        default:
//            break;
//    }
}

JNIEXPORT void JNICALL
Java_com_example_wavemaker_MainActivity_buttonPress(JNIEnv *env, jobject obj) {
    audioEngine->setToneOn();
}

JNIEXPORT void JNICALL
Java_com_example_wavemaker_MainActivity_startEngine(JNIEnv *env, jobject /* this */) {
    audioEngine->start();
}

JNIEXPORT void JNICALL
Java_com_example_wavemaker_MainActivity_stopEngine(JNIEnv *env, jobject /* this */) {
    audioEngine->stop();
}

JNIEXPORT void JNICALL
Java_com_example_wavemaker_MainActivity_setFreq(JNIEnv *env, jobject obj, jdouble val) {
    audioEngine->changeFrequency(val);
}

JNIEXPORT void JNICALL
Java_com_example_wavemaker_MainActivity_setMod(JNIEnv *env, jobject obj, jdouble val) {
    audioEngine->changeMod(val);
}

}
