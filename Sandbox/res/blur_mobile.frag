//#version 110
precision mediump float;
uniform sampler2D texture;
uniform vec2	uv;

uniform int		samples;
uniform float	kernel[64 * 4];
uniform vec2	offsets[64 * 4];
uniform vec4	srcMul;
uniform vec4	glColor;


void main() {
    vec2 coord = uv;
	vec4 color= vec4(0);
	for (int i = 0; i < samples; i++) {
		vec4 c = texture2D(texture, uv + offsets[i]);
		color += c * kernel[i] * srcMul;
	}
	gl_FragColor = color * glColor;
}
