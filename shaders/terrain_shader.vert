#version 330 core
  
layout(location=0) in vec3 vVertex; //object space vertex position
layout(location=1) in vec2 vUV; // texture coordinates

//output to the fragment shader
smooth out vec2 uv;			//2D texture coordinates
smooth out vec3 worldpos;
uniform mat4 viewProj;
uniform float patchsize;
uniform vec3 patchorigin;
uniform bool flipy;

vec3 getworldposition(vec3 vertex)
{
	return vec3(vertex.x * patchsize , vertex.y, vertex.z * patchsize) + patchorigin;
}
void main()
{   	
	//get clipspace position from the object space position
	vec3 pos = vVertex;
	pos = vec3(pos.x * patchsize,pos.y,pos.z * patchsize ) + patchorigin;
	worldpos = pos;
	//set the object space position as the 2D texture coordinates
	uv = vUV;
	if (flipy)
	{
		uv.y = 1.0 - uv.y;
	}
	gl_Position = viewProj * vec4(pos,1);
    
}