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
package com.example.wavemaker;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.MotionEvent;
import android.widget.Button;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }
    private boolean isLocked;
    private Button buttonStart;
    private Button buttonLock;

    private SeekBar carBar;
    private SeekBar modBar;

    private EditText carFreqText;
    private EditText modFreqText;
    private TextView hzText;
    private TextView hzText2;

    private native void touchEvent(int action);
    private native void startEngine();
    private native void stopEngine();
    private native void buttonPress();
    private native void setFreq(double val);
    private native void setMod(double val);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        startEngine();
        buttonStart = (Button) findViewById(R.id.startButton);
        buttonLock = (Button) findViewById(R.id.lockButton);
        carBar =  findViewById(R.id.carBar);
        modBar =  findViewById(R.id.modBar);
        buttonStart.setTag(1);
        buttonStart.setText("Play");
        buttonStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                buttonPress();
                final int status =(Integer) v.getTag();
                if(status == 1) {
                    //buttonStart.setText("Pause");
                    v.setTag(0); //pause
                } else {
                    buttonStart.setText("Play");
                    v.setTag(1); //pause
                }
            }
        });

        buttonLock.setTag(1);
        buttonLock.setText("Lock");
        buttonLock.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final int status =(Integer) v.getTag();
                if(status == 1) {
                    buttonLock.setText("Unlock");
                    isLocked = true;
                    int progress = carBar.getProgress();
                    modBar.setProgress(progress);
                    ((TextView)findViewById(R.id.carrierFrequency)).setText(String.valueOf(progress));
                    ((TextView)findViewById(R.id.modFrequency)).setText(String.valueOf(progress));
                    setFreq(progress);
                    setMod(progress);
                    v.setTag(0);
                } else {
                    buttonLock.setText("Lock");
                    isLocked = false;
                    v.setTag(1);
                }
            }
        });

        carFreqText = (findViewById(R.id.carrierFrequency));
        modFreqText = (findViewById(R.id.modFrequency));
        hzText = ((TextView)findViewById(R.id.editText3));
        hzText2 = ((TextView)findViewById(R.id.editText2));
        hzText.setInputType(0);
        modFreqText.setInputType(0);
        carFreqText.setInputType(0);
        hzText2.setInputType(0);


       /* modFreqText.addTextChangedListener(new TextWatcher() {

            public void afterTextChanged(Editable s) {
                int progress = Integer.parseInt(modFreqText.getText().toString());
                modBar.setProgress(progress);
                setMod(progress+440.0);
                ((TextView)findViewById(R.id.modFrequency)).setText(String.valueOf(progress));
                if(isLocked){
                    carBar.setProgress(progress);
                    setFreq(progress+440.0);
                    ((TextView)findViewById(R.id.carrierFrequency)).setText(String.valueOf(progress));
                }
            }

            public void beforeTextChanged(CharSequence s, int start, int count, int after) {}

            public void onTextChanged(CharSequence s, int start, int before, int count) {}
        });
*/





        carBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener(){
            int progressChangedValue = 0;
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progressChangedValue = progress;
                setFreq(progress);

                ((TextView)findViewById(R.id.carrierFrequency)).setText(String.valueOf(progress));
                if(isLocked){
                    modBar.setProgress(progress);
                    setMod(progress);
                    ((TextView)findViewById(R.id.modFrequency)).setText(String.valueOf(progress));
                }
            }
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        modBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener(){
            int progressChangedValue = 0;
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progressChangedValue = progress;
                setMod(progress);
                ((TextView)findViewById(R.id.modFrequency)).setText(String.valueOf(progress));
                if(isLocked){
                    carBar.setProgress(progress);
                    setFreq(progress);
                    ((TextView)findViewById(R.id.carrierFrequency)).setText(String.valueOf(progress));
                }
            }
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        touchEvent(event.getAction());
        return super.onTouchEvent(event);
    }

    @Override
    public void onDestroy() {
        stopEngine();
        super.onDestroy();
    }
}
