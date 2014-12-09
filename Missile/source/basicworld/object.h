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

#ifndef OBJECT_H
#define OBJECT_H
#include <Eigen/Core>

struct SObjectDesc
{
	float lin_resistance;
	float rot_resistance;
	
	SObjectDesc() = delete;
	SObjectDesc(SObjectDesc const& other) = default;
	SObjectDesc(float mass, float rcoef = 1): lin_resistance(mass), rot_resistance(mass * rcoef)
	{
	}
	
	SObjectDesc& operator= (SObjectDesc const& other) = default;
};

struct SObjectState
{
	Eigen::Vector2f lin_position;
	Eigen::Vector2f lin_velocity;
	float rot_position;
	float rot_velocity;
	
	SObjectState(SObjectState const& other) = default;
	SObjectState(Eigen::Vector2f position = Eigen::Vector2f::Zero(), float rotation = 0, Eigen::Vector2f velocity = Eigen::Vector2f::Zero(), float omega = 0):
		lin_position(position), lin_velocity(velocity), rot_position(rotation), rot_velocity(omega)
	{
	}

	SObjectState& operator= (SObjectState const& other) = default;

	Eigen::Matrix2f getMatrix() const
	{
		Eigen::Matrix2f m;
		m <<
			cos(rot_position), -sin(rot_position),
			sin(rot_position), cos(rot_position);
		return m;
	}

	Eigen::Matrix2f getInvMatrix() const
	{
		Eigen::Matrix2f m;
		m <<
			cos(rot_position), sin(rot_position),
			-sin(rot_position), cos(rot_position);
		return m;
	}
};

class CObject
{
private:
	class CBasicWorld* world;
	Eigen::Vector2f applied_force;
	float applied_torque;

protected:
	SObjectDesc desc;
	SObjectState state;
	Eigen::Matrix2f rot_position_matrix; // relative-to-absolute
	Eigen::Matrix2f rot_position_inv_matrix; // absolute-to-relative

public:
	void applyForceRel(Eigen::Vector2f force, Eigen::Vector2f point);
	void applyForceAbs(Eigen::Vector2f force, Eigen::Vector2f point);

protected:
	virtual void applyInternalForces() {}

private:
	void processForces(float dt);
	void processVelocities(float dt);
	void makeRotPositionMatrices();

public:
	CObject(class CBasicWorld* world, SObjectDesc const& desc, SObjectState const& state);
	CObject(CObject const& other) = delete;
	virtual ~CObject();

	void beforeStep();
	void afterStep(float dt);

	class CBasicWorld* getWorld() const
	{
		return world;
	}
	
	SObjectDesc const& getDesc() const
	{
		return desc;
	}
	
	SObjectState const& getState() const
	{
		return state;
	}

	SObjectState getState(CObject const& base) const
	{
		SObjectState s(state);
		SObjectState b(base.state);
		Eigen::Matrix2f m(base.getInvMatrix());
		s.lin_position = m * (s.lin_position - b.lin_position);
		s.lin_velocity = m * (s.lin_velocity - b.lin_velocity);
		s.rot_position -= b.rot_position;
		return s;
	}

	float getRCoef() const
	{
		return desc.rot_resistance / desc.lin_resistance;
	}

	Eigen::Vector2f getPosition(CObject const& base) const
	{
		return base.getInvMatrix() * (state.lin_position - base.state.lin_position);
	}

	Eigen::Vector2f getVelocity(CObject const& base) const
	{
		return base.getInvMatrix() * (state.lin_velocity - base.state.lin_velocity);
	}

	float getRotation(CObject const& base) const
	{
		return state.rot_position - base.state.rot_position;
	}

	float __attribute__((deprecated)) getRVelocity(CObject const& base) const
	{
		return state.rot_velocity;
	}

	float getRVelocity() const
	{
		return state.rot_velocity;
	}

	Eigen::Matrix2f const& getMatrix() const
	{
		return rot_position_matrix;
	}

	Eigen::Matrix2f const& getInvMatrix() const
	{
		return rot_position_inv_matrix;
	}
};

#endif // OBJECT_H
