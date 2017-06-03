
#define STRUCT_NAME Osc
#define INSTRUMENT
char *param_names[] = {
	"SINE",
	"SAWTOOTH",
	"TRIANGLE",
	"SQUARE",
	"ATTACK",
	"HOLD",
	"RELEASE",
};

#include "../vstlib.cpp"

void init(SynthVstPlugin *vst) {
	vst->title = "OSCAR";

	vst->options.sine = 0.5f;
	vst->options.saw = 0.0f;
	vst->options.tri = 0.0f;
	vst->options.square = 0.0f;
	vst->options.attack = 0.5f;
	vst->options.hold = 0.5f;
	vst->options.release = 0.5f;
}