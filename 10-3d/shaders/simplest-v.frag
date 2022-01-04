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
//  This version works for Vulkan:
//  glslc simplest-v.frag -o simplest-v.frag.spv
//
//-----------------------------------------------------------------------------

#version 460

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
  outColor = vec4(fragColor, 1.0);
}
