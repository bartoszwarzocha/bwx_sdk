/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     BWX_SDK Library Example OpenGL App
// Author:      Bartosz Warzocha
// Created:     2025-03-09
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

//---------------------------------------------------------------------------
// Configuration
//---------------------------------------------------------------------------

/****************************************************************************
* USE_SHADER_PROGRAM_MANAGER
* If 0, use the normal way of managing shaders (create, bind etc.).
* If 1, use shader program manager class to manage shader programs.
*/
#define USE_SHADER_PROGRAM_MANAGER 1

/****************************************************************************
* USE_TEXTURE_MANAGER
* If 0, use the normal/manual way of managing textures (create, bind etc.).
* If 1, use texture manager class to manage textures.
*/
#define USE_TEXTURE_MANAGER 1

/****************************************************************************
* USE_RENDER_TIMER_INSTEAD_PAINT_EVENT
* If 0, use wxPaintEvent and wxIdleEvent to render the scene.
*     Faster but stop rendering when you use the window actions (menu etc.).
* If 1, use wxTimer to render the scene (slower but more stabilized).
*     Doesn't stop rendering when you use the window actions (menu etc.).
*/
#define USE_RENDER_TIMER_INSTEAD_PAINT_EVENT 0

//-----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/glcanvas.h>
#include <wx/splitter.h>
#include <wx/stopwatch.h>

#include <bwx_sdk/bwx_globals.h>
#include <bwx_sdk/bwx_core/bwx_string.h>
#include <bwx_sdk/bwx_gl/bwx_gl.h>

//---------------------------------------------------------------------------
// DATA
//---------------------------------------------------------------------------

// Vertices & texture coordinates
std::vector<float> verts = bwx_sdk::bwxGLUtils::GenerateSimpleCubeVertices(true);

// Shaders
std::string solid_vertex_src = R"(
	#version 330 core
	layout(location = 0) in vec3 pos;
	layout(location = 1) in vec2 texCoord;
	out vec2 TexCoord;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	void main()
	{
	    gl_Position = projection * view * model * vec4(pos, 1.0f);
	    TexCoord = vec2(texCoord.x, texCoord.y);
	}
)";

std::string solid_fragment_src = R"(
	#version 330 core
	out vec4 FragColor;
	in vec2 TexCoord;
	uniform sampler2D texture1;
	void main()
	{
	    FragColor = texture(texture1, TexCoord);
	}
)";

std::string mesh_vertex_src = R"(
	#version 330 core
	layout(location = 0) in vec3 pos;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	void main()
	{
	    gl_Position = projection * view * model * vec4(pos, 1.0f);
	}
)";

std::string mesh_fragment_src = R"(
	#version 330 core
	out vec4 FragColor;
	void main()
	{
	   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
)";

//---------------------------------------------------------------------------
// MyCanvas class
//---------------------------------------------------------------------------
class MyCanvas : public wxGLCanvas
{
public:
	MyCanvas(wxWindow* parent, const wxGLAttributes& attrs, wxWindowID id = wxID_ANY)
		: wxGLCanvas(parent, attrs, id) {
	}

	virtual ~MyCanvas() {}

	int x, y;

private:
	void OnSize(wxSizeEvent& event)
	{
		x = event.GetSize().GetWidth();
		y = event.GetSize().GetHeight();
		glViewport(0, 0, x, y);
	}

	DECLARE_EVENT_TABLE();
};

//---------------------------------------------------------------------------
// MyApp class
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

//---------------------------------------------------------------------------
// MyFrame class
//---------------------------------------------------------------------------
class MyFrame : public wxFrame
{
public:
	MyFrame(wxWindow* parent, wxWindowID id, const wxString& title,
		const wxPoint& pos, const wxSize& size, long style);
	~MyFrame();

	void InitScene();

private:
#if USE_RENDER_TIMER_INSTEAD_PAINT_EVENT
	void OnRenderTimer(wxTimerEvent& event);
	wxTimer timer;
#else
	void OnPaint(wxPaintEvent& event);
	void OnIdle(wxIdleEvent& event);
#endif

	void OnAboutProgram(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);

	void OnMesh(wxCommandEvent& event);
	void OnFPS(wxCommandEvent& event);

	void Render();

	wxStatusBar* sb;
	wxMenuBar* mb;
	wxMenu* mFile;
	wxMenu* mView;
	wxMenu* mHelp;

	wxPanel* panel;

	MyCanvas* canvas;
	wxGLContext* ctx;

