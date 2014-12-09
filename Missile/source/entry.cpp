/*
    “Missile AI test” game
    Copyright (C) 2014  Виталий zero@numzero.tk

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <iostream>
#include <ctime>

#define PROGRAM_NAME "Test1"

SDL_Window *mainwindow;
SDL_GLContext maincontext;
// GLuint drawlist;

void loop();

void randomize()
{
	timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	srand(t.tv_nsec ^ t.tv_sec);
}

void initSDL()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	mainwindow = SDL_CreateWindow(PROGRAM_NAME,
								  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								  1024, 768,
								  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	maincontext = SDL_GL_CreateContext(mainwindow);
	SDL_GL_SetSwapInterval(1);
}

void doneSDL()
{
	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();
}

void initGL()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(10000.0);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

//	drawlist = glGenLists(1);
}

int main(int argc, char *argv[])
{
	randomize();
	initSDL();
	initGL();
	loop();
	doneSDL();
	return 0;
}
