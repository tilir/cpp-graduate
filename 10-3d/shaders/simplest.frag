//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Simple fragment shader: varying color passed from vertex, interpolated
//
//-----------------------------------------------------------------------------

#version 460

// from vertex shader
in vec3 vColor;

// time in seconds with fractional part
uniform float time;

void main() {  
	gl_FragColor = vec4(vColor, 1.0);
}