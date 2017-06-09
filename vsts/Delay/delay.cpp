
#define STRUCT_NAME Delay
char *param_names[] = {
	"Feedback",
	"Time",
};

#include "../vstlib.cpp"

void init(SynthVstPlugin *vst) {
	vst->title = "CANYON";

	vst->options.feedback = 0.5f;
	vst->options.time = 0.5f;
}