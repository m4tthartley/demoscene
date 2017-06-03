
#define STRUCT_NAME Noise
char *param_names[] = {
	"Volume",
};

#include "../vstlib.cpp"

void init(SynthVstPlugin *vst) {
	vst->title = "NOISE";
	vst->options.volume = 0.1f;
}