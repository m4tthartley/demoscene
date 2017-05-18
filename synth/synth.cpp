
#include "w:/libs/math.c"

#pragma warning(disable: 4244)

// todo: have this is general lib
#define array_size(arr) (sizeof(arr)/sizeof(arr[0]))

typedef int SampleOffset;

#define NOTES_MAX 128
const SampleOffset sample_rate = 44100;
const int delay_line_size = (int)((double)sample_rate / 5.0);
const int delay_line_size2 = (int)((double)sample_rate / 10.0);

enum OscParams {
	OSC_PARAM_SINE_WAVE,
	OSC_PARAM_SINE_ATTACK,
	OSC_PARAM_SINE_SUSTAIN,
	OSC_PARAM_SINE_RELEASE,

	OSC_PARAM_SAW_WAVE,
	OSC_PARAM_SAW_ATTACK,
	OSC_PARAM_SAW_SUSTAIN,
	OSC_PARAM_SAW_RELEASE,

	OSC_PARAM_MAX,
};
float osc_param_defaults[] = {
	0.5f, 0.5f, 0.5f, 0.5f,
	0.0f, 0.5f, 0.5f, 0.5f,
};
enum NoiseParams {
	NOISE_PARAM_VOLUME,

	NOISE_PARAM_MAX,
};
float noise_param_defaults[] = {
	0.1f,
};
enum ReverbParams {
	/*REVERB_PARAM_DELAY,
	REVERB_PARAM_DECAY,*/
	REVERB_PARAM_GAIN,
	REVERB_PARAM_DAMP,
	REVERB_PARAM_LOWCUT,
	//REVERB_PARAM_WET,
	//REVERB_PARAM_DRY,
	//REVERB_PARAM_FEEDBACK,
	
	//REVERB_PARAM_ROOM_SIZE,
	//REVERB_PARAM_ROOM_OFFSET,
	

	REVERB_PARAM_MAX,
};
float reverb_param_defaults[] = {
	0.5f,
	0.5f,
	0.5f,
	/*0.015f,
	1.0f,
	0.0f,
	0.5f,
	0.2f,
	0.14f,
	0.7f,*/
};
enum FeedloopParams {
	FEEDLOOP_PARAM_FEEDBACK1,
	FEEDLOOP_PARAM_FEEDBACK2,
	FEEDLOOP_PARAM_FEEDBACK3,

	FEEDLOOP_PARAM_MAX,
};
float feedloop_param_defaults[] = {
	0.5f,
	0.5f,
	0.5f,
};
enum LowpassParams {
	LOWPASS_PARAM_REZ,
	LOWPASS_PARAM_FREQ,

	LOWPASS_PARAM_MAX,
};
float lowpass_param_defaults[] = {
	0.5f,
	0.5f,
};
enum DelayParams {
	DELAY_PARAM_FEEDBACK,
	DELAY_PARAM_TIME,

	DELAY_PARAM_MAX,
};
float delay_param_defaults[] = {
	0.5f,
	0.5f,
};

enum DeviceTypes {
	DEVICE_TYPE_OSC,
	DEVICE_TYPE_NOISE,
	DEVICE_TYPE_REVERB,
	DEVICE_TYPE_FEEDLOOP,
	DEVICE_TYPE_LOWPASS,
	DEVICE_TYPE_DELAY,
};

enum NoteStage {
	ENVELOPE_OFF,
	ENVELOPE_ATTACK,
	ENVELOPE_HOLD,
	ENVELOPE_RELEASE,
};
float default_stage_times[] = {
	0.0f,
	0.1f,
	1.0f,
	0.5f,
};
struct Note {
	NoteStage stage;
	SampleOffset cursor;
	SampleOffset next_stage;
	float sine;
	float saw;
};

struct Sample {
	union {
		struct {
			float left;
			float right;
		};
		struct {
			float l;
			float r;
		};
	};
};

#define MAX_PARAMS 16
struct SynthDevice {
	DeviceTypes type;
	float params[MAX_PARAMS]; // note: max 16 parameters
	Note notes[NOTES_MAX];
	float rolling_sample_buffer[40];
	int rolling_sample_index;
	/*float delay_line[delay_line_size];
	int delay_index;
	float delay_line2[delay_line_size2];
	int delay_index2;*/

