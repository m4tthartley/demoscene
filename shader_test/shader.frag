#version 330

#define PI 3.14159265359
#define PI2 (3.14159265359*2.0)
#define saturate(x) clamp(x, 0.0, 1.0)

struct Material {
	vec3 albedo;
	float roughness;
	float metallic;
};

struct Camera {
	vec3 pos;
	mat3 mat;
};

struct Light {
	vec3 pos;
	vec3 color;
};

struct Lighting {
	Light lights[16];
	int num_lights;
};

Lighting lighting;

#include "pbr.glsl"

uniform float time;

// smoothstep = f*f*(3.0-2.0*f)

float rand(float f) {
	// y = fract(sin(x)*100000.0)
	return fract(sin(f));
}

// from The Book of Shaders
float rand2(vec2 st) {
	return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453123);
}
vec2 rand22(vec2 st) {
	st = vec2(dot(st, vec2(127.1, 311.7)), dot(st, vec2(269.5, 183.3)));
	return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}
mat2 rotate2d(float angle) {
	return mat2(cos(angle),-sin(angle), sin(angle),cos(angle));
}

float noise2(vec2 p) {
	vec2 i = floor(p);
	vec2 f = fract(p);
#if 0
	float a = rand2(i);
	float b = rand2(i + vec2(1.0, 0.0));
	float c = rand2(i + vec2(0.0, 1.0));
	float d = rand2(i + vec2(1.0, 1.0));
#else
	float a = dot(rand22(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0));
	float b = dot(rand22(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0));
	float c = dot(rand22(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0));
	float d = dot(rand22(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0));
#endif
	vec2 u = smoothstep(0.0, 1.0, f);
 	return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

// from Inigo Quilez (what a guy)
float hash(float n) {
	return fract(sin(n)*753.5453123);
}
float noise(vec3 x) {
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix(hash(n+0.0), hash(n+1.0), f.x), mix(hash(n+157.0), hash(n+158.0), f.x), f.y),
               mix(mix(hash(n+113.0), hash(n+114.0), f.x), mix(hash(n+270.0), hash(n+271.0), f.x), f.y), f.z);
}
float fbm(vec3 x) {
    float a = noise(x);
    a += noise(x * 2.0) / 2.0;
    a += noise(x * 4.0) / 4.0;
    a += noise(x * 8.0) / 8.0;
    a += noise(x * 16.0) / 16.0;
    return a;
}
// float hash(float x) {
// 	return fract(x*17.0 * fract(x*0.3183099));
// }
// float noise(vec3 x) {
// 	vec3 i = floor(x);
// 	vec3 f = fract(x);

// 	vec3 u = f*f*f*(f*(f*6.0-15.0)+10.0);
// 	float n = i.x + 317.0*i.y + 157.0*i.z;

// 	float a1 = hash(n);
// 	float a2 = hash(n+1.0);
// 	float a3 = hash(n+317.0);
// 	float a4 = hash(n+318.0);
// 	float a5 = hash(n+157.0);
// 	float a6 = hash(n+158.0);
// 	float a7 = hash(n+474.0);
// 	float a8 = hash(n+475.0);

// 	float k0 = a1;
// 	float k1 = a2 - a1;
// 	float k2 = a3 - a1;
// 	float k3 = a5 - a1;
// 	float k4 = a1 - a2 - a3 + a4;
// 	float k5 = a1 - a3 - a5 + a7;
// 	float k6 = a1 - a2 - a5 + a6;
// 	float k7 = -a1 + a2 + a3 - a4 + a5 - a6 - a7 + a8;
	
