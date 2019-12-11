#include "pch.h"
#include "SphereShape.h"
SphereShape::SphereShape(glm::float32 radius)
{
	m_radius = radius;
	m_shape = new btSphereShape(btScalar(m_radius));// TODO ask kirien if its appropriate
	m_type = CShapeType::SPHERE;
}
