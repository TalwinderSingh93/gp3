#include "Physics.h"
#include "BoxShape.h"
#include "SphereShape.h"
#include "CapsuleShape.h"
#include "CylinderShape.h"
#include "ConeShape.h"
#include "RigidBody.h"
#include "pch.h"
#include "Application.h"
#include "Common.h"
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"

//Test
btVector3 btvector;


Application* Application::m_application = nullptr;
Application::Application()
{
}
void Application::Init()
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError(), Log::Error);
		exit(-1);
	}
	//setting openGL version to 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	//creating window
	m_window = SDL_CreateWindow("GP3-GAME", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight,
		SDL_WINDOW_OPENGL);

	SDL_CaptureMouse(SDL_TRUE);
	//TODO: by scottolando!!
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	OpenGlInit();
	GameInit();

}
void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls
	//will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();
	SDL_GL_SetSwapInterval(1);
	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) <<
			std::endl;
		exit(-1);
	}

	//Smooth shading
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));

	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));

	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));

	//enabling blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//turn on back face culling
	//GL_ATTEMPT(glEnable(GL_CULL_FACE));
	//GL_ATTEMPT(glCullFace(GL_BACK));

	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);
}

void Application::GameInit()
{

	//loading all resources
	Resources::GetInstance()->AddModel("cube.obj");
	Resources::GetInstance()->AddModel("stickman.obj");
	Resources::GetInstance()->AddModel("spider.obj");
	//
	
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddShader(std::make_shared <ShaderProgram>(ASSET_PATH + "BlinnPhong_VERT.glsl", ASSET_PATH + "BlinnPhong_FRAG.glsl"), "blinn");
	Resources::GetInstance()->AddShader(std::make_shared <ShaderProgram>(ASSET_PATH + "simple_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"), "simple");
	
	Entity* a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(
		Resources::GetInstance()->GetModel("cube.obj"),
		Resources::GetInstance()->GetShader("simple"),
		Resources::GetInstance()->GetTexture("Wood.jpg"))
	);
	MeshRenderer* m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -10.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.f,
		100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));

	a = new Entity();
	m_entities.push_back(a);
	CameraComp* cc = new CameraComp();
	a->AddComponent(cc);
	//cc->Start();

	//Camera position and rotation
	m_entities.back()->GetTransform()->SetPosition(glm::vec3(0, 0, 0));

	for (int i = 0; i < 100; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("cube.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Wood.jpg"))
		);
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -20.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f,1.f, 1.f)));
		//a->GetComponent<RigidBody>()->Init(new SphereShape(1.0f));
		a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
	}
	

	/*
	//loading all resources
	Resources::GetInstance()->AddModel("cube.obj");
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "simple_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"),"simple");

	//Students should aim to have a better way of managing the scene for coursework
	m_entities.push_back(new Entity());
	m_entities.at(0)->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Wood.jpg"))
		);
			m_entities.at(0)->GetTransform()->SetPosition(glm::vec3(0, 0, -10));
			m_entities.at(0)->GetTransform()->SetScale(glm::vec3(0.05, 0.05, 0.05));

	m_entities.push_back(new Entity());
	CameraComp* cc = new CameraComp();
	m_entities.at(1)->AddComponent(cc);
	cc->Start();
	*/
	
}

void Application::Loop()
{
	m_appState = AppState::RUNNING;
	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	

	while (m_appState != AppState::QUITTING)
	{
		
		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Resources::GetInstance()->ReleaseResources();
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				Resources::GetInstance()->ReleaseResources();
				m_appState = AppState::QUITTING;
				break;
			case SDL_KEYDOWN:

				//TODO ask kirein
				INPUT->SetKey(event.key.keysym.sym, true);

				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						//Resources::GetInstance()->ReleaseResources();
						
						m_appState = AppState::QUITTING;
					break;

					/*
					//TODO: add camera movement and models movement
					case SDLK_a:

						m_entities.front()->GetTransform()->RotateEulerAxis(35, glm::vec3(0, 1, 0));

						//GetKeyDown(SDLK_a);
				
					break;
					*/

					

					//TODO
					case SDLK_UP:

						for (int iCount = 2; iCount < m_entities.size(); iCount++)
							m_entities.at(iCount)->GetComponent<RigidBody>()->ApplyForce();

					break;

					case SDLK_LEFT:

					for (int iCount = 2; iCount < m_entities.size(); iCount++)
						m_entities.at(iCount)->GetComponent<RigidBody>()->ApplyTorque();

					break;

					/*
					case SDLK_d:

						m_entities.front()->GetTransform()->RotateEulerAxis(35, glm::vec3(0, 1, 0));

					break;
					*/
					case SDLK_SPACE:

						if (INPUT->GetKeyDown(event.key.keysym.sym))
						{
							LOG_DEBUG("Space pressed this frame", Log::Trace);
						}
						


					break;

				}
				
			
			break;
				//record when the user releases a key
			case SDL_KEYUP:

				//TODO ask kirein
				INPUT->SetKey(event.key.keysym.sym, false);

				switch (event.key.keysym.sym)
				{
				
				case SDLK_a:

					//GetKeyUp(SDLK_a);
					LOG_DEBUG("Key UP", Log::Trace);

					break;

				case SDLK_SPACE:

				if (INPUT->GetKeyUp(event.key.keysym.sym))
				{
					LOG_DEBUG("Space released this frame", Log::Trace);
				}

				break;

				}

				break;
			case SDL_MOUSEMOTION:
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				glm::ivec2 mouseMovement = INPUT->GetMouseDelta();

				//TODO: create a variable to decrease the camera velocity
				m_entities.at(1)->GetTransform()->RotateEulerAxis(mouseMovement.x*(0.8f), glm::vec3(0, 1, 0));
				m_entities.at(1)->GetTransform()->RotateEulerAxis(mouseMovement.y*(0.8f), m_entities.at(1)->GetTransform()->GetRight());// );
				
				//LOG_DEBUG(std::to_string(mouseMovement.x), Log::Trace);


				break;
			case SDLK_ESCAPE:
				Quit();
				break;

			}
		}

		AllMovements();

		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime =
			(float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks -
				prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;
		Physics::GetInstance()->Update(deltaTime);

		Resources::GetInstance()->ReleaseUnusedResourses();
		// update and render all entities
		Update(deltaTime);
		Render();
		SDL_GL_SwapWindow(m_window);

		//TODO: print delta time using log
		//LOG_DEBUG(std::to_string(deltaTime));
		//SDL_GL_SwapWindow(m_window);
	}
}

void Application::Update(float deltaTime)
{
	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}

//TODO: done to make the multi movement
void Application::AllMovements()
{

	if (INPUT->GetKey(SDLK_a))
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetRight());
	}
	else if (INPUT->GetKey(SDLK_d))
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetRight());
	}

	if (INPUT->GetKey(SDLK_w))
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetForward());
	}
	else if (INPUT->GetKey(SDLK_s))
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetForward());
	}


	
	

	//TODO: add the jump by adding vec3(0,1,0) and vec3(0,-1,0)
		


}

void Application::Render()
{
	/* Clear context */
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_mainCamera->Recalculate();
	for (auto& a : m_entities)
	{
		a->OnRender();
	}
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}

void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}
Application::~Application()
{
}
Application* Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}
void Application::Run()
{
	Init();
	Loop();
	Quit();
}
