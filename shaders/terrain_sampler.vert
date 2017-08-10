#version 330 core
  
layout(location=0) in vec2 vVertex; //object space vertex position
layout(location=1) in vec2 vUV; // texture coordinates

//output to the fragment shader

smooth out vec3 worldpos;
smooth out vec2 uv;


uniform float patchsize;
uniform vec3 patchorigin;

vec3 getworldposition(vec3 vertex)
{
	return vec3(vertex.x * patchsize , vertex.y, vertex.z * patchsize) + patchorigin;
}


void main()
{   	
	//get clipspace position from the object space position
	gl_Position = vec4(vVertex,0.0,1);
	uv = vUV;
    worldpos = getworldposition(vec3(vVertex,0.1));

}