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

#include "draw.h"
#include "basicworld/object.h"

extern CObject* missile;
extern CObject* target;
extern SDL_Window *mainwindow;
extern const float r;
extern const float bfr;

void beforeDraw(CObject* object)
{
	glPushMatrix();
	glTranslatef(object->getState().lin_position(0), object->getState().lin_position(1), 0);
	glRotatef(object->getState().rot_position * 180 / M_PI, 0, 0, 1);
}

void afterDraw()
{
	glPopMatrix();
}

void drawMissile(CObject* missile)
{
	beforeDraw(missile);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.01, -0.02);
	glVertex2f(+0.01, -0.02);
	glVertex2f(0.00, +0.02);
	glEnd();
	afterDraw();
}

void drawTarget(CObject* target)
{
	beforeDraw(target);
	glBegin(GL_LINES);
	glVertex2f(-r, 0);
	glVertex2f(r, 0);
	glVertex2f(0, -r);
	glVertex2f(0, r);
	glEnd();
	glBegin(GL_LINE_LOOP);
	const long n(32);
	for(long i(0); i < n; ++i)
	{
		float a(2 * M_PI * i / n);
		glVertex2f(r * sin(a), r * cos(a));
	}
	glEnd();
	afterDraw();
}

void scenePrepare()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glScalef(0.6, 0.8, 0.8);
	glScalef(0.5, 0.5, 0.5);
}

void sceneDraw()
{
	glColor4f(1.0, 1.0, 1.0, 1.0);
	drawTarget(target);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	drawMissile(missile);
}

void sceneFinish()
{
	glLineWidth(1);
	glLineStipple(2, 0x0FF0);
	glEnable(GL_LINE_STIPPLE);
	glColor4f(1.0, 1.0, 0.0, 0.5);
	glBegin(GL_LINES);
	glVertex2f(-1, 0);
	glVertex2f(+1, 0);
	glVertex2f(0, -1);
	glVertex2f(0, +1);
	glEnd();
	glBegin(GL_LINE_LOOP);
	const long n(128);
	for(long i(0); i < n; ++i)
	{
		float a(2 * M_PI * i / n);
		glVertex2f(bfr * sin(a), bfr * cos(a));
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	
	glLoadIdentity();
	glColor4f(0.8, 0.8, 0.8, 0.9);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(-0.6, 0.8);
	glVertex2f(-1.0, 1.0);
	glVertex2f(0.6, 0.8);
	glVertex2f(1.0, 1.0);
	glVertex2f(0.6, -0.8);
	glVertex2f(1.0, -1.0);
	glVertex2f(-0.6, -0.8);
	glVertex2f(-1.0, -1.0);
	glVertex2f(-0.6, 0.8);
	glVertex2f(-1.0, 1.0);
	glEnd();

	SDL_GL_SwapWindow(mainwindow);
}
