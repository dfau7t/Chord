#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;
using namespace seed;

DaisySeed hw;
Switch octUp, octDown;
Switch3 oscControl;

Oscillator osc[5];

uint8_t chords[12][10];
uint8_t octave = 3;
uint8_t keys[24];

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{

	//UpdateOctave();

	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
	}
}

void InitOsc(float samplerate) {

	for(uint8_t i = 0; i < 5; i++) {
		osc[i].Init(samplerate);
		osc[i].SetWaveform(Oscillator::WAVE_SIN);
		osc[i].SetAmp(0.3);
		osc[i].SetFreq((i * 2) + 200);
	}
}

void InitKeys() {
	AdcChannelConfig adc_cfg[3];
	adc_cfg[0].InitMux(D3, 8, D0, D1, D2); //buttons 0-7
	adc_cfg[1].InitMux(D4, 8, D0, D1, D2); //buttons 8-15
	adc_cfg[2].InitMux(D5, 8, D0, D1, D2); //buttons 16-23
	hw.adc.Init(&adc_cfg[3], 3);
	hw.adc.Start();

	octUp.Init(D13);
	octDown.Init(D14);
	oscControl.Init(D11, D12);
}

void SetupKeys() {
	for(uint8_t i = 0; i < 8; i++) {
		keys[i] = hw.adc.GetMux(0, i);
	}

	for(uint8_t i = 0; i >= 8 && i < 16; i++) {
		keys[i] = hw.adc.GetMux(0, i);
	}

	for(uint8_t i = 0; i >= 16 && i < 24; i++) {
		keys[i] = hw.adc.GetMux(0, i);
	}
}

void UpdateKeys() {
	switch(keys[24]) {
		// osc1 = chord[i][0], osc2 = chord[i][2] * octave, osc3 = chord[i][4] * octave, osc4 = chord[i][8] * octave, osc5 = chord[i][9] * octave
		case 0: break;
		case 1: break;
		case 2: break;
		case 3: break;
		case 4: break;
		case 5: break;
		case 6: break;
		case 7: break;
		case 8: break;
		case 9: break;
		case 10: break;
		case 11: break;
		case 12: break;
		case 13: break;
		case 14: break;
		case 15: break;
		case 16: break;
		case 17: break;
		case 18: break;
		case 19: break;
		case 20: break;
		case 21: break;
		case 22: break;
		case 23: break;
	}
}

void Envelope() {
	//handles  amp envelope for all oscillators
	//seperate from master volume
}

void InitChords() {
	for(uint8_t i = 0; i < 12; i++) {
		chords[i][0] = i + 1; //root
		chords[i][1] = chords[i][0] + 3; //flat third
		chords[i][2] = chords[i][1] + 1; //third
		chords[i][3] = chords[i][2] + 2; //flat fifth
		chords[i][4] = chords[i][3] + 1;//fifth
		chords[i][5] = chords[i][4] + 1;//sharp fifth
		chords[i][6] = chords[i][5] + 1;//sixth
		chords[i][7] = chords[i][6] + 1;//flat seventh
		chords[i][8] = chords[i][7] + 1;//seventh
		chords[i][9] = chords[i][8] + 3;//ninth		
	}
}

void UpdateOctave() {
	if(octUp.RisingEdge()) {
		octave += 1;
	}

	if(octDown.RisingEdge()) {
		octave -= 1;
	}

	if(octave < 0) {octave = 0;}
	if(octave > 4) {octave = 4;}
}

void UpdateWaveform() {
	uint8_t wf;
	switch (oscControl.Read()) {
		case Switch3::POS_LEFT: wf = Oscillator::WAVE_POLYBLEP_SAW; break;
		case Switch3::POS_CENTER: wf = Oscillator::WAVE_SIN; break;
		case Switch3::POS_RIGHT: wf = Oscillator::WAVE_POLYBLEP_SQUARE; break;
	}

	for(uint8_t i = 0; i < 5; i++) {
		osc[i].SetWaveform(wf);
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	float samplerate = hw.AudioSampleRate();

	hw.SetLed(true);
	InitChords();

	//Troubleshoot
	InitKeys();
	InitOsc(samplerate);

	hw.StartLog(true);


	//switch
	
	while(1) {

		hw.Print("Input 1: %d \n", hw.adc.GetMux(0, 0));
		hw.Print("Input 2: %d \n", hw.adc.GetMux(0, 1));
		hw.Print("Input 3: %d \n", hw.adc.GetMux(0, 2));
		hw.Print("Input 4: %d \n", hw.adc.GetMux(0, 3));
		hw.Print("Input 5: %d \n", hw.adc.GetMux(0, 4));
		hw.Print("Input 6: %d \n", hw.adc.GetMux(0, 5));
		hw.Print("Input 7: %d \n", hw.adc.GetMux(0, 6));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(0, 7));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(1, 0));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(1, 1));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(1, 2));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(1, 3));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(1, 4));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(1, 5));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(1, 6));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(1, 7));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(2, 0));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(2, 1));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(2, 2));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(2, 3));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(2, 4));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(2, 5));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(2, 6));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(2, 7));


		System::Delay(250);

	}
}