	Sample filter_in_buffer[10];
	int filter_index; 
	Sample filter_out_buffer[10];

	Sample delay_line[sample_rate];
	int delay_index;
};

struct SynthChannel {
	SynthDevice devices[16]; // note: max 16 devices
};

//float synth_sine_mix = 0.5f;
//float synth_sine_attack = 0.1f;
//float synth_sine_sustain = 0.5f;
//float synth_sine_release = 0.5f;
//float synth_saw_mix = 0.0f;
//float synth_saw_attack = 0.1f;
//float synth_saw_sustain = 0.5f;
//float synth_saw_release = 0.5f;
//float synth_volume = 0.5f;

void set_note_stage(SynthDevice *device, int note, NoteStage stage) {
	device->notes[note].stage = stage;
	device->notes[note].cursor = 0;
	float time = 0.0f;
	switch (stage) {
	case ENVELOPE_ATTACK:
		time = device->params[OSC_PARAM_SINE_ATTACK];
		break;
	case ENVELOPE_RELEASE:
		time = device->params[OSC_PARAM_SINE_RELEASE];
		break;
	}
	device->notes[note].next_stage = time*(float)sample_rate;
}

void synth_note_on(SynthDevice *device, int note) {
	set_note_stage(device, note, ENVELOPE_ATTACK);
}

void synth_note_off(SynthDevice *device, int note) {
	set_note_stage(device, note, ENVELOPE_RELEASE);
}

void synth_init() {

}

struct Comb {
	float *buffer;
	int size;
	int index;
	float filter;
};

struct Allpass {
	float *buffer;
	int size;
	int index;
};

//float offsetroom = 0.7f;
//float initialroom = 0.14f;
//float initialdamp = 0.2f;
//float fixedgain = 0.015f;
//float wet = 1.0f;
//float dry = 0.0f;
//float allpass_feedback = 0.5f;

Comb init_comb(int size) {
	Comb result = {};
	result.buffer = (float*)calloc(size, sizeof(float));
	result.size = size;
	return result;
}

Allpass init_allpass(int size) {
	Allpass result = {};
	result.buffer = (float*)calloc(size, sizeof(float));
	result.size = size;
	return result;
}

Comb comb1_l = init_comb(1116);
Comb comb1_r = init_comb(1116 + 23);

Comb comb2_l = init_comb(1188);
Comb comb2_r = init_comb(1188 + 23);

Comb comb3_l = init_comb(1277);
Comb comb3_r = init_comb(1277 + 23);

Comb comb4_l = init_comb(1356);
Comb comb4_r = init_comb(1356 + 23);

Comb comb5_l = init_comb(1422);
Comb comb5_r = init_comb(1422 + 23);

Comb comb6_l = init_comb(1491);
Comb comb6_r = init_comb(1491 + 23);

Comb comb7_l = init_comb(1557);
Comb comb7_r = init_comb(1557 + 23);

Comb comb8_l = init_comb(1617);
Comb comb8_r = init_comb(1617 + 23);

Allpass allpass1_l = init_allpass(556);
Allpass allpass1_r = init_allpass(556 + 23);

Allpass allpass2_l = init_allpass(441);
Allpass allpass2_r = init_allpass(441 + 23);

Allpass allpass3_l = init_allpass(341);
Allpass allpass3_r = init_allpass(341 + 23);

Allpass allpass4_l = init_allpass(225);
Allpass allpass4_r = init_allpass(225 + 23);

