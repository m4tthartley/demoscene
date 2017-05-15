
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <vector>
#include "../vstsdk/audioeffectx.h"
#include "w:/libs/math.c"

// #ifndef VST_EFFECT
// #	define VST_INSTRUMENT
// #endif

//#define DEBUG_PRINT

const int num_outputs = 2;
const int num_devices = 100;

struct SynthVstPlugin : public AudioEffectX {

	/*DeviceTypes device_id;
	char *name;
	char *title;
	int num_params;
	ParamInfo *param_names;
	float *param_defaults;
	bool is_instrument;*/

	SynthDevice devices[num_devices];
	FILE *debug_file;
	char progname[64];

	SynthVstPlugin(audioMasterCallback audioMaster) :
		AudioEffectX(audioMaster, num_devices, num_params) {

		strcpy(progname, VST_NAME);

		// todo: slow?
		for (int i = 0; i < num_devices; ++i) {
			devices[i] = {};
			for (int j = 0; j < num_params; ++j) {
				devices[i].params[j] = param_defaults[j];
				devices[i].type = VST_TYPE;
			}
		}

#ifdef DEBUG_PRINT
		debug_file = fopen("w:/demoscene/vsts/" VST_NAME " debug.txt", "w");
#endif

		/*setProgram(0);
		curProgram = 0;*/

		if (audioMaster) {
			//setNumInputs(0);

#ifdef VST_INSTRUMENT
				setNumInputs(0);
#endif
#ifdef VST_EFFECT
				setNumInputs(2);
#endif

			setNumOutputs(num_outputs);
			canProcessReplacing();

#ifdef VST_INSTRUMENT
				isSynth();
#endif

			setUniqueID('Mt53');
		}

		suspend();

		synth_init();
	}

	void debug_print(char *str, ...) {
#ifdef DEBUG_PRINT
		va_list args;
		va_start(args, str);

		char buf[256];
		vsnprintf_s(buf, 256, _TRUNCATE, str, args);
		//OutputDebugString(buf);
		fwrite(buf, strlen(buf), 1, debug_file);

		va_end(args);
#endif
	}

	void setProgram(VstInt32 program) {
		curProgram = program;
	}

	VstInt32 canDo(char *text) {
		if (!strcmp(text, "receiveVstEvents")) {
			return 1;
		}
		if (!strcmp(text, "receiveVstMidiEvent")) {
			return 1;
		}
		return -1;
	}

	bool getEffectName(char* name) { strncpy(name, VST_TITLE, kVstMaxEffectNameLen); return true; }
	bool getVendorString(char* text) { strncpy(text, "GiantJelly", kVstMaxVendorStrLen); return true; }
	bool getProductString(char* text) { strncpy(text, VST_TITLE, kVstMaxProductStrLen); return true; }

	void getParameterName(VstInt32 index, char* label) {
		//strcpy(label, "Matt's knob");
		strcpy(label, param_info[index].name);
	}

	float getParameter(VstInt32 index) {
		return devices[curProgram].params[index];
	}

	void setParameter(VstInt32 index, float value) {
		devices[curProgram].params[index] = value;
	}

	void getParameterDisplay(VstInt32 index, char *text) {
		sprintf(text, "%.2f", devices[curProgram].params[index]);
	}

	void getParameterLabel(VstInt32 index, char *label) {
		//strcpy(label, params[index].name);
	}

	VstInt32 getNumMidiInputChannels() {
		return 1;
	}
	VstInt32 getNumMidiOutputChannels() {
		return 0;
	}

