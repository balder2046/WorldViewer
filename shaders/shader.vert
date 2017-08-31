#version 330 core
 
layout(location = 0) in vec3 vVertex;	//object space vertex position

//output from the vertex shader
smooth out vec4 vSmoothColor;		//smooth colour to fragment shader
uniform vec4 ObjColor;
//uniform
uniform mat4 MVP;	//combined modelview projection matrix

void main()
{
	//assign the per-vertex colour to vSmoothColor varying
    vec4 pos = MVP*vec4(vVertex,1);
    pos.x /= pos.w;
    pos.y /= pos.w;
    pos.z /= pos.w;

    gl_Position = MVP*vec4(vVertex,1);

   vSmoothColor = vec4(pos.z,pos.z,pos.z,1);
   //get the clip space position by multiplying the combined MVP matrix with the object space
   //vertex position
   
}