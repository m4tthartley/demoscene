
#include "w:/demoscene/synth/synth.cpp"

#define VST_TYPE DEVICE_TYPE_BEATIT
#define VST_NAME "BEAT IT"
#define VST_TITLE "BEAT IT"
#define VST_INSTRUMENT
const int num_params = BEATIT_PARAM_MAX;
const float *param_defaults = beatit_param_defaults;
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
};

#include "../vstlib.cpp"
