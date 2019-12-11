#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"

static GLfloat iX = 0;
static GLfloat iY = 0;
static GLfloat iZ = 0;

MeshRenderer::MeshRenderer(std::shared_ptr <Model> model, std::shared_ptr <ShaderProgram> program, std::shared_ptr<Texture> texture)
{
	m_model = model;
	m_program = program;
	m_texture = texture;
}

void MeshRenderer::OnUpdate(float deltaTime)
{
	//LOG_DEBUG("I'm Updating!");
}


void MeshRenderer::OnRender()
{
	m_program->Use();

	//set uniforms here!
	glm::mat4 model = m_entity->GetTransform()->GetTransformationMatrix();
	glm::mat4 mvp = Application::GetInstance()->GetCamera()->Get() * model;
	glm::mat4 view = Application::GetInstance()->GetCamera()->GetView();
	glm::vec3 viewPos = Application::GetInstance()->GetCamera()->GetParentTransform()->GetPosition();
	glm::mat4 projection = Application::GetInstance()->GetCamera()->GetProj();

	GLuint loc = glGetUniformLocation(m_program->Get(), "MVP");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(mvp));
	
	loc = glGetUniformLocation(m_program->Get(), "view");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(view));


	loc = glGetUniformLocation(m_program->Get(), "projection");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(projection));

	loc = glGetUniformLocation(m_program->Get(), "model");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(model));

	loc = glGetUniformLocation(m_program->Get(), "lightPos");
	glUniform3f(loc, 1.2f, 1.f, 2.0f);

	loc = glGetUniformLocation(m_program->Get(), "PosView");
	glUniform3f(loc, viewPos.x, viewPos.y, viewPos.z);

	loc = glGetUniformLocation(m_program->Get(), "objectColor");
	glUniform3f(loc, 1.0f, 1.f, 1.f);

	
	//loc = glGetUniformLocation(m_program->Get(), "ObjectPos");
	//glUniform3f(loc, m_entity->GetTransform()->GetPosition().x, m_entity->GetTransform()->GetPosition().y, m_entity->GetTransform()->GetPosition().z);

	m_texture->Bind();

	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();

		if (m_texture) m_texture->Bind();

		glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0);
	}

}

void MeshRenderer::OnAttach()
{

}

