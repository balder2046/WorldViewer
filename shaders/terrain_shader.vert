#version 330 core
  
layout(location=0) in vec3 vVertex; //object space vertex position
layout(location=1) in vec2 vUV; // texture coordinates

//output to the fragment shader
smooth out vec2 uv;			//2D texture coordinates
smooth out vec3 worldpos;
uniform mat4 viewProj;
uniform vec3 center;
uniform vec3 patchindex;
void main()
{   	
	//get clipspace position from the object space position
	vec3 pos = vVertex;
	pos = vec3(pos.x * patchindex.z + patchindex.x,pos.y,pos.z * patchindex.z + patchindex.y ) - center;
	gl_Position = viewProj * vec4(pos,1);
    worldpos = pos;
	//set the object space position as the 2D texture coordinates
	uv = vUV;
}