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

#include "world.h"
#include "object.h"

CBasicWorld::CBasicWorld()
{

}

CBasicWorld::CBasicWorld(const CBasicWorld& other)
{

}

CBasicWorld::~CBasicWorld()
{

}

void CBasicWorld::addObject(CObject* obj)
{
	if(obj->getWorld() != this)
		throw std::runtime_error(std::string(__func__));// "Can't add object from another world"));
	objects.insert(obj);
}

void CBasicWorld::removeObject(CObject* obj)
{
	if(obj->getWorld() != this)
		throw std::runtime_error(std::string(__func__));// "Can't remove object from another world"));
	objects.erase(obj);
}

void CBasicWorld::prepare()
{
	for(auto i(objects.begin()); i != objects.end(); ++i)
		(**i).beforeStep();
}

void CBasicWorld::step(float dt)
{
	for(auto i(objects.begin()); i != objects.end(); ++i)
		(**i).afterStep(dt);
}
