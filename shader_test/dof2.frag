#version 330

uniform sampler2D rt_tex;
uniform sampler2D rt_coc;
uniform vec2 screen_res;
in vec2 screen_pos;
out vec3 frag;

#include "math.glsl"
#include "camera.glsl"

vec2 square_to_disk(vec2 sq) {
	float a = sq.x;//2*sq.x-1.0;
	float b = sq.y;//2*sq.y-1.0;

	float phi, r;
	if (a > -b) {
		if (a > b) {
			r = a;
			phi = (PI/4.0) * (b/a);
		} else {
			r = b;
			phi = (PI/4.0) * (2.0 - (a/b));
		}
	} else {
		if (a < b) {
			r = -a;
			phi = (PI/4.0) * (4.0+(b/a));
		} else {
			r = -b;
			if (b != 0.0) {
				phi = (PI/4.0) * (6.0-(a/b));
			} else {
				phi = 0.0;
			}
		}
	}

	return vec2(r*cos(phi), r*sin(phi));
}

void main() {

	float focus = 30.0;
	float depth = texture(rt_tex, screen_pos*0.5+0.5).a;

	vec2 pix = vec2(1.0) / screen_res;
	vec3 color = vec3(0.0);
	// float disc = get_coc(depth);
	// disc = pow(abs(depth - focus), 0.8);
	// int samples = /* 64 */ 7*7;
	// vec3 brightest = vec3(0.0);
	// for (int i = 0; i < samples; ++i) {
	// 	vec2 pos = vec2(-3.0+(i%7), -3.0+(i/7));
	// 	brightest = max(brightest, texture(rt_tex, (screen_pos*0.5+0.5) + (square_to_disk(pos)*(pix/2.0)*disc)).rgb);
	// }

	float disc = texture(rt_coc, screen_pos*0.5+0.5).r;

	vec3 brightest = texture(rt_tex, screen_pos*0.5+0.5).rgb;
	int samples = 6;
	for (int y = 0; y < samples; ++y) {
		for (int x = 0; x < samples; ++x) {
			vec2 d = square_to_disk(vec2(-1.0+(x/((samples-1)*0.5)), -1.0+(y/((samples-1)*0.5))));
			vec2 coord = (screen_pos*0.5+0.5) + (d*(pix/2.0)*disc);
			float weight = (texture(rt_coc, coord).r/30.0);
			vec4 samp = texture(rt_tex, coord) * weight;
			/* if (samp.a >= depth-1.0) */ {
				brightest = max(brightest, samp.rgb);
			}
		}
	}
	frag = brightest;

	frag = texture(rt_tex, screen_pos*0.5+0.5).rgb;
	// frag = vec3(texture(rt_tex, screen_pos*0.5+0.5).a, 0.0, 0.0)/100.0;
}