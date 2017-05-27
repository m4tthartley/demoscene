
float film_size = sqrt(24.0*24.0 + 36.0*36.0); // full frame
float film_format = 35.0;
float fov = degrees(2.0 * atan(film_size / (2.0*film_format)));
float focal_length = (0.5*film_format/* film_format */) / tan(fov / 2.0);

float aperture = focal_length / 5.0; // 1.4
float focus = 25.0;
// float dist = 10.0;
//float coc = aperture * ((focal_length*(focus-dist))/(dist*(focus-focal_length)));

float get_coc(float depth) {
	float coc = aperture * ((focal_length*(focus-depth)) / (depth*(focus-focal_length)));
	return min(abs(coc), 30.0);
}