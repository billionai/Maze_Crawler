#ifndef MAPA
#define MAPA
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stack>
#include <vector>
#include "position.h"
#include "graf.h"

#define UP 1
#define DOWN 2
#define RIGHT 4
#define LEFT 8
#define MAX_MAZE 15
#define XRATIO h()/MAX_MAZE
#define YRATIO w()/MAX_MAZE

class tile{
private:
	bool disc,item;
	int con;//to which tiles around it this one connects
	GLuint tex;
public:
	tile();
	~tile();
	void print();
	void discover();
	void add(int);
	void add_item();
	bool connected(int);
	bool dead_end();
	bool d(){return disc;}
	int c(){return con;}
	void texture_update();
	void draw(int,int);
};

class mapa{
private:
	tile t[MAX_MAZE][MAX_MAZE];
	int last_exit;
public:
	mapa();
	~mapa();
	int size(){return MAX_MAZE;}
	int exit(){return last_exit;}
	bool can_move(position,int);
	void print();
	void generate();
	void draw();
	void discover(position);
};

void map_set_tex();

void map_clear_tex();

#endif
