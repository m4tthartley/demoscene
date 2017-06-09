
#define STRUCT_NAME SineFeedback
#define INSTRUMENT
char *param_names[] = {
	"ATTACK",
	"RELEASE",
	"FEEDBACK 1",
	"FEEDBACK 2",
	"FEEDBACK 3",
};

#include "../vstlib.cpp"

void init(SynthVstPlugin *vst) {
	vst->title = "SINE FEEDBACK";
	
	vst->options.attack = 0.5f;
	vst->options.release = 0.5f;
	vst->options.feedback1 = 1.0f;
	vst->options.feedback2 = 1.0f;
	vst->options.feedback3 = 1.0f;
}