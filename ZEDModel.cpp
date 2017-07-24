#include <GL/glew.h>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ZEDModel.hpp"

Zed3D::Zed3D() {
    body_io.clear();
	path_mem.clear();
    vaoID = 0;
    vboID = 0;

}

Zed3D::~Zed3D() {
	body_io.clear();
	path_mem.clear();
}

void Zed3D::setPath(sl::Transform &Path,std::vector<sl::Translation> path_history) {
    body_io.clear();
    for (int i = 0; i < NB_ALLUMINIUM_TRIANGLES * 3; i += 3) {

        for (int j = 0; j < 3; j++) {
            double3color tmp;
            int index = alluminiumTriangles[i + j] - 1;
            tmp.setCoord(vertices[index * 3], vertices[index * 3 + 1], vertices[index * 3 + 2]);
            tmp.setColor(ALLUMINIUM_COLOR.r, ALLUMINIUM_COLOR.g, ALLUMINIUM_COLOR.b);
            tmp.transform(Path);
            body_io.push_back(tmp);
        }
    }

    for (int i = 0; i < NB_DARK_TRIANGLES * 3; i += 3) {
        for (int j = 0; j < 3; j++) {
            double3color tmp;
            int index = darkTriangles[i + j] - 1;
            tmp.setCoord(vertices[index * 3], vertices[index * 3 + 1], vertices[index * 3 + 2]);
            tmp.setColor(DARK_COLOR.r, DARK_COLOR.g, DARK_COLOR.b);
            tmp.transform(Path);
            body_io.push_back(tmp);
        }
    }

	path_mem = path_history;
    glBindVertexArray(vaoID);
    glBindBuffer(vboID,GL_ARRAY_BUFFER);

    glBufferData(GL_ARRAY_BUFFER, sizeof(double3color) * body_io.size(), &body_io[0], GL_DYNAMIC_DRAW);
    //enable vertex attribute array for position
    glEnableVertexAttribArray(shader["vVertex"]);
    int stride = sizeof(double3color);
    glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, stride, 0);
    //enable vertex attribute array for colour
    glEnableVertexAttribArray(shader["vColor"]);
    glVertexAttribPointer(shader["vColor"], 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)offsetof(double3color,r ));

}

void Zed3D::draw(glm::mat4 &pm) {
  //glPushMatrix();

    shader.Use();

    glBindVertexArray(vaoID);
  //  const GLubyte *buf = gluErrorString(code);
    glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(pm));
    glDrawArrays(GL_TRIANGLES,0,body_io.size() / 3);
    shader.UnUse();
    return;
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < NB_ALLUMINIUM_TRIANGLES * 3; i++) {
        double3color tmp = body_io.at(i);
        glColor3f(tmp.r, tmp.g, tmp.b);
        glVertex3f(tmp.x, tmp.y, tmp.z);
    }

    for (int i = NB_ALLUMINIUM_TRIANGLES * 3; i < NB_ALLUMINIUM_TRIANGLES * 3 + NB_DARK_TRIANGLES * 3; i++) {
        double3color tmp = body_io.at(i);
        glColor3f(tmp.r, tmp.g, tmp.b);
        glVertex3f(tmp.x, tmp.y, tmp.z);
    }
	glEnd();

 

	if (path_mem.size() > 1)
	{
		glBegin(GL_LINES);
		for (int i = 1; i < path_mem.size(); i++)
		{
			glColor3f(0.1f, 0.5f, 0.9f);
			glVertex3f(path_mem.at(i-1).tx, path_mem.at(i-1).y, path_mem.at(i-1).z);
			glVertex3f(path_mem.at(i).tx, path_mem.at(i).y, path_mem.at(i).z);
		}
		glEnd();
	}
	


//    glPopMatrix();
}

void Zed3D::init() {
    sl::Transform path;
    GLuint tt;
    glGenVertexArrays(1,&tt);
    glGenVertexArrays(1,&vaoID);
    glGenBuffers(1,&vboID);
    shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/shader.vert");
    shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/shader.frag");
    //compile and link shader
    shader.CreateAndLinkProgram();
    shader.Use();
    //add attributes and uniforms
    shader.AddAttribute("vVertex");
    shader.AddAttribute("vColor");
    shader.AddUniform("MVP");
    shader.UnUse();

    setPath(path, path_mem);
}
