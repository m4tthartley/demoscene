
#include "w:/demoscene/synth/synth.cpp"

#define VST_TYPE DEVICE_TYPE_FEEDLOOP
#define VST_NAME "Synth Feedloop"
#define VST_TITLE "Matt's Feedloop"
#define VST_INSTRUMENT
const int num_params = FEEDLOOP_PARAM_MAX;
const float *param_defaults = feedloop_param_defaults;
struct ParamInfo {
	char *name;
};
ParamInfo param_info[] = {
	{"Feedback 1"},
	{"Feedback 2"},
	{"Feedback 3"},
};

#include "../vstlib.cpp"
