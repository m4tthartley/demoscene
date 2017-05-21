#version 330

uniform sampler2D rt_tex;
uniform vec2 screen_res;
in vec2 screen_pos;
out vec3 frag;

void main() {
	// vec2 poisson[64];
	// poisson[0] = vec2(-0.613392, 0.617481);
	// poisson[1] = vec2(0.170019, -0.040254);
	// poisson[2] = vec2(-0.299417, 0.791925);
	// poisson[3] = vec2(0.645680, 0.493210);
	// poisson[4] = vec2(-0.651784, 0.717887);
	// poisson[5] = vec2(0.421003, 0.027070);
	// poisson[6] = vec2(-0.817194, -0.271096);
	// poisson[7] = vec2(-0.705374, -0.668203);
	// poisson[8] = vec2(0.977050, -0.108615);
	// poisson[9] = vec2(0.063326, 0.142369);
	// poisson[10] = vec2(0.203528, 0.214331);
	// poisson[11] = vec2(-0.667531, 0.326090);
	// poisson[12] = vec2(-0.098422, -0.295755);
	// poisson[13] = vec2(-0.885922, 0.215369);
	// poisson[14] = vec2(0.566637, 0.605213);
	// poisson[15] = vec2(0.039766, -0.396100);
	// poisson[16] = vec2(0.751946, 0.453352);
	// poisson[17] = vec2(0.078707, -0.715323);
	// poisson[18] = vec2(-0.075838, -0.529344);
	// poisson[19] = vec2(0.724479, -0.580798);
	// poisson[20] = vec2(0.222999, -0.215125);
	// poisson[21] = vec2(-0.467574, -0.405438);
	// poisson[22] = vec2(-0.248268, -0.814753);
	// poisson[23] = vec2(0.354411, -0.887570);
	// poisson[24] = vec2(0.175817, 0.382366);
	// poisson[25] = vec2(0.487472, -0.063082);
	// poisson[26] = vec2(-0.084078, 0.898312);
	// poisson[27] = vec2(0.488876, -0.783441);
	// poisson[28] = vec2(0.470016, 0.217933);
	// poisson[29] = vec2(-0.696890, -0.549791);
	// poisson[30] = vec2(-0.149693, 0.605762);
	// poisson[31] = vec2(0.034211, 0.979980);
	// poisson[32] = vec2(0.503098, -0.308878);
	// poisson[33] = vec2(-0.016205, -0.872921);
	// poisson[34] = vec2(0.385784, -0.393902);
	// poisson[35] = vec2(-0.146886, -0.859249);
	// poisson[36] = vec2(0.643361, 0.164098);
	// poisson[37] = vec2(0.634388, -0.049471);
	// poisson[38] = vec2(-0.688894, 0.007843);
	// poisson[39] = vec2(0.464034, -0.188818);
	// poisson[40] = vec2(-0.440840, 0.137486);
	// poisson[41] = vec2(0.364483, 0.511704);
	// poisson[42] = vec2(0.034028, 0.325968);
	// poisson[43] = vec2(0.099094, -0.308023);
	// poisson[44] = vec2(0.693960, -0.366253);
	// poisson[45] = vec2(0.678884, -0.204688);
	// poisson[46] = vec2(0.001801, 0.780328);
	// poisson[47] = vec2(0.145177, -0.898984);
	// poisson[48] = vec2(0.062655, -0.611866);
	// poisson[49] = vec2(0.315226, -0.604297);
	// poisson[50] = vec2(-0.780145, 0.486251);
	// poisson[51] = vec2(-0.371868, 0.882138);
	// poisson[52] = vec2(0.200476, 0.494430);
	// poisson[53] = vec2(-0.494552, -0.711051);
	// poisson[54] = vec2(0.612476, 0.705252);
	// poisson[55] = vec2(-0.578845, -0.768792);
	// poisson[56] = vec2(-0.772454, -0.090976);
	// poisson[57] = vec2(0.504440, 0.372295);
	// poisson[58] = vec2(0.155736, 0.065157);
	// poisson[59] = vec2(0.391522, 0.849605);
	// poisson[60] = vec2(-0.620106, -0.328104);
	// poisson[61] = vec2(0.789239, -0.419965);
	// poisson[62] = vec2(-0.545396, 0.538133);
	// poisson[63] = vec2(-0.178564, -0.596057);

	// float focus = 30.0;
	// float depth = texture(rt_tex, screen_pos*0.5+0.5).a;

	// vec2 pix = vec2(1.0) / screen_res;
	// vec3 color = vec3(0.0);
	// float disc = 1.0;
	// disc = pow(abs(depth - focus), 0.7);
	// int samples = 64;
	// for (int i = 0; i < samples; ++i) {
	// 	color += texture(rt_tex, (screen_pos*0.5+0.5) + (poisson[i]*pix*disc)).rgb;
	// }
	// frag = color/float(samples);

	frag = texture(rt_tex, screen_pos*0.5+0.5).rgb;
}