#version 330 core
  
layout(location=0) in vec3 vVertex; //object space vertex position
layout(location=1) in vec2 vUV; // texture coordinates

//output to the fragment shader

smooth out vec3 worldpos;



uniform float patchsize;
uniform vec3 patchorigin;

vec3 getworldposition(vec3 vertex)
{
	return vec3(vertex.x * patchsize , vertex.y, vertex.z * patchsize) + patchorigin;
}


void main()
{   	
	//get clipspace position from the object space position
	gl_Position = vec4(vVertex,1);
    worldpos = getworldposfromvert(vVertex);

}