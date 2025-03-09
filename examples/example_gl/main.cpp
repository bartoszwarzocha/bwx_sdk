/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     BWX_SDK Library Example OpenGL App
// Author:      Bartosz Warzocha
// Created:     2025-03-09
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <GL/glew.h>
#include <wx/glcanvas.h>
#include <wx/splitter.h>
#include <wx/stopwatch.h>

#include <bwx_sdk/bwx_gl/bwx_gl_shader.h>
#include <bwx_sdk/bwx_gl/bwx_gl_texture.h>
#include <bwx_sdk/bwx_gl/bwx_gl_texture_manager.h>
#include <bwx_sdk/bwx_gl/bwx_gl_material.h>
#include <bwx_sdk/bwx_gl/bwx_gl_mesh.h>
#include <bwx_sdk/bwx_gl/bwx_gl_buffer.h>

// Wsp�rz�dne wierzcho�k�w i tekstur
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

// Shadery
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

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

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

	bwx_sdk::bwxGLVertexBuffer* vbo;
	bwx_sdk::bwxGLIndexBuffer* ibo;

	unsigned int VBO, VAO;
	bwx_sdk::bwxGLShaderProgram* solid_shader_program;
	bwx_sdk::bwxGLShaderProgram* mesh_shader_program;
	bwx_sdk::bwxGLShaderProgram* active_shader_program;

	bwx_sdk::bwxGLTexture2D* texture;

	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// Implementacja aplikacji
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

//---------------------------------------------------------------------------
// Identyfikatory
//---------------------------------------------------------------------------
enum
{
	ID_CLOSE = wxID_HIGHEST + 1,
	ID_ABOUT_PROGRAM,
	ID_MESH,
};

//---------------------------------------------------------------------------
// Tablica zdarze�
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
//
EVT_PAINT(MyFrame::OnPaint)
EVT_IDLE(MyFrame::OnIdle)
//
EVT_MENU(ID_MESH, MyFrame::OnMesh)
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

	MyFrame* frame = new MyFrame(NULL, wxID_ANY, "TEST", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
	frame->Show(true);
	return true;
}

