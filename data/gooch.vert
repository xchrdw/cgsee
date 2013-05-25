#version 330
//
// Vertex shader for Gooch shading
//
// Author: Randi Rost
//
// Copyright (c) 2002-2005 3Dlabs Inc. Ltd.
//
// See 3Dlabs-License.txt for license information
//
//updated code to newer glsl version


uniform vec3 lightposition; // (0.0, 10.0, 4.0)
uniform mat4 transform;
uniform mat4 view;

in vec3 a_vertex;
in vec3 a_normal;
out vec3 normal;

void main()
{
	gl_Position = transform * vec4(a_vertex, 1.0);
	//vec4 ecPos = vec3(transform * a_vertex);
	normal=a_normal;

}