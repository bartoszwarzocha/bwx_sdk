/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     BWX_SDK Library Example OpenGL App
// Author:      Bartosz Warzocha
// Created:     2025-03-09
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#define USE_SHADER_PROGRAM_MANAGER 1
#define USE_TEXTURE_MANAGER 1

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <GL/glew.h>
#include <wx/glcanvas.h>
#include <wx/splitter.h>
#include <wx/stopwatch.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include <bwx_sdk/bwx_globals.h>
#include <bwx_sdk/bwx_core/bwx_string.h>
#include <bwx_sdk/bwx_gl/bwx_gl.h>

// Vertices & texture coordinates
float verts[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

// Shaders
const char* solid_vertex_src = "#version 330 core\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec2 texCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(pos, 1.0f);\n"
"    TexCoord = vec2(texCoord.x, texCoord.y);\n"
"}\n\0";

const char* solid_fragment_src = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"void main()\n"
"{\n"
"    FragColor = texture(texture1, TexCoord);\n"
"}\n\0";

const char* mesh_vertex_src = "#version 330 core\n"
"layout(location = 0) in vec3 pos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(pos, 1.0f);\n"
"}\n\0";

const char* mesh_fragment_src = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";

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

	void OnAboutProgram(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);

	void OnMesh(wxCommandEvent& event);

	void OnPaint(wxPaintEvent& event);
	void OnIdle(wxIdleEvent& event);
	void OnSize(wxSizeEvent& event);

private:
	void Render();

	wxStatusBar* sb;
	wxMenuBar* mb;
	wxMenu* mFile;
	wxMenu* mView;
	wxMenu* mHelp;

	wxPanel* panel;

	//
	wxStopWatch timer;

	MyCanvas* canvas;
	wxGLContext* ctx;

	wxString GLversion;
	wxString GLvendor;
	wxString GLrenderer;

	unsigned int VBO, VAO;

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
	ID_POINT,
	ID_LINE,
	ID_FILL,
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_CLOSE, MyFrame::OnClose)
	EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
	//
	EVT_PAINT(MyFrame::OnPaint)
	EVT_IDLE(MyFrame::OnIdle)
	//
	EVT_MENU(ID_POINT, MyFrame::OnMesh)
	EVT_MENU(ID_LINE, MyFrame::OnMesh)
	EVT_MENU(ID_FILL, MyFrame::OnMesh)
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
	: wxFrame(parent, id, title, pos, size, style)
{
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

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About program\tF1"), wxT("About the program"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mView, wxT("&View"));
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

	timer.Start();

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
	this->GLversion = wxString(glGetString(GL_VERSION));
	this->GLvendor = wxString(glGetString(GL_VENDOR));
	this->GLrenderer = wxString(glGetString(GL_RENDERER));

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
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//-------------------------------------------
	// SHADERS
#if USE_SHADER_PROGRAM_MANAGER
	bwx_sdk::bwxGLShaderProgramManager::GetInstance().CreateShaderProgramFromStrings("solid", solid_vertex_src, solid_fragment_src, true);
	bwx_sdk::bwxGLShaderProgramManager::GetInstance().CreateShaderProgramFromStrings("mesh", mesh_vertex_src, mesh_fragment_src, true);
	active_shader_program = bwx_sdk::bwxGLShaderProgramManager::GetInstance().GetShaderProgramPtr("solid").get();
#else
	bwx_sdk::bwxGLShader mvs, mfs, svs, sfs;

	mvs.LoadShader(bwx_sdk::SHADER_VERTEX, mesh_vertex_src);
	mfs.LoadShader(bwx_sdk::SHADER_FRAGMENT, mesh_fragment_src);
	mesh_shader_program = std::make_shared<bwx_sdk::bwxGLShaderProgram>();
	mesh_shader_program->AttachShader(mvs);
	mesh_shader_program->AttachShader(mfs);
	mesh_shader_program->Link();

	svs.LoadShader(bwx_sdk::SHADER_VERTEX, solid_vertex_src);
	sfs.LoadShader(bwx_sdk::SHADER_FRAGMENT, solid_fragment_src);
	solid_shader_program = std::make_shared<bwx_sdk::bwxGLShaderProgram>();
	solid_shader_program->AttachShader(svs);
	solid_shader_program->AttachShader(sfs);
	solid_shader_program->Link();

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
	//font.SetCharsetPL();
	fontSmall.LoadFromFile("./assets/fonts/Ubuntu-R.ttf", 12);
	textSmall = std::make_shared<bwx_sdk::bwxGLText>(fontSmall);
	textSmall->SetDefaultShaderProgram();

	fontLarge.LoadFromFile("./assets/fonts/BW Typeface.ttf", 36);
	textLarge = std::make_shared<bwx_sdk::bwxGLText>(fontLarge);
	textLarge->SetDefaultShaderProgram();

	//-------------------------------------------
	Fit();
	CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	wxDELETE(ctx);
	wxDELETE(canvas);
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
// Function handles window repaint event
//---------------------------------------------------------------------------
void MyFrame::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	Render();
}

//---------------------------------------------------------------------------
// Function performs drawing during window idle time
//---------------------------------------------------------------------------
void MyFrame::OnIdle(wxIdleEvent& event)
{
	Render();

	// Ensure continuity...
	event.RequestMore();
}

//---------------------------------------------------------------------------
// Drawing function
//---------------------------------------------------------------------------
void MyFrame::Render()
{
	if (!this->IsShown()) return;

	// Default background color
	bwx_sdk::bwxGLUtils::SetDefaultClearColor();

	// Delta time saved to variable for different operations (for example movement)
	GLfloat delta = fps.GetDelta();

	// Activate shader program
	active_shader_program->Bind();

	// Set transformation matrices
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), (GLfloat)timer.TimeInMicro().ToDouble() / 1000000, glm::vec3(0.5f, 1.0f, 0.0f));
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
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

#if USE_TEXTURE_MANAGER
	//bwx_sdk::bwxGLTextureManager::GetInstance().UnbindTexture(0);
#endif

	//-------------------------------------------
	// RENDER TEXT
	glm::mat4 ortho = glm::ortho(0.0f, (float)canvas->x, 0.0f, (float)canvas->y);

	// Text with OpenGL coordinates... (from bottom-left corner)
	textSmall->Render(bwx_sdk::str::bwxStringToWstring(fps.GetFPSStr()), ortho, glm::vec2(10, 26), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
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
}
