/*
    “Missile AI test” game
    Copyright (C) 2014  Виталий silverunicorn2011@yandex.com

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

#ifdef _WIN32
#include <windows.h>
#endif
#include <ctime>
#include "system.h"
#include <SDL2/SDL.h>
#include <GL/gl.h>

#define PROGRAM_NAME "Test1"

#if _POSIX_C_SOURCE >= 199309L
double GetTime()
{
	timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	return t.tv_sec + 1e-9 * t.tv_nsec;
}
#else
#ifdef _WIN32
double GetTime()
{
	LARGE_INTEGER t, f;
	QueryPerformanceCounter(&t);
	QueryPerformanceFrequency(&f);
	return (double)t.QuadPart / f.QuadPart;
}
#else
#ifdef _BSD_SOURCE
#include <sys/time.h>
double GetTime()
{
	timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec + 1e-6 * t.tv_usec;
}
#else
#include <sys/timeb.h>
double GetTime()
{
	timeb t;
	ftime(&t);
	return t.time + 1e-3 * t.millitm;
}
#endif
#endif
#endif

#if _POSIX_C_SOURCE >= 199309L
void Randomize()
{
	timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	srand(t.tv_nsec ^ t.tv_sec);
}
#else
#ifdef _WIN32
void Randomize()
{
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	srand(t.HighPart ^ t.LowPart);
}
#else
void Randomize()
{
	srand(time(0));
}
#endif
#endif

SDL_Window *mainwindow;
SDL_GLContext maincontext;
// GLuint drawlist;

void loop();

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

#ifdef _WIN32
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
int main(int argc, char *argv[])
#endif
{
	Randomize();
	initSDL();
	initGL();
	loop();
	doneSDL();
	return 0;
}