//---------------------------------------------------------------------------
// Konstruktor MyFrame
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	mb = new wxMenuBar();

	mFile = new wxMenu();
	mView = new wxMenu();
	mHelp = new wxMenu();

	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Zamknij program\tCtrl-X"), wxT("Zamyka program"));

	mView->AppendCheckItem(ID_MESH, wxT("Widok siatki\tF5"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&O programie\tF1"), wxT("O programie"));

	mb->Append(mFile, wxT("&Plik"));
	mb->Append(mView, wxT("&Widok"));
	mb->Append(mHelp, wxT("P&omoc"));

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

	///////////////////////////////////////

	// Atrybuty p��tna
	wxGLAttributes vAttrs;
	vAttrs.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();

	if (!wxGLCanvas::IsDisplaySupported(vAttrs))
	{
		wxLogError(wxT("Nieobs�ugiwana konfiguracja OpenGL!"));
		return;
	}

	// Tworzymy p��tno
	canvas = new MyCanvas(panel, vAttrs, wxID_ANY);
	canvas->SetMinSize(wxSize(500, 500));

	// Atrybuty kontekstu
	wxGLContextAttrs cAttrs;
	cAttrs.PlatformDefaults().CoreProfile().OGLVersion(4, 5).EndList();

	// Tworzymy  kontekst
	ctx = new wxGLContext(canvas, (wxGLContext*)NULL, &cAttrs);

	if (!(*ctx).IsOK())
	{
		wxLogError(wxT("Kontekst OpenGL jest nieprawid�owy."));
		return;
	}

	// Ustawiamy kontekst
	canvas->SetCurrent(*ctx);

	ps->Add(canvas, 1, wxEXPAND | wxALL, 0);
	ms->Add(panel, 1, wxEXPAND | wxALL, 0);

	//*******************************************
	// OpenGL
	//*******************************************
	// Inicjujemy GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GL_NO_ERROR)
	{
		wxLogError(wxString(glewGetErrorString(err)));
	}

	// Wczytujemy informacje o wersji OpenGL oraz karcie graficznej
	this->GLversion = wxString(glGetString(GL_VERSION));
	this->GLvendor = wxString(glGetString(GL_VENDOR));
	this->GLrenderer = wxString(glGetString(GL_RENDERER));

	// W��czamy obs�ug� 3D
	glEnable(GL_DEPTH_TEST);

	// Inicjujemy bufory wsp�rz�dnych
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Tworzymy shadery
	bwx_sdk::bwxGLShader mvs, mfs, svs, sfs;
	mvs.LoadShader(bwx_sdk::SHADER_VERTEX, mesh_vertex_src);
	mfs.LoadShader(bwx_sdk::SHADER_FRAGMENT, mesh_fragment_src);
	svs.LoadShader(bwx_sdk::SHADER_VERTEX, solid_vertex_src);
	sfs.LoadShader(bwx_sdk::SHADER_FRAGMENT, solid_fragment_src);

	mesh_shader_program = new bwx_sdk::bwxGLShaderProgram();
	solid_shader_program = new bwx_sdk::bwxGLShaderProgram();

	mesh_shader_program->AttachShader(mvs);
	mesh_shader_program->AttachShader(mfs);
	mesh_shader_program->Link();

	solid_shader_program->AttachShader(svs);
	solid_shader_program->AttachShader(sfs);
	solid_shader_program->Link();

	active_shader_program = solid_shader_program;

	// Wczytujemy tekstur�
	texture = new bwx_sdk::bwxGLTexture2D("texture.png", GL_REPEAT, GL_LINEAR, true, false);

	//
	Fit();
	CenterOnScreen();
}

//---------------------------------------------------------------------------
// Destruktor MyFrame
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	wxDELETE(solid_shader_program);
	wxDELETE(mesh_shader_program);
	wxDELETE(texture);

	wxDELETE(ctx);
	wxDELETE(canvas);
}

//---------------------------------------------------------------------------
// Funkcja inicjuje zamkni�cie programu
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	this->Close(true);
}

//---------------------------------------------------------------------------
// Funkcja wy�wietla okno "O programie..."
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox("TEST", wxT("O programie..."), wxOK | wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
// Funkcja prze��cza widok pomi�dzy zwyk�ym i widokiem siatki
//---------------------------------------------------------------------------
void MyFrame::OnMesh(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		active_shader_program = mesh_shader_program;
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		active_shader_program = solid_shader_program;
	}
}

//---------------------------------------------------------------------------
// Funkcja obs�uguje zdarzenie przerysowania okna
//---------------------------------------------------------------------------
void MyFrame::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	Render();
}

//---------------------------------------------------------------------------
// Funkcja wykonuje rysowanie podczas bezczynno�ci okna
//---------------------------------------------------------------------------
void MyFrame::OnIdle(wxIdleEvent& event)
{
	Render();

	// Zapewniamy ci�g�o��...
	event.RequestMore();
}

//---------------------------------------------------------------------------
// Funkcja rysuj�ca
//---------------------------------------------------------------------------
void MyFrame::Render()
{
	if (!this->IsShown()) return;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Aktywacja programu shaderowego
	active_shader_program->Use();

	// Ustawienie macierzy transformacji
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), (GLfloat)timer.TimeInMicro().ToDouble() / 1000000, glm::vec3(0.5f, 1.0f, 0.0f));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)canvas->x / (float)canvas->y, 0.1f, 100.0f);

	// Przekazanie macierzy do shadera
	active_shader_program->SetUniform("model", model);
	active_shader_program->SetUniform("view", view);
	active_shader_program->SetUniform("projection", projection);

	// Aktywacja tekstury (zak�adaj�c, �e `texture` to wska�nik do `bwxGLTexture`)
	texture->Bind(0);

	// Renderowanie obiektu
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	canvas->SwapBuffers();
}
