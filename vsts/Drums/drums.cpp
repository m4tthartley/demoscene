
#define STRUCT_NAME Drums
#define INSTRUMENT
char *param_names[] = {
	"WAVE",
	"PITCH",
	"PITCH_FALLOFF",

	"ATTACK",
	"ATTACK_START",
	"RELEASE",
	"NOISE",

	"PITCH ATTACK",
	"PITCH RELEASE",

	"NOISE ATTACK",
	"NOISE RELEASE",

	"TRIANGLE WAVE",

	"ENVELOPE POW",
	"PITCH ENVELOPE POW",
};

#include "../vstlib.cpp"

void init(SynthVstPlugin *vst) {
	vst->title = "BEATS";

	vst->options.sine_wave = 0.5f;
	vst->options.pitch = 0.5f;
	vst->options.pitch_falloff = 0.5f;
	vst->options.attack = 0.5f;
	vst->options.attack_start = 0.5f;
	vst->options.release = 0.5f;
	vst->options.noise = 0.5f;
	vst->options.pitch_attack = 0.5f;
	vst->options.pitch_release = 0.5f;
	vst->options.noise_attack = 0.5f;
	vst->options.noise_release = 0.5f;
	vst->options.tri = 0.5f;
	vst->options.env_pow = 0.5f;
	vst->options.pitch_env_pow = 0.5f;
}