	wxString m_GLversion;
	wxString m_GLvendor;
	wxString m_GLrenderer;

	bwx_sdk::bwxGLBuffer* m_cubeVbo;

	int m_fpsLimit;

#if not USE_SHADER_PROGRAM_MANAGER
	std::shared_ptr<bwx_sdk::bwxGLShaderProgram> solid_shader_program;
	std::shared_ptr<bwx_sdk::bwxGLShaderProgram> mesh_shader_program;
#endif
	bwx_sdk::bwxGLShaderProgram* active_shader_program;

#if not USE_TEXTURE_MANAGER
	std::shared_ptr<bwx_sdk::bwxGLTexture2D> texture;
#endif

	bwx_sdk::bwxGLTTF fontSmall;
	bwx_sdk::bwxGLTTF fontLarge;

	std::shared_ptr<bwx_sdk::bwxGLText> textSmall;
	std::shared_ptr<bwx_sdk::bwxGLText> textLarge;

	bwx_sdk::bwxGLFPSMonitor fps;

	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// App implementation
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

//---------------------------------------------------------------------------
// Event ids
//---------------------------------------------------------------------------
enum
{
	ID_CLOSE = wxID_HIGHEST + 1,
	ID_ABOUT_PROGRAM,
	//
	ID_POINT,
	ID_LINE,
	ID_FILL,
	//
	ID_FPS_UNLIMITED,
	ID_FPS_30,
	ID_FPS_60,
	ID_FPS_120
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_CLOSE, MyFrame::OnClose)
	EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
	//
#if USE_RENDER_TIMER_INSTEAD_PAINT_EVENT
	EVT_TIMER(wxID_ANY, MyFrame::OnRenderTimer)
#else
	EVT_PAINT(MyFrame::OnPaint)
	EVT_IDLE(MyFrame::OnIdle)
#endif
	//
	EVT_MENU_RANGE(ID_POINT, ID_FILL, MyFrame::OnMesh)
	EVT_MENU_RANGE(ID_FPS_UNLIMITED, ID_FPS_120, MyFrame::OnFPS)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyCanvas, wxGLCanvas)
	EVT_SIZE(MyCanvas::OnSize)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	MyFrame* frame = new MyFrame(NULL, wxID_ANY, "BWX_SDK & OpenGL", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
	frame->Show(true);
	return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style), m_fpsLimit(0)
{
	wxLog::SetActiveTarget(new wxLogGui());
	wxLog::SetLogLevel(0);

	mb = new wxMenuBar();

	mFile = new wxMenu();
	mView = new wxMenu();
	mHelp = new wxMenu();

	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

	mView->AppendRadioItem(ID_POINT, wxT("Point rendering\tF5"));
	mView->AppendRadioItem(ID_LINE, wxT("Line rendering\tF6"));
	mView->AppendRadioItem(ID_FILL, wxT("Triangle rendering\tF7"));
	mView->FindItem(ID_FILL)->Check();
	mView->AppendSeparator();
	mView->AppendRadioItem(ID_FPS_UNLIMITED, wxT("Unlimited FPS\tF8"));
	mView->AppendRadioItem(ID_FPS_30, wxT("30 FPS\tF9"));
	mView->AppendRadioItem(ID_FPS_60, wxT("60 FPS\tF10"));
	mView->AppendRadioItem(ID_FPS_120, wxT("120 FPS\tF11"));
	mView->FindItem(ID_FPS_UNLIMITED)->Check();

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About program\tF1"), wxT("About the program"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mView, wxT("&Render"));
	mb->Append(mHelp, wxT("&Help"));

	this->SetMenuBar(mb);

	sb = new wxStatusBar(this);
	sb->SetFieldsCount(2);
	this->SetStatusBar(sb);
	SetStatusText(title, 0);

	wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(ms);

	panel = new wxPanel(this);
	wxBoxSizer* ps = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(ps);

#if USE_RENDER_TIMER_INSTEAD_PAINT_EVENT
	timer.SetOwner(this);

	// Render timer
	// 1 ms -> Max 1000 FPS
	// 2 ms -> Max 500 FPS
	// 5 ms -> Max 200 FPS
	// 10 ms -> Max 100 FPS
	// 20 ms -> Max 50 FPS
	// 50 ms -> Max 20 FPS
	// 100 ms -> Max 10 FPS
	timer.Start(1);
#endif

	//-------------------------------------------
	// Canvas attributes
	wxGLAttributes vAttrs;
	vAttrs.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();

	if (!wxGLCanvas::IsDisplaySupported(vAttrs))
	{
		wxLogError(wxT("Unsupported OpenGL configuration!"));
		return;
	}

	// Create canvas
	canvas = new MyCanvas(panel, vAttrs, wxID_ANY);
	canvas->SetMinSize(wxSize(500, 500));

	// Context attributes
	wxGLContextAttrs cAttrs;
	cAttrs.PlatformDefaults().CoreProfile().OGLVersion(4, 5).EndList();

	// Create context
	ctx = new wxGLContext(canvas, (wxGLContext*)NULL, &cAttrs);

	if (!(*ctx).IsOK())
	{
		wxLogError(wxT("OpenGL context is invalid."));
		return;
	}

	// Set context
	canvas->SetCurrent(*ctx);

	ps->Add(canvas, 1, wxEXPAND | wxALL, 0);
	ms->Add(panel, 1, wxEXPAND | wxALL, 0);

	//*******************************************
	// OpenGL
	//*******************************************
	// 
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GL_NO_ERROR)
	{
		wxLogError(wxString(glewGetErrorString(err)));
	}

	// Load OpenGL version and graphics card information
	this->m_GLversion = wxString(glGetString(GL_VERSION));
	this->m_GLvendor = wxString(glGetString(GL_VENDOR));
	this->m_GLrenderer = wxString(glGetString(GL_RENDERER));

	// Renderowanie 3D
	glEnable(GL_DEPTH_TEST);

	// Atialiasing
	glEnable(GL_MULTISAMPLE);

	// Ukrywanie niewidocznych œcian
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	// Obs³uga przezroczystoœci
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);

	// VSync
