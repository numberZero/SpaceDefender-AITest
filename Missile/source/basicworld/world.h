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

#ifndef WORLD_H
#define WORLD_H
#include <set>
#include <stdexcept>

class CBasicWorld
{
private:
	std::set<class CObject*> objects;

public:
	CBasicWorld();
	CBasicWorld(const CBasicWorld& other);
	virtual ~CBasicWorld();
	
	void addObject(CObject* obj);
	void removeObject(CObject* obj);
	void prepare();	
	void step(float dt);
};

#endif // WORLD_H
