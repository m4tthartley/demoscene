
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <vector>
#include "../vstsdk/audioeffectx.h"
#include "w:/libs/math.c"

//#define DEBUG_PRINT

const int num_outputs = 2;

#define __OPTIONS_STRUCT_NAME(a) a##Options
#define _OPTIONS_STRUCT_NAME(a) __OPTIONS_STRUCT_NAME(a)
#define OPTIONS_STRUCT_NAME _OPTIONS_STRUCT_NAME(STRUCT_NAME)

struct SynthVstPlugin;
void init(SynthVstPlugin *vst);

struct SynthVstPlugin : public AudioEffectX {

	char *title;
	STRUCT_NAME devices[100];
	OPTIONS_STRUCT_NAME options;
	bool instrument = false;

	FILE *debug_file;
	char progname[64];

	SynthVstPlugin(audioMasterCallback audioMaster) :
		AudioEffectX(audioMaster, array_size(devices), array_size(param_names)) {

		init(this);

		memset(devices, 0, sizeof(STRUCT_NAME)*100);
		for (int i = 0; i < array_size(devices); ++i) {
			devices[i].options = options;
		}

		strcpy(progname, title);

#ifdef DEBUG_PRINT
		char file_name[64] = "w:/demoscene/vsts/";
		strcat(file_name, title);
		strcat(file_name, " debug.txt");
		debug_file = fopen(file_name/*"w:/demoscene/vsts/" VST_NAME " debug.txt"*/, "w");
#endif

		if (audioMaster) {
			if (instrument) {
				setNumInputs(0);
			} else {
				setNumInputs(2);
			}

			setNumOutputs(num_outputs);
			canProcessReplacing();

			if (instrument) {
				isSynth();
			}

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

	bool getEffectName(char* name) { strncpy(name, title, kVstMaxEffectNameLen); return true; }
	bool getVendorString(char* text) { strncpy(text, "GIANTJELLY", kVstMaxVendorStrLen); return true; }
	bool getProductString(char* text) { strncpy(text, title, kVstMaxProductStrLen); return true; }

	void getParameterName(VstInt32 index, char* label) {
		strcpy(label, param_names[index]);
	}

	float getParameter(VstInt32 index) {
		return ((float*)&devices[curProgram].options)[index];
	}

	void setParameter(VstInt32 index, float value) {
		((float*)&devices[curProgram].options)[index] = value;
	}

	void getParameterDisplay(VstInt32 index, char *text) {
		sprintf(text, "%.2f", ((float*)&devices[curProgram].options)[index]);
	}

	void getParameterLabel(VstInt32 index, char *label) {}
	VstInt32 getNumMidiInputChannels() { return 1; }
	VstInt32 getNumMidiOutputChannels() { return 0; }

	VstInt32 processEvents(VstEvents* events) {
		for (int i = 0; i < events->numEvents; ++i) {
			if (events->events[i]->type == kVstMidiType) {

				VstMidiEvent *e = (VstMidiEvent*)events->events[i];
				int status = e->midiData[0] & 0xf0;

				VstInt32 note = e->midiData[1] & 0x7f;
				VstInt32 velocity = e->midiData[2] & 0x7f;
				// debug_print("status 0x%x, velocity %i \n", status, velocity);

				if (status == 0x90 || status == 0x80) {
					if (status == 0x90 && velocity != 0) {
						devices[curProgram].notes[note].on();
					} else {
						devices[curProgram].notes[note].off();
					}
				}
			}
		}
		return 1;
	}

	virtual void process(float **inputs, float **outputs, int samples) {
		devices[curProgram].process(inputs, outputs, samples);
	}

	void processReplacing(float **inputs, float **outputs, VstInt32 sample_frames) {
		//synth_go(&devices[curProgram], sample_frames, inputs, outputs);
		process(inputs, outputs, sample_frames);
	}

};

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
	return new SynthVstPlugin(audioMaster);
}