//Comb comb1_l = {(float*)malloc(sizeof(float) * (1116)), (1116)};
//Comb comb1_r = {(float*)malloc(sizeof(float) * (1116 + 23)), (1116 + 23)};
//
//Comb comb2_l = {(float*)malloc(sizeof(float) * (1188)), (1188)};
//Comb comb2_r = {(float*)malloc(sizeof(float) * (1188 + 23)), (1188 + 23)};
//
//Comb comb3_l = {(float*)malloc(sizeof(float) * (1277)), (1277)};
//Comb comb3_r = {(float*)malloc(sizeof(float) * (1277 + 23)), (1277 + 23)};
//
//Comb comb4_l = {(float*)malloc(sizeof(float) * (1356)), (1356)};
//Comb comb4_r = {(float*)malloc(sizeof(float) * (1356 + 23)), (1356 + 23)};
//
//Comb comb5_l = {(float*)malloc(sizeof(float) * (1422)), (1422)};
//Comb comb5_r = {(float*)malloc(sizeof(float) * (1422 + 23)), (1422 + 23)};
//
//Comb comb6_l = {(float*)malloc(sizeof(float) * (1491)), (1491)};
//Comb comb6_r = {(float*)malloc(sizeof(float) * (1491 + 23)), (1491 + 23)};
//
//Comb comb7_l = {(float*)malloc(sizeof(float) * (1557)), (1557)};
//Comb comb7_r = {(float*)malloc(sizeof(float) * (1557 + 23)), (1557 + 23)};
//
//Comb comb8_l = {(float*)malloc(sizeof(float) * (1617)), (1617)};
//Comb comb8_r = {(float*)malloc(sizeof(float) * (1617 + 23)), (1617 + 23)};
//
//Allpass allpass1_l = {(float*)malloc(sizeof(float) * (556)), (556)};
//Allpass allpass1_r = {(float*)malloc(sizeof(float) * (556 + 23)), (556 + 23)};
//
//Allpass allpass2_l = {(float*)malloc(sizeof(float) * (441)), (441)};
//Allpass allpass2_r = {(float*)malloc(sizeof(float) * (441 + 23)), (441 + 23)};
//
//Allpass allpass3_l = {(float*)malloc(sizeof(float) * (341)), (341)};
//Allpass allpass3_r = {(float*)malloc(sizeof(float) * (341 + 23)), (341 + 23)};
//
//Allpass allpass4_l = {(float*)malloc(sizeof(float) * (225)), (225)};
//Allpass allpass4_r = {(float*)malloc(sizeof(float) * (225 + 23)), (225 + 23)};

//float comb_process(SynthDevice *device, Comb *comb, float in) {
//	float feedback = (device->params[REVERB_PARAM_ROOM_SIZE]) + device->params[REVERB_PARAM_ROOM_OFFSET];
//	float damp1 = device->params[REVERB_PARAM_DAMP];
//	float damp2 = 1.0f-damp1;
//
//	float out = comb->buffer[comb->index];
//	comb->filter = (out*damp2) + (comb->filter*damp1);
//	comb->buffer[comb->index++] = in + (comb->filter*feedback);
//	if (comb->index >= comb->size) comb->index = 0;
//	return out;
//}
//
//float allpass_process(SynthDevice *device, Allpass *allpass, float in) {
//	float b = allpass->buffer[allpass->index];
//	float out = -in + b;
//	allpass->buffer[allpass->index++] = in + (b*device->params[REVERB_PARAM_FEEDBACK]);
//	if (allpass->index >= allpass->size) allpass->index = 0;
//	return out;
//}

//Sample allpass_filter(SynthDevice *device, Sample in, float gain, float d) {
//	Sample result = {};
//
//	int index = device->delay_index + ((float)array_size(device->delay_line)*d);
//	if (index >= array_size(device->delay_line)) index -= array_size(device->delay_line);
//	float delay = device->delay_line[index];
//
//	result.left = delay - (in.left * gain);
//	result.right = delay - (in.right * gain);
//	device->delay_line[device->delay_index] = in.left + (device->delay_line[device->delay_index]*gain);
//
//	return result;
//}

struct DelayLineMono {
	float *buf;
	int pos;
	int size;

	void init(int s) {
		if (!buf) {
			size = s;
			buf = (float*)calloc(size, sizeof(float));
			pos = 0;
		}
	}
};

struct DelayLine {
	DelayLineMono l;
	DelayLineMono r;

	void init(int lsize, int rsize) {
		l.init(lsize);
		r.init(rsize);
	}

	/*DelayLine(int lsize, int rsize) {
		init(lsize, rsize);
	}*/
};

DelayLine comb[4];
DelayLine allpass[4];