// 	return -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z);
// }
// float fbm(vec3 x, int octaves = 6) {
// 	float a = 0.0;
// 	float b = 0.5;
// 	float s = 0.49;
// 	float f = 1.98;
// 	mat3 m = mat3(1.0, 0.0, 0.0,
// 				  0.0, 1.0, 0.0,
// 				  0.0, 0.0, 1.0);
// 	for (int i = 0; i < octaves; ++i) {
// 		float n = noise(x);
// 		a += b*n;
// 		b *= s;
// 		x = f*(m*3.0)*x;
// 		m = f*(m*3.0*i)*m;
// 	}
// 	return a;
// }

in vec2 position;
out vec4 out_color;

float aspect = 1280.0/720.0;
float t = time*0.25;
vec2 p = vec2(position.x*-aspect, position.y);

vec3 gamma(vec3 x) {
	// x = x / (x + vec3(1.0));
	return pow(x, vec3(1.0/2.2));
}

vec3 gamma_hdr(vec3 x) {
	x = x / (x + vec3(1.0));
	return pow(x, vec3(1.0/2.2));
}

void final_color(vec3 color) {
	out_color = vec4(gamma(color), 1.0);
}

void final_color_hdr(vec3 color) {
	out_color = vec4(gamma_hdr(color), 1.0);
}

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

vec3 yrotate(vec3 x, float a) {
	return vec3(x.x*cos(a) - x.z*sin(a), x.y, x.z*cos(a) + x.x*sin(a));
}

float intersect(float a, float b) {
	return max(a, b);
}

float union(float a, float b) {
	return min(a, b);
}

float smooth_union(float a, float b, float k = 1.0) {
	// return min(a, b);

	// Exponential
	// float res = exp(-k*a) + exp(-k*b);
	// return -log(res)/k;

	// Polynomial
	float h = clamp(0.5+0.5*(b-a)/k, 0.0, 1.0);
	return mix(b, a, h) - k*h*(1.0-h);

	// Power smooth
	// a = pow(a, k);
	// b = pow(b, k);
	// return pow((a*b)/(a+b), 1.0/k);
}

float difference(float a, float b) {
	return max(a, -b);
}

float sphere(vec3 pos, vec3 offset = vec3(0.0), float size = 1.0) {
	return length(pos - offset) - size;
}

float sphere_plane(vec3 pos, float spacing, vec3 offset = vec3(0.0), float size = 1.0) {
	vec3 p = pos;
	return length(vec3(-1.0 + fract(p.x/spacing)*spacing, p.y, -1.0 + fract(p.z/spacing)*spacing) - offset) - size;
}

float cube(vec3 pos, vec3 offset = vec3(0.0), vec3 size = vec3(1.0)) {
	// return length(max(abs(pos) - offset, 0.0));
	vec3 d = abs(pos - offset)-size;
	return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0)) - (length(size)*0.05);
}

float plane(vec3 pos, vec4 n) {
	// n = normalize(n);
	return dot(pos, -normalize(n.xyz)) + n.w;
}

struct SceneResult {
	float depth;
	vec3 pos;
	Material mat;
};

SceneResult sr_union(SceneResult a, SceneResult b) {
	if (union(a.depth, b.depth) == a.depth) return a;
	else return b;
}

