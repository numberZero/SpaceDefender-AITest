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

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <iostream>
#include <thread>
#include <ctime>
#include "basicworld/world.h"
#include "basicworld/object.h"
#include "draw.h"

extern const float r = 0.1;
extern const float bfr = 2.5;
using namespace std;

//extern GLuint drawlist;

CBasicWorld world;
CObject* missile(nullptr);
CObject* target(nullptr);

long hits(0);
long misses(0);
double time_from_calc;

long time_ftl(float t, float dt)
{
	return t / dt;
}

float time_ltf(long n, float dt)
{
	if(n == LONG_MIN)
		return NAN;
	return n * dt;
}

void workMissile(CObject* missile, bool left, bool right)
{
	if(left)
		missile->applyForceRel(Eigen::Vector2f(0, 1), Eigen::Vector2f(-0.02, -0.05));
	if(right)
		missile->applyForceRel(Eigen::Vector2f(0, 1), Eigen::Vector2f(0.02, -0.05));
}

long work(long t, float dt)
{
	CBasicWorld simulation;
	SObjectState mstate;
	mstate.rot_velocity = ::missile->getRVelocity();
	CObject missile(&simulation, ::missile->getDesc(), mstate);
	CObject target(&simulation, ::target->getDesc(), ::target->getState(*::missile));
	long n = std::abs(t);
	for(long i(0); i < n; ++i)
	{
		simulation.prepare();
		workMissile(&missile, t < 0, t > 0);
		simulation.step(dt);
	}
	Eigen::Vector2f rel;
	for(long i(0); i < 1000; ++i)
	{
		simulation.prepare();
		workMissile(&missile, missile.getRVelocity() >= 0, missile.getRVelocity() <= 0);
		simulation.step(dt);
		rel = target.getPosition(missile);
		if(rel.squaredNorm() < r * r * 0.25)
			return 0;
		if(rel[1] < 0)
			break;
	}
	return (rel[0] > 0) ? 1 : -1;
}

float calc(float base = 0.0, float spread = 1.0, float dt = 0.01, long lim = 100, long rlim = 0)
{
	long na = time_ftl(base - spread, dt);
	long nb = time_ftl(base + spread, dt);
	long nc = LONG_MIN;
	long ra = work(na, dt);
	long rb = work(nb, dt);
	if(rlim < 0)
		rlim = lim;
	for(long i(0); i < rlim; ++i)
	{
		if(ra != rb)
			break;
		nc = (na + nb) / 2;
		long r = na - nb;
		na = nc - r;
		nb = nc + r;
		ra = work(na, dt);
		rb = work(nb, dt);
		std::cout << "Extending binsearch to [" << na << "; " << nb << "]\n";
	}
	if(ra && rb && (ra != rb))
	{
		for(long i(0); i < lim; ++i)
		{
			nc = (na + nb) / 2;
			if((nc == na) || (nc == nb))
				break; // precision is limited
			long rc = work(nc, dt);
			if(rc == 0)
				break;
			if(rc == ra)
				na = nc;
			else if(rc == rb)
				nb = nc;
			else
				throw "Invalid binsearch callback return value";
		}
	}
	else
	{
		if(ra == 0)
			nc = na;
		if(rb == 0)
			nc = nb;
	}
	return time_ltf(nc, dt);
}

float t;
bool k;

void recalc()
{
	time_from_calc = 0;
	float t2 = calc(0.0, 0.8);
	if(isnan(t2))
		return;
	if(target->getPosition(*missile).norm() < 8 * r)
	{
		float t1 = calc(t2, 0.02, 0.001);
		t = isnan(t1) ? t2 : t1;
	}
	else
		t = t2;
	k = t > 0;
	t = std::abs(t);
}

void resetObjects()
{
	if(missile)
		delete missile;
	if(target)
		delete target;
	missile = new CObject(&world, SObjectDesc(1, 0.01), SObjectState(Eigen::Vector2f(0, -1.5), 0));
	target = new CObject(&world, SObjectDesc(1, 1), SObjectState(
		Eigen::Vector2f(random() * (2.0 / RAND_MAX) - 1.0, random() * (1.0 / RAND_MAX)), 0,
		Eigen::Vector2f(random() * (2.0 / RAND_MAX) - 1.0, random() * (2.0 / RAND_MAX) - 1.0) * 0.3, 2 * M_PI
		));
	t = NAN;
	recalc();
}

bool processEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			return false;
		case SDL_MOUSEBUTTONDOWN:
			resetObjects();
			break;
		}
	}
	return true;
}

void loop()
{
	resetObjects();
	timespec t1, t2;
	double dt;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
	while(processEvents())
	{
		clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
		dt = 0.01;//(t2.tv_sec - t1.tv_sec) + 1e-9 * (t2.tv_nsec - t1.tv_nsec);
		t1 = t2;
		
		scenePrepare();
		sceneDraw();
		sceneFinish();

		t -= dt;
		time_from_calc += dt;
		if(time_from_calc >= 0.5)
			recalc();

		world.prepare();
		if(t >= 0)
			workMissile(missile, !k, k);
		else
		{
			float rvel = 0;
			if(isnan(t))
			{
				Eigen::Vector2f rel = target->getPosition(*missile);
				rvel = ((rel[1] < 0) ? (missile->getRVelocity() < 0) : (rel[0] > 0)) ? -6 : 6;
			}
			workMissile(missile, missile->getRVelocity() >= rvel, missile->getRVelocity() <= rvel);
		}
		world.step(dt);
		
		if((missile->getState().lin_position.norm() > bfr) || (target->getState().lin_position.norm() > bfr))
		{
			++misses;
			resetObjects();
		}

		if(target->getPosition(*missile).norm() < r)
		{
			++hits;
			resetObjects();
		}
	}
}