void synth_go(SynthDevice *device, SampleOffset samples, Sample *in, Sample *out) {
	//static float sine = 0.0f;

	for (int i = 0; i < samples; ++i) {
		out[i] = {0.0f, 0.0f};

		for (int j = 0; j < NOTES_MAX; ++j) {
			Note *note = &device->notes[j];
			float env_vol = 1.0f;

			if (note->stage != ENVELOPE_OFF && note->stage != ENVELOPE_HOLD) {
				if (note->cursor >= note->next_stage) {
					if (note->stage == ENVELOPE_RELEASE) {
						set_note_stage(device, j, ENVELOPE_OFF);
					} else {
						set_note_stage(device, j, (NoteStage)(note->stage+1));
					}
					note->cursor -= note->next_stage;
				}
				++note->cursor;
			}

			switch (note->stage) {
			case ENVELOPE_OFF:
				continue;
				break;
			case ENVELOPE_ATTACK:
				env_vol = (float)note->cursor / (float)note->next_stage;
				break;
			case ENVELOPE_HOLD:
				break;
			case ENVELOPE_RELEASE:
				env_vol = 1.0f - ((float)note->cursor / (float)note->next_stage);
				break;
			}
			
			if (device->type == DEVICE_TYPE_OSC) {
				// from http://conspiracy.hu/articles/8/
				// pitch_in_hz=440.0*pow(2.0,(note_number-45)/12.0);
				float hz = 261.6255653006f *powf(2.0f, ((float)j-48.0f)/12.0f);

				//double pitch = 440.0 * pow(2.0, (double)(j-45)/12.0);
				note->sine += (hz / (float)sample_rate) * PI2;
				//note->sine += (pitch / (double)sample_rate) * PI2;
				if (note->sine > PI2) note->sine -= PI2;

				note->saw += (hz / (float)sample_rate) * 2;
				if (note->saw >= 1.0f) note->saw -= 2.0f;

				out[i].l += (sinf(note->sine) * device->params[OSC_PARAM_SINE_WAVE]) * env_vol /** params[PARAM_VOLUME].value*/;
				out[i].r += (sinf(note->sine) * device->params[OSC_PARAM_SINE_WAVE]) * env_vol /** params[PARAM_VOLUME].value*/;
				out[i].l += (note->saw * device->params[OSC_PARAM_SAW_WAVE]) * env_vol;
				out[i].r += (note->saw * device->params[OSC_PARAM_SAW_WAVE]) * env_vol;

				/*if (note->stage != ENVELOPE_OFF) {
					float env_vol = 1.0f;
					if (note->stage != ENVELOPE_ATTACK)
				}*/

				/*float progress = notes[j].cursor/params[notes[j].stage].value;
				if (progress >= 1.0f) {
					notes[j].cursor -= params[notes[j].stage].value;
					++notes[j].stage;
					if (notes[j].stage > 3) {
						notes.erase(notes.begin()+j);
						continue;
					}
				}*/

				//float vol = stages[notes[j].stage] + (stages[notes[j].stage+1]-stages[notes[j].stage])*progress;
				//if (note->stage != ENVELOPE_OFF) {

				//}
				/*if (notes[j].cursor >= 1.0f) {
				notes.erase(notes.begin()+j);
				}*/
			}

			
		}

		if (device->type == DEVICE_TYPE_NOISE) {
			/**left = *inleft + (rand_float() * device->params[NOISE_PARAM_VOLUME]);
			*right = *inright + (rand_float() * device->params[NOISE_PARAM_VOLUME]);*/

			/*float s = 40.0f;
			int index = min(max(i, s), samples-(s+1));
			float avg = 0.0f;
			avg += left[index];
			for (int j = 0; j < s; ++j) {
				avg += left[index-j];
				avg += left[index+j];
			}
			avg /= s*2.0f + 1.0f;*/
			/*avg += left[index-1];
			avg += left[index-2];
			avg += left[index-3];
			avg += left[index-4];
			avg += left[index+1];
			avg += left[index+2];
			avg += left[index+3];
			avg += left[index+4];
			avg /= 9.0f;*/

			float avg = 0.0f;
			for (int j = 0; j < array_size(device->rolling_sample_buffer); ++j) {
				avg += fabs(device->rolling_sample_buffer[j]);
			}
			avg /= (float)array_size(device->rolling_sample_buffer);

			static float smooth = 0.0f;
			smooth = smooth - (0.025f * (smooth - in[i].l));

			out[i].l = in[i].l + (smooth * (rand_float()*2.0f-1.0f));
			out[i].r = in[i].r + (smooth * (rand_float()*2.0f-1.0f));
			//left[i] = inleft[i] + (avg /** (rand_float()*2.0f-1.0f)*/ * device->params[NOISE_PARAM_VOLUME]);
			//right[i] = inright[i] + (avg /** (rand_float()*2.0f-1.0f)*/ * device->params[NOISE_PARAM_VOLUME]);

			device->rolling_sample_buffer[device->rolling_sample_index++] = in[i].l;
			if (device->rolling_sample_index >= array_size(device->rolling_sample_buffer)) device->rolling_sample_index -= array_size(device->rolling_sample_buffer);
		}

		if (device->type == DEVICE_TYPE_REVERB) {
			comb[0].init(1309, 1327);
			comb[1].init(1635, 1631);
			comb[2].init(1811, 1833);
			comb[3].init(1926, 1901);
			allpass[0].init(220, 205);
			allpass[1].init(74, 77);
			static Sample combl[4] = {};
			static Sample lowcut_filter = {};

			float damp = device->params[REVERB_PARAM_DAMP];
			float gain = device->params[REVERB_PARAM_GAIN];
			float lowcut = device->params[REVERB_PARAM_LOWCUT];

			float comb_gain[] = {
				0.97f,
				0.96f,
				0.95f,
				0.94f,
			};
			float allpass_gain[] = {
				0.93f,
				0.97f,
			};

			Sample inpt = {in[i].l * gain, in[i].r * gain};
			Sample otpt = {};
			for (int k = 0; k < 4; ++k) {
				float dv = comb_gain[k] * comb[k].l.buf[comb[k].l.pos];
				float nv = (k&1) ? (dv-inpt.l) : (dv+inpt.l);
				float lp = damp * nv;
				comb[k].l.buf[comb[k].l.pos] = lp;
				if (++comb[k].l.pos == comb[k].l.size) comb[k].l.pos = 0;
				otpt.l += lp;

				dv = comb_gain[k] * comb[k].r.buf[comb[k].r.pos];
				nv = (k&1) ? (dv-inpt.r) : (dv+inpt.r);
				lp = damp * nv;
				comb[k].r.buf[comb[k].r.pos] = lp;
				if (++comb[k].r.pos == comb[k].r.size) comb[k].r.pos = 0;
				otpt.r += lp;
			}
			for (int k = 0; k < 2; ++k) {
				float dv = allpass[k].l.buf[allpass[k].l.pos];
				float dz = otpt.l + allpass_gain[k] * dv;
				allpass[k].l.buf[allpass[k].l.pos] = dz;
				if (++allpass[k].l.pos == allpass[k].l.size) allpass[k].l.pos = 0;
				otpt.l = dv - allpass_gain[k] * dz;

				dv = allpass[k].r.buf[allpass[k].r.pos];
				dz = otpt.r + allpass_gain[k] * dv;
				allpass[k].r.buf[allpass[k].r.pos] = dz;
				if (++allpass[k].r.pos == allpass[k].r.size) allpass[k].r.pos = 0;
				otpt.r = dv - allpass_gain[k] * dz;
			}

			lowcut_filter.l += lowcut * (otpt.l - lowcut_filter.l);
			lowcut_filter.r += lowcut * (otpt.r - lowcut_filter.r);
			out[i].l = otpt.l - lowcut_filter.l;
			out[i].r = otpt.r - lowcut_filter.r;

#if 0
			float wet1 = device->params[REVERB_PARAM_WET]*(1.0f/2.0f + 0.5f);
			float wet2 = device->params[REVERB_PARAM_WET]*((1.0f-1.0f)/2.0f);

			float outl = 0.0f;
			float outr = 0.0f;
			float inmix = (in[i].l + in[i].r) * device->params[REVERB_PARAM_GAIN];

			outl += comb_process(device, &comb1_l, inmix);
			outr += comb_process(device, &comb1_r, inmix);
			outl += comb_process(device, &comb2_l, inmix);
			outr += comb_process(device, &comb2_r, inmix);
			outl += comb_process(device, &comb3_l, inmix);
			outr += comb_process(device, &comb3_r, inmix);
			outl += comb_process(device, &comb4_l, inmix);
			outr += comb_process(device, &comb4_r, inmix);
			outl += comb_process(device, &comb5_l, inmix);
			outr += comb_process(device, &comb5_r, inmix);
			outl += comb_process(device, &comb6_l, inmix);
			outr += comb_process(device, &comb6_r, inmix);
			outl += comb_process(device, &comb7_l, inmix);
			outr += comb_process(device, &comb7_r, inmix);
			outl += comb_process(device, &comb8_l, inmix);
			outr += comb_process(device, &comb8_r, inmix);

			outl = allpass_process(device, &allpass1_l, outl);
			outr = allpass_process(device, &allpass1_r, outr);
			outl = allpass_process(device, &allpass2_l, outl);
			outr = allpass_process(device, &allpass2_r, outr);
			outl = allpass_process(device, &allpass3_l, outl);
			outr = allpass_process(device, &allpass3_r, outr);
			outl = allpass_process(device, &allpass4_l, outl);
			outr = allpass_process(device, &allpass4_r, outr);

			out[i].l = outl*wet1 + outr*wet2 + in[i].l*(device->params[REVERB_PARAM_DRY]*2.0f);
			out[i].r = outr*wet1 + outl*wet2 + in[i].r*(device->params[REVERB_PARAM_DRY]*2.0f);
#endif

#if 0
			float gain = 1.0f;

			int index = device->delay_index + ((float)array_size(device->delay_line) * device->params[REVERB_PARAM_DELAY]);
			if (index >= array_size(device->delay_line)) index -= array_size(device->delay_line);
			int index2 = index-1;
			int index3 = index-2;
			if (index2 < 0) index2 += array_size(device->delay_line);
			if (index3 < 0) index2 += array_size(device->delay_line);
			float delay = (device->delay_line[index] + device->delay_line[index2] + device->delay_line[index3]) / 3.0f;
			/*int index2 = device->delay_index2 + ((float)array_size(device->delay_line2) * device->params[REVERB_PARAM_DELAY]);
			if (index2 >= array_size(device->delay_line2)) index2 -= array_size(device->delay_line2);
			float delay2 = device->delay_line2[index2];*/

			left[i] = inleft[i] + delay;
			right[i] = inright[i] + delay;
			device->delay_line[device->delay_index] = inleft[i] + (device->delay_line[device->delay_index] * device->params[REVERB_PARAM_DECAY]);
			//device->delay_line2[device->delay_index2] = inleft[i] + (device->delay_line2[device->delay_index2] * device->params[REVERB_PARAM_DECAY]);

			/*Sample s = allpass_filter(device, {inleft[i], inright[i]}, gain, 0.5f);
			s = allpass_filter(device, s, gain, 0.6f);*/
			/*s = allpass_filter(device, s, gain, 0.7f);
			s = allpass_filter(device, s, gain, 0.8f);*/

			/*left[i] = s.left;
			right[i] = s.right;*/



			/*left[i] = delay - (inleft[i] * gain);
			right[i] = delay - (inright[i] * gain);
			device->delay_line[device->delay_index] = inleft[i] + (device->delay_line[device->delay_index]*gain);

			left[i] = delay - (left[i] * gain);
			right[i] = delay - (right[i] * gain);
			device->delay_line[device->delay_index] = left[i] + (device->delay_line[device->delay_index]*gain);*/
			
			++device->delay_index;
			if (device->delay_index >= array_size(device->delay_line)) {
				device->delay_index -= array_size(device->delay_line);
			}
#endif
		}

		if (device->type == DEVICE_TYPE_LOWPASS) {
			if (device->params[LOWPASS_PARAM_FREQ] < 0.01) device->params[LOWPASS_PARAM_FREQ] = 0.001;

			float r = 0.1f + device->params[LOWPASS_PARAM_REZ]*(sqrt(2.0f)-0.1f);
			float f = powf(device->params[LOWPASS_PARAM_FREQ], 2.0f) * (float)(sample_rate/4.0f);
			float c = 1.0f / tanf(PI * f / (float)sample_rate);
			float a1 = 1.0f / (1.0f + r*c + c*c);
			float a2 = 2.0f*a1;
			float a3 = a1;
			float b1 = 2.0f * (1.0f - c*c) * a1;
			float b2 = (1.0f - r*c + c*c) * a1;

			int index1 = device->filter_index-1;
			if (index1 < 0) index1 += array_size(device->filter_in_buffer);
			int index2 = device->filter_index-2;
			if (index2 < 0) index2 += array_size(device->filter_in_buffer);
			Sample in_1 = device->filter_in_buffer[index1];
			Sample in_2 = device->filter_in_buffer[index2];
			Sample out_1 = device->filter_out_buffer[index1];
			Sample out_2 = device->filter_out_buffer[index2];

			Sample o = {
				a1*in[i].l + a2*in_1.l + a3*in_2.l - b1*out_1.l - b2*out_2.l,
				a1*in[i].r + a2*in_1.r + a3*in_2.r - b1*out_1.r - b2*out_2.r,
			};

			device->filter_in_buffer[device->filter_index] = in[i];
			device->filter_out_buffer[device->filter_index] = o;
			if (++device->filter_index >= array_size(device->filter_in_buffer)) device->filter_index = 0;

			out[i] = o;
		}

		if (device->type == DEVICE_TYPE_DELAY) {
			int index = device->delay_index - (1 + (float)array_size(device->delay_line)*device->params[DELAY_PARAM_TIME]);
			if (index < 0) index += array_size(device->delay_line);

			device->delay_line[index].l *= device->params[DELAY_PARAM_FEEDBACK];
			device->delay_line[index].r *= device->params[DELAY_PARAM_FEEDBACK];
			out[i] = {in[i].l + device->delay_line[index].l,
					  in[i].r + device->delay_line[index].r};

			device->delay_line[device->delay_index].l += in[i].l;
			device->delay_line[device->delay_index].r += in[i].r;
			if (++device->delay_index >= array_size(device->delay_line)) device->delay_index = 0;
		}

		if (device->type == DEVICE_TYPE_FEEDLOOP) {
			for (int j = 0; j < NOTES_MAX; ++j) {
				Note *note = &device->notes[j];
				float env_vol = 1.0f;

				if (note->stage != ENVELOPE_OFF && note->stage != ENVELOPE_HOLD) {
					if (note->cursor >= note->next_stage) {
						if (note->stage == ENVELOPE_RELEASE) {
							set_note_stage(device, j, ENVELOPE_OFF);
						} else {
							set_note_stage(device, j, (NoteStage)(note->stage+1));
						}
						note->cursor -= note->next_stage;
					}
					++note->cursor;
				}

				switch (note->stage) {
				case ENVELOPE_OFF:
					continue;
					break;
				case ENVELOPE_ATTACK:
					env_vol = (float)note->cursor / (float)note->next_stage;
					break;
				case ENVELOPE_HOLD:
					break;
				case ENVELOPE_RELEASE:
					env_vol = 1.0f - ((float)note->cursor / (float)note->next_stage);
					break;
				}

				float pitch = 440.0f * powf(2.0f, (float)(j-45)/12.0f);
				note->sine += (pitch / (float)sample_rate) * PI2;
				if (note->sine > PI2) note->sine -= PI2;

				float s1 = sinf(sinf(note->sine) * note->sine);
				float s2 = sinf(sinf(s1 * note->sine) * note->sine);
				out[i].l += s2 * 0.5f * env_vol;
				out[i].r += s2 * 0.5f * env_vol;
			}
		}

		/*++inleft;
		++inright;
		++left;
		++right;*/
	}

	/*if (device->type == DEVICE_TYPE_OSC) {
		for (int i = 0; i < samples; ++i) {
			*left += rand_float() * device->params[NOISE_PARAM_VOLUME];
			*right += rand_float() * device->params[NOISE_PARAM_VOLUME];
			++left;
			++right;
		}
	}*/
}
