#include "TestTri.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/vec4.hpp"
#include "RenderTextureFBO.h"
#include "GLSLShader.h"
#include "ScreenSizeQuad.h"
#include "Texture.h"
#include <opencv2/imgcodecs.hpp>
#include "Terrain.h"
#include "Camera.h"
#include "ZEDModel.hpp"
#include "RenderDevice.h"
using namespace TRN;
using namespace glm;

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);


using namespace std;
using namespace cv;

//screen size
const int WIDTH = 800;
const int HEIGHT = 600;
TrackBallCameraA camera(vec3(0.0f,20.0f,20.0f),vec3(0.0f,0.0f,0.0f));
//shader reference
GLSLShader shader;
Zed3D g_zed3D;
bool g_bdraw3d = true;
//vertex array and vertex buffer object IDs
GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;
int g_iTestPatchX = 1;
int g_iTestPatchY = 1;
//out vertex struct for interleaved attributes
struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
};

//triangle vertices and indices
Vertex vertices[3];
GLushort indices[3];

//projection and modelview matrices
glm::mat4  P = glm::mat4(1);
glm::mat4 MV = glm::mat4(1);
RenderTextureFBO *g_pRenderTextureFBO = 0;
RenderTextureFBO *g_pScreenFBO = 0;
CScreenSizeQuad *g_pScreenQuad = 0;
Texture g_Texture1;
RenderDevice g_RenderDevice;
GLuint g_testText;
Terrain g_Terrain;
Terrain g_TerrainSamp;
int windowWidth = 0;
int windowHeight = 0;

bool Rotate = false;
int startx = 0;
int starty = 0;
bool Translate = false;
bool Zoom = false;
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            Rotate = true;
            startx = x;
            starty = y;
            printf("You Clicked (%d,%d)\n",x,y);
            vec2 pos;
            pos = camera.GetViewportPosFromScreenPos(x,y);
            printf("You Clicked Viewport(%.2f,%.2f)\n",pos.x,pos.y);
            vec3 vrayorg,vraydir;
            camera.GetRayFromScreenPos(x,y,vrayorg,vraydir);
            printf("The ray dir is (%.2f,%.2f,%.2f) \n",vraydir.x,vraydir.y,vraydir.z);
        }
        if (state == GLUT_UP)
            Rotate = false;
    }
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            Translate = true;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP)
            Translate = false;
    }

    if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {
            Zoom = true;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP)
            Zoom = false;
    }

    if ((button == 3) || (button == 4)) {
        if (state == GLUT_UP) return;
        if (button == 3)
            camera.zoom(0.5f);
        else
            camera.zoom(-0.5f);
    }
}

void motion(int x, int y) {
    if (Translate) {
        float Trans_x = (x - startx) / 30.0f;
        float Trans_y = (y - starty) / 30.0f;
        vec3 left = camera.getLeft();
        vec3 up = camera.getUp();
        camera.translateAll(vec3(left.x * Trans_x, left.y * Trans_x, left.z * Trans_x));
        camera.translateAll(vec3(up.x * -Trans_y, up.y * -Trans_y, up.z * -Trans_y));
        startx = x;
        starty = y;
    }

    if (Zoom) {
        camera.zoom((float) (y - starty) / 10.0f);
        starty = y;
    }

    if (Rotate) {
        float sensitivity = 100.0f;
        float Rot = y - starty;
        vec3 tmp = camera.getPositionFromLookAt();
        tmp.y = tmp.x;
        tmp.x = -tmp.z;
        tmp.z = tmp.y;
        tmp.y = 0.0f;
        tmp = normalize(tmp);
        camera.rotate(Rot * sensitivity, vect3(tmp.x,tmp.y,tmp.z));

        Rot = x - startx;
        camera.rotate(-Rot * sensitivity, vect3(0.0f, 1.0f, 0.0f));

        startx = x;
        starty = y;
    }
    glutPostRedisplay();
}
void SampleTerrainTexture()
{
    glm::mat4 view,proj;
    view = camera.V;
    proj = camera.P;
    glm::mat4 viewproj = proj * view;
	// get current frame 
    g_pScreenFBO->Use();
    g_Terrain.Draw(viewproj);
    g_pScreenFBO->UnUse();
	GLuint tex = g_pScreenFBO->getTex();
	g_Terrain.SampleTexture(viewproj, tex, 1, 1);
	g_Terrain.SetFlipY(true);


}
void keydown(unsigned char keycode,
	int x, int y)
{
	printf("the key is trigger %c at (%d,%d)\n", keycode,x,y);
	switch(keycode)
    {
        case 's':
            printf("start to record!\n");
            SampleTerrainTexture();
            break;
		case 't':
			g_bdraw3d = !g_bdraw3d;
			break;
		case 'i':
			g_iTestPatchY++;
			break;
		case 'k':
			g_iTestPatchY--;
			break;
		case 'j':
			g_iTestPatchX--;
			break;
		case 'l':
			g_iTestPatchX++;
			break;
        default:
            break;
    }
	glutPostRedisplay();
    return;
}





