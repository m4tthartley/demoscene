
#define PI 3.14159265359
#define PI2 (3.14159265359*2.0)
#define saturate(x) clamp(x, 0.0, 1.0)

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

vec3 yrotate(vec3 x, float a) {
	return vec3(x.x*cos(a) - x.z*sin(a), x.y, x.z*cos(a) + x.x*sin(a));
}
