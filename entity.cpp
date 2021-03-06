#include "entity.h"

entity::entity(int x,int y){
	p.set(x,y);
}

entity::~entity(){}

void entity::draw(){
	position pixel(XRATIO*p.x()+XRATIO/2,YRATIO*(MAX_MAZE-p.y())-YRATIO/2);
	glBindTexture(GL_TEXTURE_2D,tex);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glColor4f(0.0,0.0,0.0,1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex2f(pixel.x()-8,pixel.y()-8);
	glTexCoord2f(0,1); glVertex2f(pixel.x()-8,pixel.y()+8);
	glTexCoord2f(1,1); glVertex2f(pixel.x()+8,pixel.y()+8);
	glTexCoord2f(1,0); glVertex2f(pixel.x()+8,pixel.y()-8);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
}

bool entity::colision(entity e){
	return !(abs(p.x()-e.p.x())+abs(p.y()-e.p.y()));
}

void entity::move(int dir){
	position d((dir==RIGHT)-(dir==LEFT),(dir==DOWN)-(dir==UP));
	p=p+d;
}

void entity::load_tex(const char *filepath){
	if(filepath) tex=load_texture(filepath);
}

void entity::clear_tex(){
	glDeleteTextures(1,&tex);
	tex=0;
}

//-------------------------------------------------------------------

player::player(int y):entity(0,y){}

player::~player(){}

void player::move(int dir, mapa m){
	if(m.can_move(pos(),dir)) {
		entity::move(dir);
	}
}

void player::load_tex(){
	entity::load_tex("textures/player/player.bmp");
}

bool player::win(int x,int y){
	entity e(x,y);
	return entity::colision(e);
}