#ifdef __WXMSW__
	((BOOL(WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(0);
#endif

	//-------------------------------------------
	// BUFFERS
	m_cubeVbo = new bwx_sdk::bwxGLBuffer(verts, 5, { 3, 2 });

	//-------------------------------------------
	// SHADERS
#if USE_SHADER_PROGRAM_MANAGER
	bwx_sdk::bwxGLShaderProgramManager::GetInstance().CreateShaderProgramFromStrings("solid", solid_vertex_src, solid_fragment_src);
	bwx_sdk::bwxGLShaderProgramManager::GetInstance().CreateShaderProgramFromStrings("mesh", mesh_vertex_src, mesh_fragment_src);
	active_shader_program = bwx_sdk::bwxGLShaderProgramManager::GetInstance().GetShaderProgramPtr("solid").get();

	bwx_sdk::bwxGLShaderProgramManager::GetInstance().Dump();
	//wxMessageBox(bwx_sdk::str::bwxSimpleJoin(bwx_sdk::bwxGLShaderProgramManager::GetInstance().GetShaderProgramNames()));
#else
	bwx_sdk::bwxGLShader mvs, mfs, svs, sfs;

	if (mvs.LoadShader(bwx_sdk::SHADER_VERTEX, mesh_vertex_src) &&
		mfs.LoadShader(bwx_sdk::SHADER_FRAGMENT, mesh_fragment_src)) {
		mesh_shader_program = std::make_shared<bwx_sdk::bwxGLShaderProgram>();
		mesh_shader_program->AttachShader(mvs);
		mesh_shader_program->AttachShader(mfs);
		if (!mesh_shader_program->Link())
			wxLogError(wxT("Mesh shader program link failed."));
	}
	else {
		wxLogError(wxT("Mesh shader program failed to load."));
	}

	if (svs.LoadShader(bwx_sdk::SHADER_VERTEX, solid_vertex_src) &&
		sfs.LoadShader(bwx_sdk::SHADER_FRAGMENT, solid_fragment_src)) {
		solid_shader_program = std::make_shared<bwx_sdk::bwxGLShaderProgram>();
		solid_shader_program->AttachShader(svs);
		solid_shader_program->AttachShader(sfs);
		if (!solid_shader_program->Link())
			wxLogError(wxT("Solid shader program link failed."));
	}
	else {
		wxLogError(wxT("Solid shader program failed to load."));
	}

	active_shader_program = solid_shader_program.get();
#endif

	//-------------------------------------------
	// TEXTURES
#if USE_TEXTURE_MANAGER
	bwx_sdk::bwxGLTextureManager::GetInstance().LoadTexture("./assets/images/texture.png");
#else
	texture = std::make_shared<bwx_sdk::bwxGLTexture2D>("./assets/images/texture.png");
#endif

	//-------------------------------------------
	// TEXT
	fontSmall.LoadFromFile("./assets/fonts/Ubuntu-R.ttf", 12);
	textSmall = std::make_shared<bwx_sdk::bwxGLText>(fontSmall);

	fontLarge.LoadFromFile("./assets/fonts/BW Typeface.ttf", 36);
	textLarge = std::make_shared<bwx_sdk::bwxGLText>(fontLarge);

	//-------------------------------------------
	// SCENE
	//InitScene();

	//-------------------------------------------
	Fit();
	CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
	wxDELETE(m_cubeVbo);

	wxDELETE(ctx);
	wxDELETE(canvas);
}

//---------------------------------------------------------------------------
// InitScene() function
//---------------------------------------------------------------------------
void MyFrame::InitScene() {
	using namespace bwx_sdk;

	// ---------- CAMERA ----------
	auto cameraNode = std::make_shared<bwxGLNode>();
	auto camTransform = cameraNode->AddComponent<bwxGLTransformComponent>();
	auto camComponent = cameraNode->AddComponent<bwxGLCameraComponent>(bwxGL_CAMERA_TYPE::CAMERA_TYPE_SPECTATOR);
	auto movement = cameraNode->AddComponent<bwxGLMovementComponent>();
	auto control = cameraNode->AddComponent<bwxGLControlComponent>();

	camTransform->SetPosition(0.0f, 0.0f, 5.0f);

	// ---------- LIGHT 1 (yellow) ----------
	auto light1 = std::make_shared<bwxGLNode>();
	auto light1Transform = light1->AddComponent<bwxGLTransformComponent>();
	auto light1Comp = light1->AddComponent<bwxGLLightComponent>(bwxGL_LIGHT_TYPE::LIGHT_POINT);

	light1Transform->SetPosition(3.0f, 2.0f, 2.0f);
	light1Comp->SetLightColor({ 1.0f, 1.0f, 0.8f });
	light1Comp->SetPower(1.0f);
	light1Comp->SetRange(10.0f);

	// ---------- LIGHT 2 (blue) ----------
	auto light2 = std::make_shared<bwxGLNode>();
	auto light2Transform = light2->AddComponent<bwxGLTransformComponent>();
	auto light2Comp = light2->AddComponent<bwxGLLightComponent>(bwxGL_LIGHT_TYPE::LIGHT_POINT);

	light2Transform->SetPosition(-3.0f, 2.0f, 1.0f);
	light2Comp->SetLightColor({ 0.7f, 0.8f, 1.0f });
	light2Comp->SetPower(1.0f);
	light2Comp->SetRange(10.0f);

	// ---------- CUBE ----------
	auto cubeNode = std::make_shared<bwxGLNode>();
	auto cubeTransform = cubeNode->AddComponent<bwxGLTransformComponent>();
	auto cubeRenderable = cubeNode->AddComponent<bwxGLRenderableComponent>();

	// Za³aduj zasób siatki / bufora kostki (jeœli masz menad¿er zasobów)
	// lub u¿yj tymczasowych danych VBO (przyk³ad pomijam dla skrótu)
	cubeTransform->SetScale(1.0f);

	// ---------- SYSTEMY ----------
	auto& lightSystem = bwxGLLightSystem::GetInstance();
	lightSystem.Register(light1);
	lightSystem.Register(light2);

	auto& renderSystem = bwxGLRenderSystem::GetInstance();
	//renderSystem.SetActiveCamera(cameraNode);
	//renderSystem.SetLightSystem(std::make_shared<bwxGLLightSystem>(lightSystem));
	//renderSystem.Register(cubeRenderable);
}


//---------------------------------------------------------------------------
// Function initiates program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays "About program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox("TEST", wxT("About program..."), wxOK | wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
// Function toggles view between normal and mesh view
//---------------------------------------------------------------------------
void MyFrame::OnMesh(wxCommandEvent& event)
{
	active_shader_program->Unbind();

#if not USE_SHADER_PROGRAM_MANAGER
	if (event.GetId() == ID_POINT)
	{
		glPointSize(5);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		active_shader_program = mesh_shader_program.get();
	}
	else if (event.GetId() == ID_LINE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		active_shader_program = mesh_shader_program.get();
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		active_shader_program = solid_shader_program.get();
	}
#else
	if (event.GetId() == ID_POINT)
	{
		glPointSize(5);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		active_shader_program = bwx_sdk::bwxGLShaderProgramManager::GetInstance().GetShaderProgramPtr("mesh").get();
	}
	else if (event.GetId() == ID_LINE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		active_shader_program = bwx_sdk::bwxGLShaderProgramManager::GetInstance().GetShaderProgramPtr("mesh").get();
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		active_shader_program = bwx_sdk::bwxGLShaderProgramManager::GetInstance().GetShaderProgramPtr("solid").get();
	}
#endif
}

//---------------------------------------------------------------------------
// Function handles FOS limit change
//---------------------------------------------------------------------------
void MyFrame::OnFPS(wxCommandEvent& event)
{
	if (event.GetId() == ID_FPS_UNLIMITED)
		m_fpsLimit = 0;
	else if (event.GetId() == ID_FPS_30)
		m_fpsLimit = 30;
	else if (event.GetId() == ID_FPS_60)
		m_fpsLimit = 60;
	else if (event.GetId() == ID_FPS_120)
		m_fpsLimit = 120;
}

//---------------------------------------------------------------------------
// Function handles window repaint event
//---------------------------------------------------------------------------
#if not USE_RENDER_TIMER_INSTEAD_PAINT_EVENT
void MyFrame::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	Render();
}
#endif

//---------------------------------------------------------------------------
// Function performs drawing during window idle time
//---------------------------------------------------------------------------
#if not USE_RENDER_TIMER_INSTEAD_PAINT_EVENT
void MyFrame::OnIdle(wxIdleEvent& event)
{
	Render();
	event.RequestMore(); // Ensure continuity...
}
#endif

//---------------------------------------------------------------------------
// Function performs drawing during render timer event
//---------------------------------------------------------------------------
#if USE_RENDER_TIMER_INSTEAD_PAINT_EVENT
void MyFrame::OnRenderTimer(wxTimerEvent& event)
{
	Render();
}
#endif

//---------------------------------------------------------------------------
// Drawing function
//---------------------------------------------------------------------------
void MyFrame::Render()
{
	if (!this->IsShown()) return;

	fps.StartFrame();

	// Default background color
	bwx_sdk::bwxGLUtils::SetDefaultClearColor();

	// Delta time saved to variable for different operations (for example movement)
	GLfloat delta = fps.GetDelta();

	// Activate shader program
	active_shader_program->Bind();

	// Set transformation matrices
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), fps.GetElapsedTime(), glm::vec3(0.5f, 1.0f, 0.0f));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)canvas->x / (float)canvas->y, 0.1f, 100.0f);

	// Pass matrices to shader
	active_shader_program->SetUniform("model", model);
	active_shader_program->SetUniform("view", view);
	active_shader_program->SetUniform("projection", projection);

	// Activate texture
