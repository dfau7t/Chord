#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
Switch oscControl, octDown, octUp; //waveform control, octave down, octave up
Switch seventh, ninth;
Oscillator osc[5];
uint8_t octave = 4, octValue;

long key[24]; //24 key inputs
uint8_t count = 0; //Oscillator index [0-Saw, 1-Square, 2-Triangle, 3-Sine]

uint8_t chord[12][10];

void Chords() {
	for(uint8_t i = 0; i < 12; i++) { // C C# D D# E F F# G G# A A# B
		chord[i][0] = i; //root
		chord[i][1] = chord[i][0] + 3; //flat third
		chord[i][2] = chord[i][1] + 1; //third
		chord[i][3] = chord[i][2] + 2; //flat fifth
		chord[i][4] = chord[i][3] + 1;//fifth
		chord[i][5] = chord[i][4] + 1;//sharp fifth
		chord[i][6] = chord[i][5] + 1;//sixth
		chord[i][7] = chord[i][6] + 1;//flat seventh
		chord[i][8] = chord[i][7] + 1;//seventh
		chord[i][9] = chord[i][8] + 3;//ninth		
	}
}

void UpdateOctave() {
	if(octDown.RisingEdge()) {
		octave -= 1;
	}
	if(octUp.RisingEdge()) {
		octave += 1;
	}

	if(octave > 6) {octave = 6;}
	if(octave < 2) {octave = 2;}

	octValue = 12 * octave ;
	//if octDown.RisingEdge() octave -=1; 
	//if octUp.RisingEdge() octave +=1;
	//if octave > 5 octave = 5;if octave < 3 octave = 3;
	//octValue = octave*12
}

void InitOsc(float samplerate) {
	//
}

void UpateFreq(uint8_t root) {
	//if root < 12, osc[0].freq = chord[i][0] + octValue, osc[1].freq = chord[i][2] + octValue, osc[2].freq = chord[i][4], ...
	//if root > 12, osc[0].freq = chord[i][0] + octValue, osc[1].freq = chord[i][1] + octValue, osc[2].freq = chord[i][4], ...
	if(root < 12) {
		osc[0].SetFreq(mtof(chord[root][0] + octValue)); //root
		osc[1].SetFreq(mtof(chord[root][2] + octValue)); //third
		osc[2].SetFreq(mtof(chord[root][4] + octValue)); //fifth
		osc[3].SetFreq(mtof(chord[root][8] + octValue)); //seventh
		osc[4].SetFreq(mtof(chord[root][9] + octValue)); //ninth
	} else if(root >= 12) {
		osc[0].SetFreq(mtof(chord[root - 12][0] + octValue)); //root
		osc[1].SetFreq(mtof(chord[root - 12][1] + octValue)); //flat third
		osc[2].SetFreq(mtof(chord[root - 12][4] + octValue)); //fifth
		osc[3].SetFreq(mtof(chord[root][8] + octValue)); //seventh
		osc[4].SetFreq(mtof(chord[root][9] + octValue)); //ninth

	}
}

void InitKeys() {
	AdcChannelConfig adc_cfg[3];
    adc_cfg[0].InitMux(seed::A0, 8, seed::D1, seed::D2, seed::D3); //keys 0-7
    adc_cfg[1].InitMux(seed::A1, 8, seed::D1, seed::D2, seed::D3); //keys 8-15
    adc_cfg[2].InitMux(seed::A2, 8, seed::D1, seed::D2, seed::D3); //keys 16-23
    hw.adc.Init(adc_cfg, 3);
    hw.adc.Start();
}

void HandleKeyInput() {
	for(uint8_t i = 0; i < 8; i++) {
		key[i] = hw.adc.GetMux(0, i);
		key[i + 8] = hw.adc.GetMux(1, i);
		key[i + 16] = hw.adc.GetMux(2, i);

		if(key[i] < 32767) {key[i] = 0;}
		if(key[i + 8] < 32767) {key[i + 8] = 0;}
		if(key[i + 16] < 32767) {key[i + 16] = 0;}
		if(key[i] > 40000) {key[i] = 1;}
		if(key[i + 8] > 40000) {key[i + 8] = 1;}
		if(key[i + 16] > 40000) {key[i + 16] = 1;}
	}

	for(uint8_t i = 0; i < 24; i++) {
		if(key[i] > 0) {
			//update osc with chord freq
			hw.PrintLine("Key: %i", i);
			UpateFreq(i);
			//setamp = 1
		} else {
			//setamp = 0
		}
	}
}


void UpdateWaveform() {
	int wf;
	if(oscControl.RisingEdge()) {
		count += 1;
		// hw.PrintLine("%d", count);
		if(count > 3) {count = 0;}

		switch(count) {
			case 0: hw.PrintLine("Saw"); wf = Oscillator::WAVE_POLYBLEP_SAW; break;
			case 1: hw.PrintLine("Sqr"); wf = Oscillator::WAVE_SQUARE; break;
			case 2: hw.PrintLine("Tri"); wf = Oscillator::WAVE_TRI; break;
			case 3: hw.PrintLine("Sin"); wf = Oscillator::WAVE_SIN; break;
		}
		for(uint8_t i = 0; i < 5; i++) {
			osc[i].SetWaveform(wf);
		}
	}
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
	}
}

int main(void) {
    hw.Init();
	hw.SetLed(true);
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	float sr = hw.AudioSampleRate();

	Chords();

	//Initialize individual buttons
	oscControl.Init(seed::D4);
	octDown.Init(seed::D5);
	octUp.Init(seed::D6);

	InitKeys();
	InitOsc(sr);

	for(uint8_t i = 0; i < 5; i++) {
		osc[i].Init(sr);
	}
    

    hw.StartLog(true);

    while(1)
    {
		oscControl.Debounce();
		HandleKeyInput(); //mux
		UpdateOctave(); //octave control
		UpdateWaveform(); //oscControl

        System::Delay(10);
		// hw.PrintLine("%d", key[8]);


		
		
    }
}

/*
keys 0-11 major triads
keys 12-23 minor triads

osc freq = chord + octave


*/