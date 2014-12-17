/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioCallback.h"
#include <vector>
#include "loadfile.h"
#include "stempler.h"
#include <fftw3.h>


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/



class note {
public:
	note(float* n_buf, long n_len, int n_chn, float n_speed);
	float get_samp();
	bool del_me;
private:
	float* buffer;
	long len;
	int chn;
	float read_p;
	float speed;
};

class MainContentComponent   : public Component, public ButtonListener, public SliderListener, private AudioCallback, public KeyListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
	
    void paint (Graphics&);
    void resized();

    void buttonClicked(Button* button);
	void sliderValueChanged(Slider* sider);
	void load_button();
	void fill_buffer(int bufsize);
	void calc_tuning();

	float* out_buf;

	float* file_buf;	
	int file_fs;
	long file_len;
	int file_chn;
	bool is_file_loaded;

	float* file_mX;
	float* file_pX;

	std::vector<float> toonh;
	
	bool is_calc_done;
	tuning* ladder;
	int key_count;
	bool mirror_tuning;
	float peak_tresh;

private:
    //==============================================================================
    void audioCallback(float** buffer, int channels, int frames);
	bool keyPressed(const KeyPress &key, Component* origin);
	std::vector<note> notes;

    TextButton* load_but;
    TextButton* calc_but;
	std::vector<TextButton*> keyboard;
	Slider* tn_sldr;
	Slider* th_sldr;
	void change_keyboard(int keys);	

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
