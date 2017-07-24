
#include "ScreenSizeQuad.h"
#include <glm/glm.hpp>
#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);
CScreenSizeQuad::CScreenSizeQuad(void)
{
	
	//generate the cube object
	shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/fullscreen_quad_shader.vert");
	GL_CHECK_ERRORS
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/fullscreen_quad_shader.frag");
	GL_CHECK_ERRORS
	shader.CreateAndLinkProgram();
	GL_CHECK_ERRORS
	shader.Use();
	
	GL_CHECK_ERRORS
		shader.AddAttribute("vVertex"); 
		shader.AddAttribute("vUV"); 		 
		shader.AddUniform("textureMap"); 
		glUniform1i(shader("textureMap"), 0);
	shader.UnUse();

	glm::vec2* vertices=new glm::vec2[4];
	vertices[0]=glm::vec2(-1,-1);
	vertices[1]=glm::vec2( 1,-1);
	vertices[2]=glm::vec2( 1, 1);
	vertices[3]=glm::vec2(-1, 1); 
	
	totalIndices = 2*3;
	GLushort* indices = new GLushort[totalIndices];
	 
	int count = 0;
	
	//fill indices array
	GLushort* id=&indices[0]; 
	 
	*id++ = 0; 	*id++ = 1; 	*id++ = 2;
	*id++ = 0; 	*id++ = 2; 	*id++ = 3; 
	 
	//setup vao and vbo stuff
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVerticesID);
	glGenBuffers(1, &vboIndicesID);

	glBindVertexArray(vaoID);	

		glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
		glBufferData (GL_ARRAY_BUFFER, 8*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		 
		glEnableVertexAttribArray(shader["vVertex"]);
		glVertexAttribPointer(shader["vVertex"], 2, GL_FLOAT, GL_FALSE,0,0);
		  
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*totalIndices, &indices[0], GL_STATIC_DRAW);
		 
	glBindVertexArray(0);
	delete [] indices;
	delete [] vertices;
}

void CScreenSizeQuad::FillVertexBuffer(GLfloat* pBuffer) {
	return;
}

void CScreenSizeQuad::FillIndexBuffer(GLuint* pBuffer) {
	return;
}
CScreenSizeQuad::~CScreenSizeQuad(void)
{
}


void CScreenSizeQuad::Render(const float* MVP) {
	shader.Use();	
	glUniform1i(shader("textureMap"), 0);
	glBindVertexArray(vaoID);
	
	glDrawElements(GL_TRIANGLES, totalIndices, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	shader.UnUse();
} 