SceneResult scene(vec3 pos) {
	lighting.num_lights = 1;//int(floor(mod(time, 8.0)));
	lighting.lights[0].pos = vec3(15.0, 4.0, 15.0);
	// lighting.lights[1].pos = vec3(-10.0, 10.0, 10.0);
	// lighting.lights[2].pos = vec3(10.0, -10.0, 10.0);
	// lighting.lights[3].pos = vec3(-10.0, -10.0, 10.0);
	// lighting.lights[4].pos = vec3(10.0, 10.0, -10.0);
	// lighting.lights[5].pos = vec3(-10.0, 10.0, -10.0);
	// lighting.lights[6].pos = vec3(10.0, -10.0, -10.0);
	// lighting.lights[7].pos = vec3(-10.0, -10.0, -10.0);

	// float a = sphere(pos, vec3(0.0, 0.0, 0.0), 1.4);
	// a = difference(cube(pos), a);
	// a = union(a, intersect(sphere(pos, vec3(0.0), 0.27), cube(yrotate(pos, time*2.0), vec3(0.0), vec3(0.2))));
	// return a;

	float y = clamp(pos.y*(0.5*(sin(fract(time*0.5)*PI2))), -1.0, 1.0);
	float s = sin(y);
	float c = cos(y);
	vec3 p = vec3(pos.x*c - pos.z*s, pos.y, pos.z*c + pos.x*s);
	// vec3 p = yrotate(pos, pos.y*0.5);

	float spacing = 2.0;
	// return sphere_plane(pos, spacing, vec3(/* 1.0*sin(time) */0.0, 4.0*cos(1.0*time+noise(floor(vec3(pos.x/spacing, pos.z/spacing, 0.0))*spacing)), 0.0));
	float o = sin(fbm(vec3(vec2(1.0, 1.0), time*0.25))*PI2);
	vec3 stuff[8];
	stuff[0] = vec3(-1.5, -1.5, -1.5);
	stuff[1] = vec3( 1.5, -1.5, -1.5);
	stuff[2] = vec3(-1.5,  1.5, -1.5);
	stuff[3] = vec3( 1.5,  1.5, -1.5);
	stuff[4] = vec3(-1.5, -1.5, 1.5);
	stuff[5] = vec3( 1.5, -1.5, 1.5);
	stuff[6] = vec3(-1.5,  1.5, 1.5);
	stuff[7] = vec3( 1.5,  1.5, 1.5);
	float a = sphere(pos, 			stuff[0]*vec3(sin(fbm(stuff[0])+time), sin(fbm(stuff[0])+time), sin(fbm(stuff[0])+time)));
	a = smooth_union(a, sphere(pos, stuff[1]*vec3(sin(fbm(stuff[1])+time*1.1), sin(fbm(stuff[1])+time*1.1), sin(fbm(stuff[1])+time*1.1))));
	a = smooth_union(a, sphere(pos, stuff[2]*vec3(sin(fbm(stuff[2])+time*1.2), sin(fbm(stuff[2])+time*1.2), sin(fbm(stuff[2])+time*1.2))));
	a = smooth_union(a, sphere(pos, stuff[3]*vec3(sin(fbm(stuff[3])+time*1.3), sin(fbm(stuff[3])+time*1.3), sin(fbm(stuff[3])+time*1.3))));
	a = smooth_union(a, sphere(pos, stuff[4]*vec3(sin(fbm(stuff[4])+time*1.4), sin(fbm(stuff[4])+time*1.4), sin(fbm(stuff[4])+time*1.4))));
	a = smooth_union(a, sphere(pos, stuff[5]*vec3(sin(fbm(stuff[5])+time*1.5), sin(fbm(stuff[5])+time*1.5), sin(fbm(stuff[5])+time*1.5))));
	a = smooth_union(a, sphere(pos, stuff[6]*vec3(sin(fbm(stuff[6])+time*1.6), sin(fbm(stuff[6])+time*1.6), sin(fbm(stuff[6])+time*1.6))));
	a = smooth_union(a, sphere(pos, stuff[7]*vec3(sin(fbm(stuff[7])+time*1.7), sin(fbm(stuff[7])+time*1.7), sin(fbm(stuff[7])+time*1.7))));
	SceneResult res = SceneResult(a, pos, /* Material(vec3(0.2, 0.1, 1.0), 0.4, 0.7) */Material(vec3(1.0, 1.0, 1.0), 0.5, 0.7));

	Material m = Material(vec3(1.0, 0.5, 0.5), 0.5, 0.0);
	Material sand = Material(vec3(0.2, 0.2, 0.05), 1.0, 0.0);
	res = sr_union(res, SceneResult(plane(pos, vec4(-1.0, 0.0, 0.0, 20.0)), pos, m));
	res = sr_union(res, SceneResult(plane(pos, vec4(1.0, 0.0, 0.0, 20.0)), pos, m));
	res = sr_union(res, SceneResult(plane(pos, vec4(0.0, -1.0, 0.0, 5.0 /* - noise(pos+noise(pos+10.0))*0.1 */)), pos, sand));
	res = sr_union(res, SceneResult(plane(pos, vec4(0.0, 1.0, 0.0, 5.0)), pos, m));
	res = sr_union(res, SceneResult(plane(pos, vec4(0.0, 0.0, -1.0, 20.0)), pos, m));
	res = sr_union(res, SceneResult(plane(pos, vec4(0.0, 0.0, 1.0, 20.0)), pos, m));
	return res;

	// return cube(p, vec3(1.0*sin(time), 1.0*cos(time), 0.0));
}

