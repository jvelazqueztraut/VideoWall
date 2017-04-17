#version 120

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;

uniform float rowInterval;
uniform float colInterval;
uniform float rowDisplacement;
uniform float colDisplacement;

void main() {
	vec2 pos = texCoordVarying;

	pos.x = pos.x + floor(pos.x/(colInterval-colDisplacement))*2*colDisplacement;
	pos.y = pos.y + floor(pos.y/(rowInterval-rowDisplacement))*2*rowDisplacement;
	
    // Get color value from texture
    gl_FragColor = texture2DRect(tex0, pos);
}