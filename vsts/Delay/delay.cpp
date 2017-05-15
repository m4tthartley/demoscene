
#include "w:/demoscene/synth/synth.cpp"

#define VST_TYPE DEVICE_TYPE_DELAY
#define VST_NAME "Synth Delay"
#define VST_TITLE "Matt's Delay"
#define VST_EFFECT
const int num_params = DELAY_PARAM_MAX;
const float *param_defaults = delay_param_defaults;
struct ParamInfo {
	char *name;
};
ParamInfo param_info[] = {
	{"Feedback"},
	{"Time"},
};

#include "../vstlib.cpp"
