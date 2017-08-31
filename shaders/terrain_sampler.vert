#version 330 core
  
layout(location=0) in vec2 vVertex; //object space vertex position
layout(location=1) in vec3 vWorldPos; // texture coordinates
						   

//output to the fragment shader

smooth out vec3 worldpos;
void main()
{   	
	//get clipspace position from the object space position
	gl_Position = vec4(vVertex,0.999999,1);

    worldpos = vWorldPos;

}