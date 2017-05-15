
#include "w:/demoscene/synth/synth.cpp"

#define VST_TYPE DEVICE_TYPE_LOWPASS
#define VST_NAME "Synth Lowpass"
#define VST_TITLE "Matt's Lowpass"
#define VST_EFFECT
const int num_params = LOWPASS_PARAM_MAX;
const float *param_defaults = lowpass_param_defaults;
struct ParamInfo {
	char *name;
};
ParamInfo param_info[] = {
	{"Resonance"},
	{"Freq"},
};

#include "../vstlib.cpp"
