#version 330

uniform sampler2D frame;
uniform vec2 resolution;
in vec2 position;

out vec4 out_color;

#include "math.glsl"

void main() {
	vec2 pix = vec2(1.0)/resolution;
	vec3 color = vec3(0.0);

#if 0 // box blur
	float samples = 41;
	for (int y = int(-floor(samples/2)); y < int(ceil(samples/2)); ++y)
	for (int x = int(-floor(samples/2)); x < int(ceil(samples/2)); ++x) {
		color += texture(frame, (position*0.5+0.5)+vec2(pix.x*float(x), pix.y*float(y))).rgb;
	}
	out_color = vec4(color/(samples*samples), 0.0);
#elif 0 // weird radial blur
	float samples = 10;
	for (int rot = 0; rot < samples; ++rot)
	for (int stp = 0; stp < samples; ++stp) {
		float r = float(rot)/samples * PI2;
		vec2 a = vec2(sin(r), cos(r)) * (pix*3.0) * stp;
		color += texture(frame, (position*0.5+0.5)+a).rgb;
	}
	out_color = vec4(color/(samples*samples), 1.0);
#else
	out_color = texture(frame, position*0.5+0.5);
#endif
}
