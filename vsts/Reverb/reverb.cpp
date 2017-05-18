
#include "w:/demoscene/synth/synth.cpp"

#define VST_TYPE DEVICE_TYPE_REVERB
#define VST_NAME "Synth Reverb"
#define VST_TITLE "Matt's Reverb"
#define VST_EFFECT
const int num_params = REVERB_PARAM_MAX;
const float *param_defaults = reverb_param_defaults;
struct ParamInfo {
	char *name;
};
ParamInfo param_info[] = {
	{"GAIN"},
	{"DAMP"},
	{"LOWCUT"},
	//{"Wet"},
	//{"Dry"},
	//{"Feedback"},
	//{"Room size"},
	//{"Room offset"},
};

#include "../vstlib.cpp"
