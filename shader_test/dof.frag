#version 330

uniform sampler2D rt_tex;
uniform sampler2D rt_coc;
uniform sampler2D rt_coc_tile;
uniform vec2 screen_res;
in vec2 screen_pos;
out vec4 frag;

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
	// disc = pow(abs(depth - focus), 0.8);

	float total_weight = 0.0;

	// float disc = get_coc(depth);
	float disc = texture(rt_coc, screen_pos*0.5+0.5).r;
	{
		int samples = 64;
		// int actual_samples = 0;
		for (int y = 0; y < 8; ++y) {
			for (int x = 0; x < 8; ++x) {
				vec2 d = square_to_disk(vec2(-1.0+(x*(1.0/3.5)), -1.0+(y*(1.0/3.5))));
				vec2 coord = (screen_pos*0.5+0.5) + (d*(pix/2.0)*disc);

				float weight = (texture(rt_coc, coord).r /* /50.0 */);
				color += pow(texture(rt_tex, coord).rgb, vec3(1.0)) /* * (disc) */ * weight * 1.0;
				// ++actual_samples;
				total_weight += weight;
			}
		}
	}

	// disc = texture(rt_coc_tile, screen_pos*0.5+0.5).g;
	// {
	// 	int samples = 64;
	// 	for (int y = 0; y < 8; ++y) {
	// 		for (int x = 0; x < 8; ++x) {
	// 			vec2 d = square_to_disk(vec2(-1.0+(x*(1.0/3.5)), -1.0+(y*(1.0/3.5))));
	// 			vec2 coord = (screen_pos*0.5+0.5) + (d*(pix/2.0)*disc);

	// 			float weight = (texture(rt_coc, coord).g/30.0);
	// 			color += texture(rt_tex, coord).rgb * weight;
	// 			total_weight += weight;
	// 			// total_weight += 1.0;
	// 		}
	// 	}
	// }

	frag = vec4(color/total_weight /* float(actual_samples) */, /* depth */0.0);

	// frag = vec4(texture(rt_tex, screen_pos*0.5+0.5).rgb / 1.0, 0.0);
	// frag = vec3(texture(rt_tex, screen_pos*0.5+0.5).a, 0.0, 0.0)/100.0;
}