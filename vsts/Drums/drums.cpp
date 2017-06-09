
#define STRUCT_NAME Drums
#define INSTRUMENT
char *param_names[] = {
	"SINE WAVE",
	"TRIANGLE WAVE",

	"ATTACK",
	"RELEASE",
	"ENVELOPE POW",

	"PITCH",
	"PITCH_BEND",
	"PITCH ATTACK",
	"PITCH RELEASE",
	"PITCH ENVELOPE POW",

	"NOISE",
	"NOISE ATTACK",
	"NOISE RELEASE",
	"NOISE ENVELOPE POW",
};

#include "../vstlib.cpp"

void init(SynthVstPlugin *vst) {
	vst->title = "BEATS";

	vst->options.sine_wave = 0.5f;
	vst->options.tri_wave = 0.0f;

	vst->options.attack = 0.1f;
	vst->options.release = 0.5f;
	vst->options.env_pow = 0.5f;

	vst->options.pitch = 0.25f;
	vst->options.pitch_bend = 0.5f;
	vst->options.pitch_attack = 0.0f;
	vst->options.pitch_release = 0.1f;
	vst->options.pitch_env_pow = 0.5f;

	vst->options.noise = 0.0f;
	vst->options.noise_attack = 0.0f;
	vst->options.noise_release = 0.0f;
	vst->options.noise_env_pow = 0.5f;
}
