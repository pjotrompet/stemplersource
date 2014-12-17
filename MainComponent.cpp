/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "AudioCallback.h"
#include "loadfile.h"
#include "stempler.h"
#include <vector>
#include <complex>
#include <fftw3.h>


//==============================================================================




MainContentComponent::MainContentComponent()
{
    setSize (500, 400);

	tn_sldr = new Slider(("lekker sliden!"));
	(*tn_sldr).setBounds(50, 50, 260, 50);
	addAndMakeVisible(tn_sldr);
	(*tn_sldr).addListener(this);

	(*tn_sldr).setRange(3, 48, 1);

 	load_but = new TextButton(("play"));
    (*load_but).setBounds(10, 10, 50, 20);
    addAndMakeVisible(load_but);
    (*load_but).addListener(this);
    
    calc_but = new TextButton(("Calc."));
    (*calc_but).setBounds(100, 10, 50, 20);
    addAndMakeVisible(calc_but);
    (*calc_but).addListener(this);

	addKeyListener(this);
	setWantsKeyboardFocus(true);

	out_buf = new float[512];
	
	key_count = 3;								// amount of notes in tuning (per octave?) 	<-- maak een slider voor keuze aantal tonen?
	mirror_tuning = true;						// invert intervals around 3/2				<-- maak schakelaar voor keuze inverteren?
	peak_tresh = 0.01;							// peakdetection treshold					<--	maak een slider/draaiknop voor treshold?

	toonh.push_back(1);
	toonh.push_back(3.0/2);
	toonh.push_back(2);

	// init keyboard:
	change_keyboard(key_count);
	//


}

MainContentComponent::~MainContentComponent()
{
    deleteAllChildren();
}

void MainContentComponent::buttonClicked(Button* button) {
    if(button == load_but) {
		load_button();
		this->startAudioCallback();
	} else if (button == calc_but) {
		calc_tuning();
	} else {
		int key = 0;
		while((button != keyboard[key]) && key < keyboard.size()) ++key;
		float speed = toonh[key];
		notes.push_back(note(file_buf, file_len, file_chn, speed));
		std::cout<<"notes: "<<notes.size()<<std::endl;
	}
}

void MainContentComponent::sliderValueChanged(Slider* slider) {
	if(slider == tn_sldr) {
		change_keyboard((*slider).getValue());
	}
}

void MainContentComponent::change_keyboard(int keys) {
	for(int key=0; key<keyboard.size(); key++) {
		delete keyboard[key];
	}
	keyboard.clear();
	this->key_count = keys;
	for(int key=0; key<key_count; key++) {
		keyboard.push_back(new TextButton((" ")));
		keyboard[key]->setBounds(50+25*key, 100, 20, 50);
		addAndMakeVisible(keyboard[key]);
		keyboard[key]->addListener(this);
	}
}

void MainContentComponent::load_button() {
	FileChooser myChooser ("I pitty the file who doesn't choose a fool!", File::getSpecialLocation(File::userHomeDirectory), "*.wav");
        if(myChooser.browseForFileToOpen()) {
            File wavFile (myChooser.getResult());
			long length_of_file; int channels, f_fs;
            float* data = load_file(wavFile.getFullPathName(), &length_of_file, &channels, &f_fs);
			file_buf = data;
			file_fs = f_fs;
			file_len = length_of_file;
			file_chn = channels;
        }

}

void MainContentComponent::calc_tuning() {								//calculate tuning

	//initialise fft i/o-buffers:				
	fftw_complex *in, *out;												
	fftw_plan p;					
	
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * file_len);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * file_len);
	
	p = fftw_plan_dft_1d(file_len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	for(long n=0; n<file_len; n++) {									// fill fft input buffer:
		in[n][0] = file_buf[n];
		in[n][1] = 0;
	}
	fftw_execute(p);													// execute fft

	file_mX = new float[file_len];										//get mag and phase:
	file_pX = new float[file_len];
	for(long n=0; n<file_len/2; n++) {
		file_mX[n] = c_abs(out[n][0], out[n][1]);
		file_pX[n] = c_arg(out[n][0], out[n][1]);
/*
		std::cout<<"magnitude: "<<file_mX[n];
		std::cout<<"\tphase: "<<file_pX[n]<<std::endl;
*/
	}
	
	fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out); 

	ladder = new tuning(file_mX, file_len/2, key_count, mirror_tuning, peak_tresh, file_fs);	//(spectrum, length of spectrum (N/2 ivm symmetrie),
																								//	amount of keys, mirror intervals round 3/2?, 
																								//		peak detection treshold)  
	

}

void MainContentComponent::fill_buffer(int bufsize) {
	for(int i=0; i<notes.size(); i++) if(notes[i].del_me) notes.erase(notes.begin()+i);
	for(long n=0; n<bufsize; n++) {
		out_buf[n] = 0;
		for(int i=0; i<notes.size(); i++) {
			out_buf[n]+=notes[i].get_samp();							// query note for samples
		}
	}
}


void MainContentComponent::audioCallback(float** buffer, int channels, int frames) {
	fill_buffer(frames);
	for(int c=0; c<channels; c++) {
		for(long n=0; n<512; n++) {
			buffer[c][n] = out_buf[n];
		}		
	}
	
}

bool MainContentComponent::keyPressed(const KeyPress &key, Component *origin) {
	float speed = 1;
	if(key == 'a') {			//1
		std::cout<<"aaaah!\n";
		speed = 1;
	} else if(key == 's') {		//2
		speed = 9.0/8;
	} else if(key == 'd') {		//3
		speed = 9.0/8*9.0/8;
	} else if(key == 'f') {		//4
		speed = 4.0/3;
	} else if(key == 'g') {		//5
		speed = 3.0/2;
	} else if(key == 'h') {		//6
		speed = 3.0/2*9.0/8;
	} else if(key == 'j') {		//7
		speed = 3.0/2*9.0/8*9.0/8;
	} else if(key == 'k') {
		speed = 2;
	}

	notes.push_back(note(file_buf, file_len, file_chn, speed));
	std::cout<<"notes: "<<notes.size()<<std::endl;
}


void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffeeddff));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

note::note(float* n_buf, long n_len, int n_chn, float n_speed) {
	this->buffer = n_buf;
	this->len = n_len;
	this->chn = n_chn;
	this->read_p = 0;
	this->del_me = false;
	this->speed = n_speed;
}

float note::get_samp() {
	float sample = 0;
	if(read_p >= len) {
		del_me = true;		
	} else {
		sample = buffer[(int)read_p];
		read_p+=speed;
	}

	return sample;
}


