#include "pch.h"
#include "CylinderShape.h"
CylinderShape::CylinderShape(glm::vec3 halfExtents)
{
	m_halfExtents = halfExtents;
	m_shape = new btCylinderShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));// TODO ask kirien if its appropriate
	m_type = CShapeType::CYLINDER;
}
