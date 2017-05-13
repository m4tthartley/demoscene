
#include "w:/demoscene/synth/synth.cpp"

#define VST_TYPE DEVICE_TYPE_OSC
#define VST_NAME "Synth Osc"
#define VST_TITLE "Matt's Osc"
#define VST_INSTRUMENT
const int num_params = OSC_PARAM_MAX;
const float *param_defaults = osc_param_defaults;
struct ParamInfo {
	char *name;
};
ParamInfo param_info[] = {
	{"Sine wave"},
	{"Sine Attack"},
	{"Sine Sustain"},
	{"Sine Release"},

	{"Sawtooth wave"},
	{"Sawtooth Attack"},
	{"Sawtooth Sustain"},
	{"Sawtooth Release"},
};

#include "../vstlib.cpp"
