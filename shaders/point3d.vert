#version 330 core
 
layout(location = 0) in vec3 vVertex;	//object space vertex position
layout(location = 1) in float PointSize;
layout(location = 2) in vec4 ObjColor;

//output from the vertex shader
smooth out vec4 vSmoothColor;		//smooth colour to fragment shader
//uniform
uniform mat4 MVP;	//combined modelview projection matrix

void main()
{
	//assign the per-vertex colour to vSmoothColor varying

    gl_Position = MVP*vec4(vVertex,1);
    gl_Position.z = 0.0;
    gl_PointSize = PointSize;
   vSmoothColor = ObjColor;
   //get the clip space position by multiplying the combined MVP matrix with the object space
   //vertex position
   
}