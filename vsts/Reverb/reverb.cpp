
#define STRUCT_NAME Reverb
char *param_names[] = {
	"GAIN",
	"DAMP",
	"LOWCUT",
};

#include "../vstlib.cpp"

void init(SynthVstPlugin *vst) {
	vst->title = "CATHEDRAL";

	vst->options.gain = 0.5f;
	vst->options.damp = 0.5f;
	vst->options.lowcut = 0.5f;
}