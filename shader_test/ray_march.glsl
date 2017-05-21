
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
	for (int i = 0; i < 200; ++i) {
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
		return SceneResult(candidate_t, sr.pos, sr.mat);
		//return sr;
	} else {
		return SceneResult(t_max, vec3(0.0), Material(vec3(0.0), 0.0, 0.0));
	}		
}
#endif

bool scene_hit(SceneResult res) {
	return res.depth < 100.0 - epsilon;
}

struct STResult {
	bool hit;
	vec3 color;
	float depth;
	vec3 normal;
};
STResult sphere_trace(vec2 sp, Camera camera) {
	{ // create camera matrix
		vec3 f = normalize(vec3(0.0) - camera.pos);
		vec3 s = normalize(cross(f, vec3(0.0, 1.0, 0.0)));
		vec3 u = cross(s, f);
		camera.mat = mat3(s, u, -f);
	}

	vec2 screen = vec2(2.0*(16.0/9.0), 2.0);
	vec2 xy = ((sp*0.5+0.5)*screen) - screen/2.0;
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
		reflection = ray_march(r_cam, r);

		r = reflection.pos;
		vec3 r_normal = normalize(vec3(scene(vec3(r.x + epsilon, r.y, r.z)).depth - scene(vec3(r.x - epsilon, r.y, r.z)).depth,
									   scene(vec3(r.x, r.y + epsilon, r.z)).depth - scene(vec3(r.x, r.y - epsilon, r.z)).depth,
									   scene(vec3(r.x, r.y, r.z + epsilon)).depth - scene(vec3(r.x, r.y, r.z - epsilon)).depth));

		reflect_color = light_it(r, r_normal, r_cam, reflection.mat, vec3(0.0), false);
	}

	if (scene_hit(res)) {
		vec3 lit_color = light_it(pos, normal, camera, res.mat, reflect_color);
		//final_color_hdr(lit_color);
		return STResult(true, lit_color, res.depth, normal);
	} else {
		return STResult(false, vec3(0.0), res.depth, normal);
	}
}
