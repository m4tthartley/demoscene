
#include "w:/demoscene/synth/synth.cpp"

#define VST_TYPE DEVICE_TYPE_NOISE
#define VST_NAME "Synth Noise"
#define VST_TITLE "Matt's Noise"
#define VST_EFFECT
const int num_params = NOISE_PARAM_MAX;
const float *param_defaults = noise_param_defaults;
struct ParamInfo {
	char *name;
};
ParamInfo param_info[] = {
	{"Volume"},
};

#include "../vstlib.cpp"