const float epsilon = 0.0001;

#if 0
SceneResult ray_march(vec2 position, Camera camera) {
	vec2 screen = vec2(2.0*(16.0/9.0), 2.0);
	vec2 xy = ((position*0.5+0.5)*screen) - screen/2.0;
	float z = screen.y / tan(radians(70.0)/2.0);
	vec3 ray_dir = camera.mat * normalize(vec3(xy, -z));

	float depth = 0.0;
	float end = 100.0;
	vec3 pos;
	for (int i = 0; i < 255; ++i) {
		pos = camera.pos + depth*ray_dir;
		float dist = scene(pos);
		if (dist < 0.0001) {
			break;
		}
		depth += dist;
		if (depth >= end) {
			depth = end;
			break;
		}
	}

	return SceneResult(depth, pos);
}
#else
// Enhanced
SceneResult ray_march(Camera camera, vec3 ray) {
	float t = 0.001;
	float t_max = 100.0;
	float omega = 1.2;
	float previous_radius = 0.0;
	float step_length = 0.0;
	float candidate_error = 1.0/0.0;
	float candidate_t = t;
	float pixel_radius = 0.001;
	vec3 pos;
	SceneResult sr;
	for (int i = 0; i < 100; ++i) {
		pos = camera.pos + t*ray;
		sr = scene(pos);
		float signed_radius = 1.0 * sr.depth;
		float radius = abs(signed_radius);
		bool fail = omega > 1.0 && (radius+previous_radius) < step_length;
		if (fail) {
			step_length -= omega * step_length;
			omega = 1;
		} else {
			step_length = signed_radius*omega;
		}

		previous_radius = radius;
		float error = radius / t;
		if (!fail && error < candidate_error) {
			candidate_t = t;
			candidate_error = error;
		}

		if (!fail && error < pixel_radius || t > t_max) break;
		t += step_length;
	}

	if (t <= t_max && candidate_error <= pixel_radius) {
		// return SceneResult(candidate_t, camera.pos + candidate_t*ray);
		return sr;
	} else {
		return SceneResult(t_max, vec3(0.0), Material(vec3(0.0), 0.0, 0.0));
	}		
}
#endif

bool scene_hit(SceneResult res) {
	return res.depth < 100.0 - epsilon;
}

