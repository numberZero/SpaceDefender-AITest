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

#include "object.h"
#include "world.h"
#include <Eigen/Geometry>

CObject::CObject(class CBasicWorld* world, SObjectDesc const& desc, SObjectState const& state):
	world(world), desc(desc), state(state)
{
	makeRotPositionMatrices();
	world->addObject(this);
}

CObject::~CObject()
{
	world->removeObject(this);
}

void CObject::beforeStep()
{
	applied_force.setZero();
	applied_torque = 0;
}

void CObject::afterStep(float dt)
{
	applyInternalForces();
	processForces(dt);
	processVelocities(dt);
}

void CObject::processVelocities(float dt)
{
	state.lin_position += state.lin_velocity * dt;
	state.rot_position += state.rot_velocity * dt;
	makeRotPositionMatrices();
}

void CObject::processForces(float dt)
{
	state.lin_velocity += rot_position_matrix * (applied_force * dt / desc.lin_resistance);
	state.rot_velocity += applied_torque * dt / desc.rot_resistance;
}

void CObject::applyForceAbs(Eigen::Vector2f force, Eigen::Vector2f point)
{
	applyForceRel(rot_position_inv_matrix * force, rot_position_inv_matrix * (point - state.lin_position));
}

void CObject::applyForceRel(Eigen::Vector2f force, Eigen::Vector2f point)
{
	applied_force += force;
	applied_torque += point.dot(Eigen::Vector2f(force[1], -force[0]));
/*
	Eigen::Hyperplane<float, 2> plane(Eigen::Hyperplane<float, 2>::Through(point, point + force));
	applied_force += force;
	applied_torque += force.norm() * plane.signedDistance(Eigen::Vector2f::Zero());
*/
}

void CObject::makeRotPositionMatrices()
{
	rot_position_matrix = state.getMatrix();
	rot_position_inv_matrix = state.getInvMatrix();
}
