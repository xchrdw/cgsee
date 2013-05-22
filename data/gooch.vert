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
out float NdotL;
out vec3 ReflectVec;
out vec3 ViewVec;

void main()
{
	gl_Position = transform * vec4(a_vertex, 1.0);
	//vec4 ecPos = vec3(transform * a_vertex);

	vec4 temp= normalize(view*vec4(a_normal,0.0));
	vec3 tnorm = temp.xyz;

	vec3 lightVec = normalize(lightposition - gl_Position.xyz);
	ReflectVec = normalize(reflect(-lightVec, tnorm));
	ViewVec = normalize(-gl_Position.xyz);
	NdotL = (dot(lightVec, tnorm) + 1.0) * 0.5;
	//gl_Position = transform * vec4(a_vertex, 1.0);
}