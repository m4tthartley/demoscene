
#define STRUCT_NAME Filter
char *param_names[] = {
	"Resonance",
	"Freq",
	"HighFreq",
};

#include "../vstlib.cpp"

void init(SynthVstPlugin *vst) {
	vst->title = "BRITA";

	vst->options.rez = 0.5f;
	vst->options.low_freq = 0.5f;
	vst->options.high_freq = 0.0f;
}