	VstInt32 processEvents(VstEvents* events) {
		/*char str[64];
		sprintf(str, "Event %i: \n", events->numEvents);
		fwrite(str, strlen(str), 1, debug_file);*/
		/*if (events->numEvents > 0) {
		if (events->events[0]->type == kVstMidiType) {
		VstMidiEvent *e = (VstMidiEvent*)events->events[0];
		play_note = true;
		}
		if (events->events[0]->type == kVstSysExType) {
		VstMidiEvent *e = (VstMidiEvent*)events->events[0];
		play_note = true;
		}
		}*/

		for (int i = 0; i < events->numEvents; ++i) {
			if (events->events[i]->type == kVstMidiType) {

				VstMidiEvent *e = (VstMidiEvent*)events->events[i];
				int status = e->midiData[0] & 0xf0;

				/**/

				VstInt32 note = e->midiData[1] & 0x7f;
				VstInt32 velocity = e->midiData[2] & 0x7f;
				debug_print("status 0x%x, velocity %i \n", status, velocity);

				/*
				ableton midi data
				status 0x90, velocity 100
				status 0x80, velocity 64
				status 0x90, velocity 100
				status 0x80, velocity 64
				status 0x90, velocity 100
				status 0x80, velocity 64
				*/
				/*
				fl midi data
				status 0x80, velocity 64
				status 0x90, velocity 100
				status 0x80, velocity 64
				status 0x90, velocity 100
				status 0x80, velocity 64

				fl does 0x80 first
				*/

				if (status == 0x90 || status == 0x80) {
					if (status == 0x90 && velocity != 0) {
						synth_note_on(&devices[curProgram], note);
					} else {
						synth_note_off(&devices[curProgram], note);
					}
				}

				/*if (status == 0x90) {
				play_note = true;
				}
				if (status == 0x80) {
				play_note = false;
				}*/

				/*if (status == 0x90 || status == 0x80) {
				VstInt32 note = e->midiData[1] & 0x7f;
				VstInt32 velocity = e->midiData[2] & 0x7f;
				if (status == 0x80) {
				char *str = "MIDI EVENT 0x80\n"; fwrite(str, strlen(str), 1, debug_file);
				velocity = 0;
				}
				if (!velocity && (note == current_note)) {
				char *str = "MIDI EVENT !velocity && (note == current_note)\n"; fwrite(str, strlen(str), 1, debug_file);
				play_note = false;
				} else {
				current_note = note;
				play_note = true;
				char *str = "MIDI EVENT play_note\n"; fwrite(str, strlen(str), 1, debug_file);
				}
				} else if (status == 0xb0) {
				if (e->midiData[1] == 0x7e || e->midiData[1] == 0x7b) {
				play_note = false;
				char *str = "MIDI EVENT all notes off\n"; fwrite(str, strlen(str), 1, debug_file);
				}
				}*/

				/*char str[64];
				sprintf(str, "MIDI EVENT: status %i \n", status);
				fwrite(str, strlen(str), 1, debug_file);*/
				/*char *str = "MIDI EVENT\n";
				fwrite(str, strlen(str), 1, debug_file);*/
			}
		}
		return 1;
	}

	void process(float **inputs, float **outputs, VstInt32 sample_frames) {
		float *inleft = NULL;
		float *inright = NULL;
		if (inputs) {
			inleft = inputs[0];
			inright = inputs[1];
		}
		Sample *in = (Sample*)malloc(sizeof(Sample)*sample_frames);
		Sample *out = (Sample*)malloc(sizeof(Sample)*sample_frames);
		for (int i = 0; i < sample_frames; ++i) {
			in[i] = {inleft[i], inright[i]};
		}
		synth_go(&devices[curProgram], sample_frames, in, out);
		for (int i = 0; i < sample_frames; ++i) {
			outputs[0][i] = out[i].l;
			outputs[1][i] = out[i].r;
		}
		free(in);
		free(out);
	}

	void processReplacing(float **inputs, float **outputs, VstInt32 sample_frames) {
		float *inleft = NULL;
		float *inright= NULL;
		if (inputs) {
			inleft = inputs[0];
			inright = inputs[1];
		}
		Sample *in = (Sample*)malloc(sizeof(Sample)*sample_frames);
		Sample *out = (Sample*)malloc(sizeof(Sample)*sample_frames);
		if (inputs) {
			for (int i = 0; i < sample_frames; ++i) {
				in[i] = {inleft[i], inright[i]};
			}
		}
		synth_go(&devices[curProgram], sample_frames, in, out);
		for (int i = 0; i < sample_frames; ++i) {
			outputs[0][i] = out[i].l;
			outputs[1][i] = out[i].r;
		}
		free(in);
		free(out);
	}

};

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
	return new SynthVstPlugin(audioMaster);
}
