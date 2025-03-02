#ifndef _BWX_GLOBALS_H_
#define _BWX_GLOBALS_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <algorithm>
#include <chrono>
#include <deque>
#include <functional>
#include <map>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <vector>

// *** MEMORY LEAKS DIAGNOSE ***
// Options: _NORMAL_BLOCK/_CLIENT_BLOCK
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#define _new new (_CLIENT_BLOCK, __FILE__, __LINE__)
#define bwxMemStat(x)         \
    _CrtMemState _##x;        \
    _CrtMemCheckpoint(&_##x); \
    _CrtMemDumpStatistics(&_##x);
#else
#define _new new
#define bwxMemStat(x)
#endif
// *** EOF MEMORY LEAKS DIAGNOSE ***

/* CONSTANS                                                                      */

#if BWX_BUILD_AS_DLL
#define BWX_EXPORT __declspec(dllexport)
#define BWX_EXPORT_DATA(type) BWX_EXPORT type
#define BWX_IMPORT __declspec(dllimport)
#define BWX_IMPORT_DATA(type) BWX_IMPORT type
#define BWX_IMPORT_EVT(name, type) BWX_IMPORT const wxEventTypeTag<type> name
#else
#define BWX_EXPORT
#define BWX_EXPORT_DATA(type)
#define BWX_IMPORT
#define BWX_IMPORT_DATA(type)
#define BWX_IMPORT_EVT(name, type)
#endif

#define BWX_OK 0
#define BWX_FAIL -1

#define BWX_YES wxYES
#define BWX_NO wxNO

typedef enum { BWX_SEX_UNKNOWN = 0x00000000, BWX_SEX_MALE = 0x00000001, BWX_SEX_FEMALE = 0x00000002 } BWX_SEX;

typedef enum {
    BWX_NOMINATIVE = wxID_HIGHEST + 1,
    BWX_ACCUSATIVE,
    BWX_GENITIVE,
    BWX_DATING,
    BWX_INSTRUMENTAL,
    BWX_LOCATIVE,
    BWX_VOCATIVE
} BWX_CASES;

/* MACROS                                                                        */

// C++ 11 =========================================================================
#define _CUP unique_ptr
#define _CMU make_unique
#define _CSP shared_ptr
#define _CMS make_shared
#define _CFE for_each

// ================================================================================
#define _NBMP_ wxNullBitmap
#define _ES_ wxEmptyString

#define _DP_ wxDefaultPosition
#define _DS_ wxDefaultSize
#define _DT_ wxDefaultDateTime
#define _DV_ wxDefaultValidator
#define _DPS_ _DP_, _DS_
#define _EDPS_ _ES_, _DP_, _DS_

#define bwxSLEEP(x) wxMilliSleep(x)

#define bwxAPP(a) IMPLEMENT_APP(a)

// Classes ========================================================================
// n(T, x, f) - type, variable name, methods name
// n(Class, T, x, f) - like abowe (set class name)

#define _member(T, x, f)                              \
public:                                               \
    void Set##f(T x) { this->m_##x = x; }             \
    void Set##f##Ref(const T& x) { this->m_##x = x; } \
    void Set##f##Ptr(T* x) { this->m_##x = *x; }      \
    T Get##f() { return this->m_##x; }                \
    const T& Get##f##Ref() { return this->m_##x; }    \
    T* Get##f##Ptr() { return &(this->m_##x); }       \
                                                      \
private:                                              \
    T m_##x

#define _ptr_member(T, x, f)               \
public:                                    \
    void Set##f(T* x) { this->m_##x = x; } \
    T* Get##f() { return this->m_##x; }    \
                                           \
private:                                   \
    T* m_##x

#define _member_isOk                                           \
public:                                                        \
    void SetIsOK(bool isOk) { this->m_isOk = isOk; }           \
    void SetIsOKRef(const bool& isOk) { this->m_isOk = isOk; } \
    void SetIsOKPtr(bool* isOk) { this->m_isOk = *isOk; }      \
    bool IsOk() { return this->m_isOk; }                       \
    const bool& GetIsOKRef() { return this->m_isOk; }          \
    bool* GetIsOKPtr() { return &(this->m_isOk); }             \
                                                               \
private:                                                       \
    bool m_isOk

#define _member_id _member(int, id, Id)
#define _member_count _member(int, count, Count)
#define _member_min(T) _member(T, min, Min)
#define _member_max(T) _member(T, max, Max)

#define _inline(T, x, f)                                     \
public:                                                      \
    inline void Set##f(T x) { this->m_##x = x; }             \
    inline void Set##f##Ref(const T& x) { this->m_##x = x; } \
    inline void Set##f##Ptr(T* x) { this->m_##x = *x; }      \
    inline T Get##f() { return this->m_##x; }                \
    inline const T& Get##f##Ref() { return this->m_##x; }    \
    inline T* Get##f##Ptr() { return &(this->m_##x); }       \
                                                             \
private:                                                     \
    T m_##x;

#define _static(Class, T, x, f)                               \
public:                                                       \
    static void Set##f(T x) { Class::m_##x = x; }             \
    static void Set##f##Ref(const T& x) { Class::m_##x = x; } \
    static void Set##f##Ptr(T* x) { Class::m_##x = *x; }      \
    static T Get##f() { return Class::m_##x; }                \
    static const T& Get##f##Ref() { return Class::m_##x; }    \
    static T* Get##f##Ptr() { return &(Class::m_##x); }       \
                                                              \
private:                                                      \
    static T m_##x

#define _vector(T, x, f)                                                                               \
public:                                                                                                \
    void Add##f(T x) { this->m_##x.push_back(x); }                                                     \
    T Get##f(const int& index) { return this->m_##x[index]; }                                          \
    int Get##f##Count() { return this->m_##x.size(); }                                                 \
    void Clear##f##Vector() { this->m_##x.clear(); }                                                   \
    wxVector<T>* Get##f##VectorPtr() { return &(this->m_##x); }                                        \
    const wxVector<T>& Get##f##VectorRef() { return this->m_##x; }                                     \
    wxVector<T>::iterator Get##f##VectorBegin() { return this->m_##x.begin(); }                        \
    wxVector<T>::iterator Get##f##VectorEnd() { return this->m_##x.end(); }                            \
    wxVector<T>::iterator EraseFirst##f##() { return this->m_##x.erase(this->Get##f##VectorBegin()); } \
                                                                                                       \
private:                                                                                               \
    wxVector<T> m_##x

#define _vector_inline(T, x, f)                                                                               \
public:                                                                                                       \
    inline void Add##f(T x) { this->m_##x.push_back(x); }                                                     \
    inline T Get##f(const int& index) { return this->m_##x[index]; }                                          \
    inline int Get##f##Count() { return this->m_##x.size(); }                                                 \
    inline void Clear##f##Vector() { this->m_##x.clear(); }                                                   \
    inline wxVector<T>* Get##f##VectorPtr() { return &(this->m_##x); }                                        \
    inline const wxVector<T>& Get##f##VectorRef() { return this->m_##x; }                                     \
    inline wxVector<T>::iterator Get##f##VectorBegin() { return this->m_##x.begin(); }                        \
    inline wxVector<T>::iterator Get##f##VectorEnd() { return this->m_##x.end(); }                            \
    inline wxVector<T>::iterator EraseFirst##f##() { return this->m_##x.erase(this->Get##f##VectorBegin()); } \
                                                                                                              \
private:                                                                                                      \
    wxVector<T> m_##x

#define _vector_static(Class, T, x, f)                                                                          \
public:                                                                                                         \
    static void Add##f(T x) { Class::m_##x.push_back(x); }                                                      \
    static T Get##f(const int& index) { return Class::m_##x[index]; }                                           \
    static int Get##f##Count() { return Class::m_##x.size(); }                                                  \
    static void Clear##f##Vector() { Class::m_##x.clear(); }                                                    \
    static const wxVector<T>& Get##f##VectorRef() { return Class::m_##x; }                                      \
    static wxVector<T>::iterator Get##f##VectorBegin() { return Class::m_##x.begin(); }                         \
    static wxVector<T>::iterator Get##f##VectorEnd() { return Class::m_##x.end(); }                             \
    static wxVector<T>::iterator EraseFirst##f##() { return Class::m_##x.erase(Class::Get##f##VectorBegin()); } \
                                                                                                                \
private:                                                                                                        \
    static wxVector<T> m_##x

#define _std_vector(T, x, f)                                                                              \
public:                                                                                                   \
    void Add##f(T x) { this->m_##x.push_back(x); }                                                        \
    T Get##f(const int& index) { return this->m_##x[index]; }                                             \
    int Get##f##Count() { return this->m_##x.size(); }                                                    \
    void Clear##f##Vector() { this->m_##x.clear(); }                                                      \
    std::vector<T>* Get##f##VectorPtr() { return &(this->m_##x); }                                        \
    const std::vector<T>& Get##f##VectorRef() { return this->m_##x; }                                     \
    std::vector<T>::iterator Get##f##VectorBegin() { return this->m_##x.begin(); }                        \
    std::vector<T>::iterator Get##f##VectorEnd() { return this->m_##x.end(); }                            \
    std::vector<T>::iterator EraseFirst##f##() { return this->m_##x.erase(this->Get##f##VectorBegin()); } \
                                                                                                          \
private:                                                                                                  \
    std::vector<T> m_##x

#define _std_vector_inline(T, x, f)                                                                              \
public:                                                                                                          \
    inline void Add##f(T x) { this->m_##x.push_back(x); }                                                        \
    inline T Get##f(const int& index) { return this->m_##x[index]; }                                             \
    inline int Get##f##Count() { return this->m_##x.size(); }                                                    \
    inline void Clear##f##Vector() { this->m_##x.clear(); }                                                      \
    inline std::vector<T>* Get##f##VectorPtr() { return &(this->m_##x); }                                        \
    inline const std::vector<T>& Get##f##VectorRef() { return this->m_##x; }                                     \
    inline std::vector<T>::iterator Get##f##VectorBegin() { return this->m_##x.begin(); }                        \
    inline std::vector<T>::iterator Get##f##VectorEnd() { return this->m_##x.end(); }                            \
    inline std::vector<T>::iterator EraseFirst##f##() { return this->m_##x.erase(this->Get##f##VectorBegin()); } \
                                                                                                                 \
private:                                                                                                         \
    std::vector<T> m_##x

#define _std_vector_static(Class, T, x, f)                                                                         \
public:                                                                                                            \
    static void Add##f(T x) { Class::m_##x.push_back(x); }                                                         \
    static T Get##f(const int& index) { return Class::m_##x[index]; }                                              \
    static int Get##f##Count() { return Class::m_##x.size(); }                                                     \
    static void Clear##f##Vector() { Class::m_##x.clear(); }                                                       \
    static const std::vector<T>& Get##f##VectorRef() { return Class::m_##x; }                                      \
    static std::vector<T>::iterator Get##f##VectorBegin() { return Class::m_##x.begin(); }                         \
    static std::vector<T>::iterator Get##f##VectorEnd() { return Class::m_##x.end(); }                             \
    static std::vector<T>::iterator EraseFirst##f##() { return Class::m_##x.erase(Class::Get##f##VectorBegin()); } \
                                                                                                                   \
private:                                                                                                           \
    static std::vector<T> m_##x

#define _map(T1, T2, x, f)                                                        \
public:                                                                           \
    void Set##f(T1 key, T2 value) { this->m_##x[key] = value; }                   \
    T2 Get##f(T1 key) { return this->m_##x[key]; }                                \
    const T2& Get##f##Ref(T1 key) { return this->m_##x[key]; }                    \
    void Clear##f##Map() { this->m_##x.clear(); }                                 \
    int Get##f##Count() { return this->m_##x.size(); }                            \
    std::map<T1, T2>* Get##f##MapPtr() { return &(this->m_##x); }                 \
    const std::map<T1, T2>& Get##f##MapRef() { return this->m_##x; }              \
    std::map<T1, T2>::iterator Get##f##MapBegin() { return this->m_##x.begin(); } \
    std::map<T1, T2>::iterator Get##f##MapEnd() { return this->m_##x.end(); }     \
                                                                                  \
private:                                                                          \
    std::map<T1, T2> m_##x

#define _map_static(Class, T1, T2, x, f)                                                  \
public:                                                                                   \
    static void Set##f(T1 key, T2 value) { Class::m_##x[key] = value; }                   \
    static T2 Get##f(T1 key) { return Class::m_##x[key]; }                                \
    static const T2& Get##f##Ref(T1 key) { return Class::m_##x[key]; }                    \
    static void Clear##f##Map() { Class::m_##x.clear(); }                                 \
    static int Get##f##Count() { return Class::m_##x.size(); }                            \
    static const std::map<T1, T2>& Get##f##MapRef() { return Class::m_##x; }              \
    static std::map<T1, T2>::iterator Get##f##MapBegin() { return Class::m_##x.begin(); } \
    static std::map<T1, T2>::iterator Get##f##MapEnd() { return Class::m_##x.end(); }     \
                                                                                          \
private:                                                                                  \
    static std::map<T1, T2> m_##x

// wxString =======================================================================
#define _SREF const wxString&
#define _STDREF const std::string&

#define _CStr(s) s.ToStdString()
#define _CC(s) (const_cast<char*>((const char*)s.mb_str()))

#define _SF wxString::Format
#define _SFD(a) _SF("%d", a)
#define _SFF(a) _SF("%f", a)
#define _SFx(a) _SF("%x", a)
#define _SFX(a) _SF("%X", a)
#define _FromSI(a) std::atoi(a)
#define _FromSD(a) strtod((const char*)a.mb_str(), NULL)
#define _SF1(f, a) _SF(f, a)
#define _SF2(f, a, b) _SF(f, a, b)
#define _SF3(f, a, b, c) _SF(f, a, b, c)
#define _SF4(f, a, b, c, d) _SF(f, a, b, c, d)
#define _SF5(f, a, b, c, d, e) _SF(f, a, b, c, d, e)

#define _Str(n) wxString n = _ES_  // Declare string

// ================================================================================
#define _L(s) wxGetTranslation(s)

// wxDateTime =====================================================================
#define _DTREF const wxDateTime&

#define _NOW_ wxDateTime::Now()
#define _FDTC(d) d.FormatISOCombined(' ')
#define _FDTT(t) t.FormatISOTime()
#define _FDTD(d) d.FormatISODate()
#define _NOWC_ _FDTC(_NOW_)
#define _NOWT_ _FDTT(_NOW_)
#define _NOWD_ _FDTD(_NOW_)

// MessageBox =====================================================================
#define _MBX(s1, s2) wxMessageBox(s1, s2)
#define _MB(s) _MBX(s, wxEmptyString)
#define _MF(a) wxMessageBox(_SF("d%", a))
#define _MF1(f, s) _MB(_SF(f, s))
#define _MF2(f, s1, s2) _MB(_SF2(f, s1, s2))

#define _MBE(s) wxMessageBox(s, _("Error!"), wxOK | wxICON_ERROR)
#define _MBW(s) wxMessageBox(s, _("Warning"), wxOK | wxICON_EXCLAMATION)
#define _MBI(s) wxMessageBox(s, _("Information"), wxOK | wxICON_INFORMATION)
#define _MBQ(s) wxMessageBox(s, _("Question..."), wxYES_NO | wxICON_QUESTION)
#define _MBQ2(s) wxMessageBox(s, _("Question..."), wxOK | wxCANCEL | wxICON_QUESTION)

// wxLog ==========================================================================
#define _LM(s) wxLogMessage(s)
#define _LW(s) wxLogWarning(s)
#define _LE(s) wxLogError(s)

// Graphics =======================================================================
#define _PNGRC(i) wxBITMAP_PNG(i)
#define _PNG(f) wxBitmap(f, wxBITMAP_TYPE_PNG)
#define _JPG(f) wxBitmap(f, wxBITMAP_TYPE_JPEG)
#define _BMP(f) wxBitmap(f, wxBITMAP_TYPE_BMP)
#define _GetPNG(f) wxBitmap(bwxGetAppFile(f), wxBITMAP_TYPE_PNG)
#define _GetJPG(f) wxBitmap(bwxGetAppFile(f), wxBITMAP_TYPE_JPEG)
#define _GetBMP(f) wxBitmap(bwxGetAppFile(f), wxBITMAP_TYPE_BMP)

#define wxGray(c) wxColour(c, c, c)
#define wxGrayAlpha(c, a) wxColour(c, c, c, a)

#define _G(c) wxColour(c, c, c)
#define _C(r, g, b) wxColour(r, g, b)
#define _GA(c, a) wxColour(c, c, c, a)
#define _CA(r, g, b, a) wxColour(r, g, b, a)
#define _rgb _C
#define _RGB _C
#define _rgba _CA
#define _RGBA _CA
#define _grey _G
#define _GREY _G
#define _greya _GA
#define _GREYA _GA

// Events =========================================================================
#define _DET() DECLARE_EVENT_TABLE()
#define _BET(x, y) BEGIN_EVENT_TABLE(x, y)
#define _EET() END_EVENT_TABLE()

#define BindMenuEvt(id, fn) Bind(wxEVT_MENU, &fn, this, id)
#define UnbindMenuEvt(id, fn) Unbind(wxEVT_MENU, &fn, this, id)

#define BindButtonEvt(id, fn) Bind(wxEVT_BUTTON, &fn, this, id)
#define UnbindButtonEvt(id, fn) Unbind(wxEVT_BUTTON, &fn, this, id)

// Sizers =========================================================================
typedef int bwxMargin;
typedef int bwxAlign;

#define _AL wxALIGN_LEFT
#define _AR wxALIGN_RIGHT
#define _AT wxALIGN_TOP
#define _AB wxALIGN_BOTTOM
#define _AC wxALIGN_CENTER
#define _ACH wxALIGN_CENTER_HORIZONTAL
#define _ACV wxALIGN_CENTER_VERTICAL

// ================================================================================
#define _AF(var, flag) var |= flag
#define _DF(var, flag) var &= ~(flag)
#define _GF(var, flag) var& flag

// Algorithms =====================================================================
#include <algorithm>
#define _FI(a) for (int i = 0; i < a; i++)
#define _FJ(a) for (int j = 0; j < a; j++)
#define _FX(a) for (int x = 0; x < a; x++)
#define _FY(a) for (int y = 0; y < a; y++)
#define _FZ(a) for (int z = 0; z < a; z++)
#define _FIB(b, a) for (int i = b; i < a; i++)  // base
#define _FJB(b, a) for (int j = b; j < a; j++)
#define _FXB(b, a) for (int x = b; x < a; x++)
#define _FYB(b, a) for (int y = b; y < a; y++)
#define _FZB(b, a) for (int z = b; z < a; z++)
#define _FIBS(b, a, s) for (int i = b; i < a; i += s)  // base + step
#define _FJBS(b, a, s) for (int j = b; j < a; j += s)
#define _FXBS(b, a, s) for (int x = b; x < a; x += s)
#define _FYBS(b, a, s) for (int y = b; y < a; y += s)
#define _FZBS(b, a, s) for (int z = b; z < a; z += s)
#define _FID(a) for (int i = a; i > 0; i--)
#define _FJD(a) for (int j = a; j > 0; j--)
#define _FXD(a) for (int x = a; x > 0; x--)
#define _FYD(a) for (int y = a; y > 0; y--)
#define _FZD(a) for (int z = a; z > 0; z--)
#define _FIDB(a, b) for (int i = a; i > b; i--)  // base
#define _FJDB(a, b) for (int j = a; j > b; j--)
#define _FXDB(a, b) for (int x = a; x > b; x--)
#define _FYDB(a, b) for (int y = a; y > b; y--)
#define _FZDB(a, b) for (int z = a; z > b; z--)
#define _FIDBS(a, b, s) for (int i = a; i > b; i -= s)
#define _FJDBS(a, b, s) for (int j = a; j > b; j -= s)
#define _FXDBS(a, b, s) for (int x = a; x > b; x -= s)
#define _FYDBS(a, b, s) for (int y = a; y > b; y -= s)
#define _FZDBS(a, b, s) for (int z = a; z > b; z -= s)
#define _FOREACH(v, f) std::for_each(v.begin(), v.end(), f)
#define _ALL(v, f) std::all_of(v.begin(), v.end(), f)
#define _ANY(v, f) std::any_of(v.begin(), v.end(), f)
#define _NONE(v, f) std::none_of(v.begin(), v.end(), f)

// Controls ======================================================================
#define _P wxPanel

typedef wxSizer wxSi;
typedef wxBoxSizer wxBSi;

typedef wxPanel wxP;
typedef wxToolBar wxTB;
typedef wxMenuBar wxMB;
typedef wxMenu wxMN;
typedef wxStatusBar wxSB;
typedef wxStaticText wxSTXT;
typedef wxStaticBitmap wxSBMP;
typedef wxButton wxBTN;
typedef wxTextCtrl wxTE;

/* Types                                                                         */

typedef int bwxByteFlag;

#endif
