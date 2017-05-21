
struct Material {
	vec3 albedo;
	float roughness;
	float metallic;
};

struct Camera {
	vec3 pos;
	mat3 mat;
};

float intersect(float a, float b) {
	return max(a, b);
}

float union(float a, float b) {
	return min(a, b);
}

float smooth_union(float a, float b, float k = 1.0) {
	// return min(a, b);

	// Exponential
	// k*=32;
	// float res = exp(-k*a) + exp(-k*b);
	// return -log(res)/k;

	// Polynomial
	float h = clamp(0.5+0.5*(b-a)/k, 0.0, 1.0);
	return mix(b, a, h) - k*h*(1.0-h);

	// Power smooth
	// k*=8;
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

float cube_plane(vec3 pos, float spacing, vec3 offset = vec3(0.0), vec3 size = vec3(1.0)) {
	vec3 d = abs(vec3(-1.0 + fract(pos.x/spacing)*spacing, pos.y, -1.0 + fract(pos.z/spacing)*spacing) - offset)-size;
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