#if USE_TEXTURE_MANAGER
	bwx_sdk::bwxGLTextureManager::GetInstance().BindTexture("./assets/images/texture.png");
#else
	texture->Bind();
#endif

	// Render object
	glBindVertexArray(m_cubeVbo->GetVAO());
	glDrawArrays(GL_TRIANGLES, 0, verts.size() / 5); // 5 floats per vertex (pos + texcoord)
	glBindVertexArray(0);

#if USE_TEXTURE_MANAGER
	//bwx_sdk::bwxGLTextureManager::GetInstance().UnbindTexture(0);
#endif

	//-------------------------------------------
	// RENDER TEXT
	glm::mat4 ortho = glm::ortho(0.0f, (float)canvas->x, 0.0f, (float)canvas->y);

	// Text with OpenGL coordinates... (from bottom-left corner)
	textSmall->Render(bwx_sdk::str::bwxStringToWstring(fps.GetFPSStr()), ortho, glm::vec2(10, 42), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	textSmall->Render(bwx_sdk::str::bwxStringToWstring(std::format("FPS Limit: {}", m_fpsLimit)), ortho, glm::vec2(10, 26), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	textSmall->Render(L"(c) 2025 by Bartosz Warzocha", ortho, glm::vec2(10, 10), 0.9f, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)); // Smaller scale looks better
	
	// Text with window coordinates... (from top-left corner)
	textLarge->Render(
		L"BWX_SDK OpenGL Example", 
		ortho, 
		bwx_sdk::bwxGLUtils::GetWindowCoordinates(canvas, glm::vec2(20, 20 + textLarge->GetFontHeight())), // Client window coordinates
		1.0f, // If scale is larger than 1.0f, text will be blurry
		glm::vec4(0.75f, 1.0f, 1.0f, 1.0f)
	); 

	//-------------------------------------------
	// Swap buffers
	canvas->SwapBuffers();

	// FPS limit
	fps.LimitFPS(m_fpsLimit);
}
