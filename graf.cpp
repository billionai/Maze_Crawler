#include "graf.h"
#include <stdio.h>

int height=HEIGHT,width=WIDTH;

int h(){
	return height;
}

int w(){
	return width;
}

void reshape(GLsizei l, GLsizei a){
	glViewport(0,0,l,a);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,l,0,a);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	height=l;
	width=a;
}

void timer(int value){
	int t=10;//milisseconds
	glutPostRedisplay();
	glutTimerFunc(t,timer,0);
}

void screen_start(int* argc,char** argv){
	glutInit(argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(HEIGHT,WIDTH);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Maze Crawler");
	glutReshapeFunc(reshape);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1);
	glDisable(GL_LIGHTING);
}

void screen_set(void (*display)(void),void (*key)(unsigned char,int,int),void (*mouse)(int,int,int,int)){
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
}

unsigned int load_texture(const char* filename){
	return SOIL_load_OGL_texture(filename,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
}

//--------------------------------------------------------------------------

button::button(){}

button::~button(){
	if(tex)glDeleteTextures(1,&tex);
}

bool button::click(int x,int y){
	return (top_left.x()<=x && x<=bottom_right.x()) && (top_left.y()<=(900-y) && (900-y)<=bottom_right.y());
}

void button::set(int t,int top,int bottom,int left,int right){
	type=t;
	top_left.set(left,top);
	bottom_right.set(right,bottom);
	if(t==0) tex=load_texture("textures/button/sair.bmp");
	if(t==1) tex=load_texture("textures/button/jogar.bmp");
}

void button::draw(){
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glBindTexture(GL_TEXTURE_2D,tex);
	glColor4f(0.0f,0.0f,0.0f,0.5f);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex2f(top_left.x(),top_left.y());
	glTexCoord2f(0,1); glVertex2f(top_left.x(),bottom_right.y());
	glTexCoord2f(1,1); glVertex2f(bottom_right.x(),bottom_right.y());
	glTexCoord2f(1,0); glVertex2f(bottom_right.x(),top_left.y());
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
}
