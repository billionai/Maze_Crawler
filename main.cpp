#include "entity.h"

using namespace std;

mapa m;
player p(0);
button b[2];
GLuint tex;

void keyDown_dummy(unsigned char key,int x,int y){}

void mouse_dummy(int button,int stateMouse,int x,int y){}

void keyDown_end(unsigned char key, int x,int y){
	if(key=='q' || key=='Q')
		exit(0);
}

void display_end(){
	p.clear_tex();
	map_clear_tex();
	tex=load_texture("textures/end_game.bmp");
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0,0.0,0.0,1.0);
	glBindTexture(GL_TEXTURE_2D,tex);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex2f(0,0);
	glTexCoord2f(0,1); glVertex2f(0,w());
	glTexCoord2f(1,1); glVertex2f(h(),w());
	glTexCoord2f(1,0); glVertex2f(h(),0);
	glEnd();
	render();
}

void keyDown_game(unsigned char key, int x,int y){
	int dir;
	if(key=='w' || key=='W') dir=UP;
	if(key=='s' || key=='S') dir=DOWN;
	if(key=='a' || key=='A') dir=LEFT;
	if(key=='d' || key=='D') dir=RIGHT;
	p.move(dir,m);
	if(p.win(m.size(),m.exit())) screen_set(display_end,keyDown_end,mouse_dummy);
	m.discover(p.pos());
	glutPostRedisplay();
}

void display_game(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	m.draw();
	p.draw();
	render();
}

void mouse_menu(int button,int stateMouse,int x,int y){
	if(button==GLUT_LEFT_BUTTON && stateMouse==GLUT_DOWN){
		if(b[0].click(x,y)) 
			exit(0);
		if(b[1].click(x,y)){
			m.generate();
			screen_set(display_game,keyDown_game,mouse_dummy);
			glutPostRedisplay();
		}
	}
}

void display_menu(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	b[0].draw();
	b[1].draw();
	render();
}

void menu(){
	screen_set(display_menu,keyDown_dummy,mouse_menu);
	b[0].set(0,100,132,100,228);
	b[1].set(1,100,132,1000,1128);
	glutMainLoop();
}

int main(int argc,char** argv){
	srand(time(NULL));
	screen_start(&argc,argv);
	map_set_tex();
	p.load_tex();
	menu();
	return 0;
}