void callback(GLenum source,
        GLenum type,
GLuint id,
        GLenum severity,
GLsizei length,
const GLchar *message,
const void *userParam)
{
    printf("info : %s\n",message);
    if (type == GL_DEBUG_TYPE_ERROR)
	{
		printf("error : %s\n",message);
		assert(false);
	}
}


//OpenGL initialization

void OnInit() {

    // Init for Debug
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	g_RenderDevice.Init();
    InitFullScreenQuad();
	
	glDebugMessageCallback(&callback, NULL);

    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	Mat mat;
	mat = imread("test.bmp", IMREAD_COLOR);
    glGenTextures(1,&g_testText);
    g_Terrain.SetPatchSize(32.0);
    g_TerrainSamp.SetPatchSize(16.0f);
	g_Terrain.Init();
   // g_TerrainSamp.Init();
	GL_CHECK_ERRORS
    glBindTexture(GL_TEXTURE_2D,g_testText);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,256,256,0,GL_RGB,GL_UNSIGNED_BYTE,0);
	GL_CHECK_ERRORS
	g_pRenderTextureFBO = new RenderTextureFBO(256, 256);
    g_pRenderTextureFBO->Init();
    g_pScreenFBO = new RenderTextureFBO();
    g_pScreenFBO->Init();
	GL_CHECK_ERRORS
	g_pScreenQuad = new CScreenSizeQuad();
	GL_CHECK_ERRORS
    g_Texture1.FillWithMat(mat);
	g_Terrain.setTexture(g_Texture1.m_iTextureIndex);
		//load the shader
	//compile and link shader
    g_zed3D.init();
		cout << "Initialization successfull" << endl;
}

//release all allocated resources
void OnShutdown() {
	//Destroy shader

	//Destroy vao and vbo
	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);
    delete g_pRenderTextureFBO;
    delete g_pScreenFBO;
    g_RenderDevice.Fini();
	cout << "Shutdown successfull" << endl;
}

//resize event handler
void OnResize(int w, int h) {
	//set the viewport size

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    windowWidth = w;
    windowHeight = h;
	//setup the projection matrix


	P = glm::ortho(-1, 1, -1, 1);
}

//display callback function

void OnRender() {
    glEnable(GL_DEPTH_TEST);

    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glActiveTexture(GL_TEXTURE0);
	glm::mat4 matView,matProj;
	matProj = glm::perspectiveRH(glm::radians(75.0f),windowWidth / (float)windowHeight,0.1f,1000.0f);

    camera.applyTransformations();
    camera.P = matProj;
    camera.show();
	matView = camera.V;//glm::lookAt(vec3(0.0f,20.0f,0.0f),vec3(0.0f,0.0f,1.0f),vec3(0.0f,1.0f,0.0f));4
    glm::mat4 matProjView;
    matProjView = matProj * matView;
    g_RenderDevice.SetViewProj(matView,matProj);
	g_Terrain.Draw(matProj * matView);
	vec2 corners2d[4];
	vec3 corners3d[4];
	g_Terrain.GetPatchWorldCorners(g_iTestPatchX,g_iTestPatchY,corners3d);
	g_Terrain.TestTransform(matProjView, corners3d, corners2d);
	//g_Terrain.TestDraw(matProjView);
	// blue,green,red,
	unsigned vertexcolor[] = { 0xffff0000,0xff00ff00,0xff0000ff,0xffffff00 };
	if(g_bdraw3d)
	{
		for (int i = 0; i < 4; ++i)
		{
			g_RenderDevice.Draw3DPoint(corners3d[i].x, corners3d[i].y, corners3d[i].z, vertexcolor[i]);
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			g_RenderDevice.Draw2DPoint(corners2d[i].x, corners2d[i].y, 0xffff0000);
		}
	}
//	g_RenderDevice.Draw3DPoint(0.0f,0.0f,0.0f, 0xffff0000);
//	g_RenderDevice.Draw2DPoint(0.5,0.9,0xff00ff00);
	//g_RenderDevice.Draw3DPoint(0.0,0.0,0.0,0xff00ff00);
    //g_zed3D.draw(matProjView);
	glutSwapBuffers();
	return;

}

int mainTestTri(int argc, char** argv) {
	//freeglut initialization calls
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("simple1");

    glutMouseFunc(mouse);
    glutMotionFunc(motion);
	glutKeyboardFunc(keydown);
	//glew initialization
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		cerr << "Error: " << glewGetErrorString(err) << endl;
	}
	else {
		if (GLEW_VERSION_3_3)
		{
			cout << "Driver supports OpenGL 3.3\nDetails:" << endl;
		}
	}
	err = glGetError(); //this is to ignore INVALID ENUM error 1282
	GL_CHECK_ERRORS

		//print information on screen
		cout << "\tUsing GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	GL_CHECK_ERRORS

		//opengl initialization
		OnInit();

	//callback hooks
	glutCloseFunc(OnShutdown);
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);

	//main loop call
	glutMainLoop();

	return 0;
}
