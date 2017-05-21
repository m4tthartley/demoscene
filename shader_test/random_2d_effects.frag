
#include "math.glsl"

void circle_grid() {
	vec3 color = vec3(smoothstep(-1.0, 1.0, position.x), smoothstep(-1.0, 1.0, position.y), 0.5);
	
	vec2 pm = fract((p+t) * 2.0)*2.0 - 1.0;
	float dist = sqrt(pm.x*pm.x + pm.y*pm.y);
	float r = rand(p.x*1000.0);
	{
		float i = floor(p.x*20.0);
		float f = fract(p.x*20.0);

		r = rand(i);
		r = mix(rand(i), rand(i+1.0), f);
		r = mix(rand(i), rand(i+1.0), smoothstep(0, 1, f));
	}
	{
		float scale = 40.0;
		vec2 i = vec2(floor((p.x-t)*scale), floor((p.y-t)*scale));
		vec2 f = vec2(fract((p.x-t)*scale), fract((p.y-t)*scale));

		//r = rand(i);
		//r = mix(rand(i), rand(i+1.0), f);
		r = pow(mix(rand(i.x), rand(i.x+1.0), smoothstep(0, 1, f.x)),
				mix(rand(i.y), rand(i.y+1.0), smoothstep(0, 1, f.y)));
	}

	final_color(mix(vec3(r), color, smoothstep(0.5, 1.0, dist)));
}

void swirly_lines() {
	float r = noise2(p * 5.0 + t*10.0);
	{
		float zoom = 1.0+(0.5+sin(time*0.5)*0.5)*100.0;
		vec2 pos = rotate2d(noise2(p*zoom+t)) * p;
		r = smoothstep(0.0, 0.2, abs(0.5 - fract(pos.y)));
	}
	
	vec4 circle_color = vec4(r, r, r, 1.0);
	out_color = circle_color;
}

void cell_noise() {
	vec2 points[4];
	points[0] = vec2(0.83,0.75);
	points[1] = vec2(0.60,0.07);
	points[2] = vec2(0.28,0.64);
	points[3] = vec2(0.31,0.26);
	float min_dist = 1.0;
	float t = t*0.0005;
	for (int i = 0; i < 10; ++i) {
		vec2 point = vec2(noise2(vec2(i*t, 0.0)) * 10.0, noise2(vec2(0.0, i*t)) * 10.0);
		min_dist = min(min_dist, distance(p, point));

	}
	out_color = vec4(vec3(min_dist), 1.0);
}

float fbm2(vec2 st) {
	float result = 0.0;
	float amp = 0.5;
	float freq = 0.0;
	for (int i = 0; i < 8; ++i) {
		result += amp * noise2(st);
		st *= 2.0;
		amp *= 0.8;
	}
	return result;
}

void warp1() {
	vec2 pos = p*0.05;
	float f = fbm2(pos + fbm2(pos + fbm2(pos)));
	out_color = vec4(vec3(f), 0.0);
}
void warp2() {
	vec3 pos = vec3(p*2.0, t*0.2);
	float f = fbm(pos + fbm(pos + fbm(pos)));
	final_color(vec3(f*0.5+0.5));
}

void warp3() {
	vec2 q = vec2(fbm(vec3(p, t*1.0)),
				  fbm(vec3(p+vec2(5.2, 1.3), 0.0)));
	vec2 r = vec2(fbm(vec3(p+4.0*q+vec2(1.7, 9.2), 0.0)),
				  fbm(vec3(p+4.0*q+vec2(8.3, 2.8), 0.0)));
	final_color(vec3(fbm(vec3(p+4.0*r, 0.0)), fbm(vec3(r, 0.0)), fbm(vec3(q, 0.0)))*0.5+0.5);
}
void warp4() {
	vec3 pos = vec3(p*1.0, t);
	float a = fbm(pos);
	float b = fbm(pos*2.0 + vec3(a));
	float c = fbm(pos*4.0 + vec3(b));

	float c1 = fbm(vec3(a, b*2.0, c*4.0));
	float c2 = fbm(vec3(b, c*2.0, a*4.0));
	float c3 = fbm(vec3(c, a*2.0, b*4.0));
	vec3 col = vec3(c1, c2, c3);

	final_color(col*0.5+0.5);
}

void main() {
	circle_grid();
	// swirly_lines();
	// cell_noise();
	// warp1();
	// warp2();
	// warp3();
	// warp4();
}
