#version 330

in vec2 vertex_position;
out vec2 screen_pos;
void main() {
	screen_pos = vertex_position;
	gl_Position = vec4(vertex_position/**vec2(0.9, 0.9)*/, 0.0, 1.0);
}
