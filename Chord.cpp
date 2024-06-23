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

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{

	//UpdateOctave();

	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
	}
}

void InitOsc() {
	//add code
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

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	float samplerate = hw.AudioSampleRate();

	hw.SetLed(true);
	InitChords();

	AdcChannelConfig adc_cfg;
	adc_cfg.InitMux(D4, 8, D1, D2, D3);
	hw.adc.Init(&adc_cfg, 1);

	octUp.Init(D13);
	octDown.Init(D14);
	oscControl.Init(D11, D12);

	for(uint8_t i = 0; i < 5; i++) {
		osc[i].Init(samplerate);
		osc[i].SetWaveform(Oscillator::WAVE_SIN);
		osc[i].SetAmp(0.3);
		osc[i].SetFreq((i * 2) + 200);
	}
	
	while(1) {

		hw.Print("Input 1: %d \n", hw.adc.GetMux(0, 0));
		hw.Print("Input 2: %d \n", hw.adc.GetMux(0, 1));
		hw.Print("Input 3: %d \n", hw.adc.GetMux(0, 2));
		hw.Print("Input 4: %d \n", hw.adc.GetMux(0, 3));
		hw.Print("Input 5: %d \n", hw.adc.GetMux(0, 4));
		hw.Print("Input 6: %d \n", hw.adc.GetMux(0, 5));
		hw.Print("Input 7: %d \n", hw.adc.GetMux(0, 6));
		hw.Print("Input 8: %d \n", hw.adc.GetMux(0, 7));
		System::Delay(250);

	}
}