void main() {
	circle_grid();
	// swirly_lines();
	// cell_noise();
	// warp1();
	// warp2();
	// warp3();
	// warp4();

#if 1
	

	{
		float t = time;
		Camera camera;
		camera.pos = vec3(sin(t)*10.0, sin(fract(time*0.25)*PI2)*4.0, cos(t)*10.0);
		// camera.pos = vec3(sin(t)*20.0, sin(fract(time*0.1)*PI2)*10.0, cos(t)*20.0);
		// camera.pos = vec3(0.0, 0.0, 10.0);
		vec3 f = normalize(vec3(0.0) - camera.pos);
		vec3 s = normalize(cross(f, vec3(0.0, 1.0, 0.0)));
		vec3 u = cross(s, f);
		camera.mat = mat3(s, u, -f);

		
		// ray_dir.x *= -normalize(camera).x;
		// ray_dir.z *= -normalize(camera).z;

		// float dist = ray_march(vec3(0.0, 0.0, 10.0), ray_dir);
		
		vec2 screen = vec2(2.0*(16.0/9.0), 2.0);
		vec2 xy = ((position*0.5+0.5)*screen) - screen/2.0;
		float z = screen.y / tan(radians(70.0)/2.0);
		vec3 ray_dir = camera.mat * normalize(vec3(xy, -z));
		SceneResult res = ray_march(camera, ray_dir);
		vec3 pos = res.pos;

		vec3 normal = normalize(vec3(scene(vec3(pos.x + epsilon, pos.y, pos.z)).depth - scene(vec3(pos.x - epsilon, pos.y, pos.z)).depth,
									 scene(vec3(pos.x, pos.y + epsilon, pos.z)).depth - scene(vec3(pos.x, pos.y - epsilon, pos.z)).depth,
									 scene(vec3(pos.x, pos.y, pos.z + epsilon)).depth - scene(vec3(pos.x, pos.y, pos.z - epsilon)).depth));

		SceneResult reflection;
		vec3 reflect_color;
		{
			vec3 r = reflect(normalize(pos-camera.pos), normal);
			Camera r_cam;
			r_cam.pos = pos;
			reflection = ray_march(camera, r);

			r = reflection.pos;
			vec3 r_normal = normalize(vec3(scene(vec3(r.x + epsilon, r.y, r.z)).depth - scene(vec3(r.x - epsilon, r.y, r.z)).depth,
										   scene(vec3(r.x, r.y + epsilon, r.z)).depth - scene(vec3(r.x, r.y - epsilon, r.z)).depth,
										   scene(vec3(r.x, r.y, r.z + epsilon)).depth - scene(vec3(r.x, r.y, r.z - epsilon)).depth));

			reflect_color = light_it(r, r_normal, r_cam, reflection.mat, vec3(0.0));
		}

		// float modulate = 1.0;
		// vec3 n = vec3(pow(fbm(pos)*modulate, 1.0),
		// 			  pow(fbm(pos+10)*modulate, 1.0),
		// 			  pow(fbm(pos+20)*modulate, 1.0));
		// normal *= n;
		// normal = normalize(normal);

		if (!scene_hit(res)) {
			final_color(vec3(0.001, 0.001, 0.01));
		} else {
			final_color(vec3(1.0, fract(time), 1.0-fract(time)));
			float light = max(dot(normal, normalize(vec3(10.0, 10.0, 10.0))), 0.0) * (1.0 / length(vec3(10.0, 10.0, 10.0))*length(vec3(10.0, 10.0, 10.0)));
			// final_color(vec3(0.1 + light));
			float f = clamp(pow((fbm(normal)*1.0), 3.1), -1.0, 1.0)*0.5+0.5;
			// f = saturate(pow(saturate(fbm(normal * fbm(normal)))+0.0, 1));
#if 1
			f = saturate(pow((fbm(pos*2.0)), 30.0));
#else
			f = saturate(pow(fbm(pos * 10.0 * fbm(normal * 1.0)), 2.0) + 0.0);
#endif
			/* REAL NICE
			Material m1 = Material(vec3(0.01, 0.01, 0.01), 0.0, 0.0);
			Material m2 = Material(vec3(0.0, 0.1, 0.3), 0.8, 0.2);
			Material m = Material(mix(m1.albedo, m2.albedo, f), mix(m1.roughness, m2.roughness, f), mix(m1.metallic, m2.metallic, f)); */
			
			final_color_hdr(light_it(pos, normal, camera, res.mat, reflect_color/* Material(vec3(0.0, 0.1, 1.0), 0.4, 0.7) *//* , lighting */));
			// out_color = vec4(light_it(pos, normal, Camera(camera, cam_mat)), 1.0);
		}
	}
#endif
}
