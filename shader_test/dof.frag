#version 330

uniform sampler2D rt_far;
uniform sampler2D rt_near;
uniform sampler2D rt_coc;
uniform sampler2D rt_coc_tile;
uniform vec2 screen_res;
in vec2 screen_pos;
out vec4 fragfar;
out vec4 fragnear;

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
	// float focus = 30.0;
	// float depth = texture(rt_tex, screen_pos*0.5+0.5).a;

	vec2 pix = vec2(1.0) / screen_res;

	vec4 color_far = vec4(0.0);
	float total_weight_far = 1.0;
	vec4 color_near = vec4(0.0);
	float total_weight_near = 1.0;
	float final_near_coc = 0.0;
	float final_near_coc_total = 1.0;

	// float disc = get_coc(depth);
	float farcoc = texture(rt_coc, screen_pos*0.5+0.5).r;
	float nearcoc = texture(rt_coc, screen_pos*0.5+0.5).g;
	// vec3 c = vec3(0.0);
	// if (texture(rt_coc, screen_pos*0.5+0.5).r > 0.0) {
		int samples = 64;
		// int actual_samples = 0;
		for (int y = 0; y < 16; ++y) {
			for (int x = 0; x < 16; ++x) {
				// vec2 d = square_to_disk(vec2(-1.0+(x*(1.0/3.5)), -1.0+(y*(1.0/3.5))));

				// vec2 d = square_to_disk(vec2(-1.0+(x/7.0 * 2.0), -1.0+(y/7.0 * 2.0)));
				// vec2 coord = (screen_pos*0.5+0.5) + (d*farcoc*0.0004*vec2(1.0, screen_res.x/screen_res.y));

				if (farcoc > 0.0) {
					vec2 d = square_to_disk(vec2(-0.5) + vec2(x, y)/vec2(15.0));
					vec2 coord = (screen_pos*0.5+0.5) + (d*0.0005*farcoc*vec2(screen_res.y/screen_res.x, 1.0));

					float weight = pow(texture(rt_coc, coord).r, 1.0) / 50.0;
					color_far += pow(texture(rt_far, coord), vec4(1.0)) * pow(weight, 1.0);
					total_weight_far += weight;
				}

				float near_tile_max = texture(rt_coc_tile, screen_pos*0.5+0.5).g / 50.0;
				/* if (nearcoc > 0.0) */ {
					float scale = 0.75 + nearcoc/200.0 ;//>= near_tile_max ? 1.0 : (near_tile_max - nearcoc);
					// scale = max(scale, 0.5);
					vec2 d = square_to_disk(vec2(-0.5) + vec2(x, y)/vec2(15.0));
					vec2 coord = (screen_pos*0.5+0.5) + (d*0.002*(scale*50.0)*vec2(screen_res.y/screen_res.x, 1.0));

					float sample_coc = texture(rt_coc, coord).g / 50.0;
					//float weight = sample_coc >= near_tile_max ? 1.0 : (near_tile_max - sample_coc);
					float weight = 0.2 + sample_coc;
					color_near += texture(rt_near, coord) /* * sample_coc */ * weight;
					total_weight_near += weight;




					// final_near_coc += texture(rt_coc, coord).g * weight;
					// final_near_coc_total += weight;
				} /* else {
					vec2 d = square_to_disk(vec2(-0.5) + vec2(x, y)/vec2(15.0));
					vec2 coord = (screen_pos*0.5+0.5) + (d*0.002*50.0*vec2(screen_res.y/screen_res.x, 1.0));

					color_near += texture(rt_near, coord).rgb;
					total_weight_near += 1.0;
					final_near_coc += texture(rt_coc, coord).g;
					final_near_coc_total += 1.0;
				} */
			}
		}

		fragfar = color_far/total_weight_far;
		fragnear = color_near/total_weight_near;
		// fragnear = vec4(fragnear.a/50.0, 0.0, 0.0, 0.0);

		// fragnear = vec4(final_near_coc/final_near_coc_total / 50.0, 0.0, 0.0, 1.0);
		// fragnear = vec4(texture(rt_near, screen_pos*0.5+0.5).rgb, 1.0);
	// } else {
	// 	frag = texture(rt_tex, screen_pos*0.5+0.5);
	// }

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
}