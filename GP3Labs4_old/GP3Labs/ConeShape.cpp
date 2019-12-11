#include "pch.h"
#include "ConeShape.h"
ConeShape::ConeShape(glm::float32 radius, glm::float32 height)
{
	m_radius = radius;
	m_height = height;
	m_shape = new btConeShape(btScalar(m_radius), btScalar(m_height));// TODO ask kirien if its appropriate
	m_type = CShapeType::CONE;
}
