//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Simple but interesting fragment shader example
//
//----------------------------------------------------------------------------

// from vertex shader
in vec3 ourColor;

// to rasterizer
out vec4 FragColor;

// time in seconds with fractional part
uniform float time;

void main() {  
	FragColor = vec4(ourColor, 1.0);
}