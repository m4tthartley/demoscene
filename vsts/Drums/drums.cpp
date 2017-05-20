
#include "w:/demoscene/synth/synth.cpp"

#define VST_TYPE DEVICE_TYPE_BEATIT
#define VST_NAME "BEATS"
#define VST_TITLE "BEATS"
#define VST_INSTRUMENT
const int num_params = DRUM_PARAM_MAX;
const float *param_defaults = drum_param_defaults;
struct ParamInfo {
	char *name;
};
ParamInfo param_info[] = {
	{"WAVE"},
	{"PITCH"},
	{"PITCH_FALLOFF"},

	{"ATTACK"},
	{"ATTACK_START"},
	{"RELEASE"},
	{"NOISE"},

	{"PITCH ATTACK"},
	{"PITCH RELEASE"},

	{"NOISE ATTACK"},
	{"NOISE RELEASE"},
};

#include "../vstlib.cpp"
