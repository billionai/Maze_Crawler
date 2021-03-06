#include "mapa.h"

using namespace std;

GLuint tex_IDs[16];

tile::tile(){
	disc=con=item=0;
}

tile::~tile(){}

bool tile::connected(int dir){
	return con & dir;
}

bool tile::dead_end(){
	return (connected(UP)+connected(DOWN)+connected(LEFT)+connected(RIGHT))==1;
}

void tile::print(){
	printf("%d %d%d%d%d\t",disc,connected(LEFT),connected(RIGHT),connected(DOWN),connected(UP));
}

void tile::discover(){
	disc=!disc;
}

void tile::add(int dir){
	con+=dir;
}

void tile::add_item(){
	item=1;
}

void tile::texture_update(){
	tex=tex_IDs[con];
}

void tile::draw(int x,int y){
	position p(XRATIO*x,YRATIO*(MAX_MAZE-y));
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glBindTexture(GL_TEXTURE_2D,tex);
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex2f(p.x(),(p.y()-YRATIO));
	glTexCoord2f(0,1); glVertex2f(p.x(),p.y());
	glTexCoord2f(1,1); glVertex2f((p.x()+XRATIO),p.y());
	glTexCoord2f(1,0); glVertex2f((p.x()+XRATIO),(p.y()-YRATIO));
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	if(!item) return;
	glColor4f(0.0f,1.0f,1.0f,1.0f);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2f(p.x()+XRATIO/2,p.y()-YRATIO/2);
	glEnd();
}

//--------------------------------------------------------------------

mapa::mapa(){
	last_exit=0;
}

mapa::~mapa(){}

bool mapa::can_move(position p,int dir){
	return t[p.x()][p.y()].connected(dir);
}

void mapa::print(){
	int i,j;
	for(i=0;i<MAX_MAZE;i++){
		for(j=0;j<MAX_MAZE;j++){
			t[i][j].print();
		}
		printf("\n");
	}
	printf("\n");
}

void mapa::generate(){
	int i,j;
	position p,dir[4];
	stack<position> s;
	vector<int> unv;//nvisited directions
	i=rand()%MAX_MAZE;
	j=rand()%MAX_MAZE;
	dir[0].set(0,-1);//up
	dir[1].set(0,1);//down
	dir[2].set(1,0);//right
	dir[3].set(-1,0);//left
	p.set(i,j);
	s.push(p);
	t[i][j].discover();
	while(!s.empty()){
		p=s.top();
		//mark unvisited neighbours
		if(p.y() > 0 && !t[p.x()][p.y()-1].d()) unv.push_back(0);//up
		if(p.y() < MAX_MAZE-1 && !t[p.x()][p.y()+1].d()) unv.push_back(1);//down
		if(p.x() < MAX_MAZE-1 && !t[p.x()+1][p.y()].d()) unv.push_back(2);//right
		if(p.x() > 0 && !t[p.x()-1][p.y()].d()) unv.push_back(3);//left
		
		if(!unv.empty()){
			i=rand()%unv.size();
			t[p.x()][p.y()].add(1<<unv[i]);
			p=p+dir[unv[i]];
			if(unv[i]==0 || unv[i]==2) t[p.x()][p.y()].add(1<<(unv[i]+1));
			else t[p.x()][p.y()].add(1<<(unv[i]-1));
			t[p.x()][p.y()].discover();
			s.push(p);
		}else{
			s.pop();
			i=rand()%100;
			if(i<30 && t[p.x()][p.y()].dead_end())t[p.x()][p.y()].add_item();
		}
		unv.clear();
	}
	t[0][last_exit].add(LEFT);
	t[0][last_exit].discover();
	last_exit=rand()%MAX_MAZE;
	t[MAX_MAZE-1][last_exit].add(RIGHT);
	t[MAX_MAZE-1][last_exit].discover();
	for (i=0;i<MAX_MAZE;i++)
		for (j=0;j<MAX_MAZE;j++){
			t[i][j].texture_update();
			t[i][j].discover();
		}
}

void mapa::draw(){
	int i,j;
	for(i=0;i<MAX_MAZE;i++)
		for(j=0;j<MAX_MAZE;j++)
			if(t[i][j].d())t[i][j].draw(i,j);
}

void mapa::discover(position p){
	if(!t[p.x()][p.y()].d()) t[p.x()][p.y()].discover();
}

//--------------------------------------------------------------------------

void map_set_tex(){
	tex_IDs[0]=load_texture("textures/background/black.bmp");
	tex_IDs[1]=load_texture("textures/background/1-side-up.bmp");
	tex_IDs[2]=load_texture("textures/background/1-side-down.bmp");
	tex_IDs[3]=load_texture("textures/background/2-side-up-down.bmp");
	tex_IDs[4]=load_texture("textures/background/1-side-right.bmp");
	tex_IDs[5]=load_texture("textures/background/2-side-up-right.bmp");
	tex_IDs[6]=load_texture("textures/background/2-side-down-right.bmp");
	tex_IDs[7]=load_texture("textures/background/3-side-up-down-right.bmp");
	tex_IDs[8]=load_texture("textures/background/1-side-left.bmp");
	tex_IDs[9]=load_texture("textures/background/2-side-up-left.bmp");
	tex_IDs[10]=load_texture("textures/background/2-side-down-left.bmp");
	tex_IDs[11]=load_texture("textures/background/3-side-up-down-left.bmp");
	tex_IDs[12]=load_texture("textures/background/2-side-right-left.bmp");
	tex_IDs[13]=load_texture("textures/background/3-side-up-left-right.bmp");
	tex_IDs[14]=load_texture("textures/background/3-side-down-right-left.bmp");
	tex_IDs[15]=load_texture("textures/background/4-side.bmp");
}

void map_clear_tex(){
	glDeleteTextures(16,tex_IDs);
}
