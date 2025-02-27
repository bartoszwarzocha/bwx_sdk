/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_core.cpp
// Purpose:     BWX_SDK Library; Basic core functions
// Author:      Bartosz Warzocha
// Created:     2015-09-03
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/stdpaths.h>

#include "bwx_core.h"

namespace bwx_sdk {

	//----------------------------------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------------------------------
	void bwxStdPathsInfo(void)
	{
		wxMessageBox(
			"GetAppDocumentsDir(): " + wxStandardPaths::Get().GetAppDocumentsDir() + "\n"
			+ "GetConfigDir(): " + wxStandardPaths::Get().GetConfigDir() + "\n"
			+ "GetDataDir(): " + wxStandardPaths::Get().GetDataDir() + "\n"
			+ "GetDocumentsDir(): " + wxStandardPaths::Get().GetDocumentsDir() + "\n"
			+ "GetExecutablePath(): " + wxStandardPaths::Get().GetExecutablePath() + "\n"
#if defined(__WXGTK__)
			+"GetInstallPrefix(): " + wxStandardPaths::Get().GetInstallPrefix() + "\n"
#endif
			+ "GetLocalDataDir(): " + wxStandardPaths::Get().GetLocalDataDir() + "\n"
			+ "GetPluginsDir(): " + wxStandardPaths::Get().GetPluginsDir() + "\n"
			+ "GetResourcesDir(): " + wxStandardPaths::Get().GetResourcesDir() + "\n"
			+ "GetTempDir(): " + wxStandardPaths::Get().GetTempDir() + "\n"
			+ "GetUserConfigDir(): " + wxStandardPaths::Get().GetUserConfigDir() + "\n"
			+ "GetUserDataDir(): " + wxStandardPaths::Get().GetUserDataDir() + "\n"
			+ "GetUserLocalDataDir(): " + wxStandardPaths::Get().GetUserLocalDataDir() + "\n",
			wxEmptyString,
			wxOK
		);
	}

	//----------------------------------------------------------------------------------------------
	// Bitwise flag operations
	//----------------------------------------------------------------------------------------------
	inline void bwxAddByteFlag(int& var, bwxByteFlag flag) noexcept {
		var |= flag;
	}

	inline void bwxRemoveByteFlag(int& var, bwxByteFlag flag) noexcept {
		var &= ~flag;
	}

	inline bool bwxIsByteFlagSet(const int& var, bwxByteFlag flag) noexcept {
		return (var & flag) != 0;
	}

	inline void bwxSetByteFlag(int& var, bwxByteFlag flag) noexcept {
		var = flag;
	}

	//----------------------------------------------------------------------------------------------
	// Multithreading (MT) error description functions
	//----------------------------------------------------------------------------------------------

	wxString bwxGetThreadErrorDescription(const wxThreadError& err, bool processLog)
	{
		wxString msg = wxEmptyString;

		switch (err)
		{
		case wxTHREAD_NO_ERROR: break;
		case wxTHREAD_NO_RESOURCE: msg = _("No resource left to create a new thread"); break;
		case wxTHREAD_RUNNING: msg = _("The thread is already running"); break;
		case wxTHREAD_NOT_RUNNING: msg = _("The thread isn't running"); break;
		case wxTHREAD_KILLED: msg = _("The thread is killed"); break;
		case wxTHREAD_MISC_ERROR: msg = _("Some other error"); break;
		default: break;
		}

		if (!msg.IsEmpty())
		{
			msg = wxString::Format(_("Multithread error: %s"), msg);

			if (processLog)
			{
				//wxLogSdkCoreError(msg);
			}
		}

		return msg;
	}

	wxString bwxGetSemaphoreErrorDescription(const wxSemaError& err, bool processLog)
	{
		wxString msg = wxEmptyString;

		switch (err)
		{
		case wxSEMA_NO_ERROR: break;
		case wxSEMA_INVALID: msg = _("The semaphore hasn't been initialized successfully"); break;
		case wxSEMA_BUSY: msg = _("The semaphore is busy"); break;
		case wxSEMA_TIMEOUT: msg = _("Timeout"); break;
		case wxSEMA_OVERFLOW: msg = _("The semaphore is full"); break;
		case wxSEMA_MISC_ERROR: msg = _("Some other error"); break;
		default: break;
		}

		if (!msg.IsEmpty())
		{
			msg = wxString::Format(_("Semaphore error: %s"), msg);

			if (processLog)
			{
				//wxLogSdkCoreError(msg);
			}
		}

		return msg;
	}

	wxString bwxGetMutexErrorDescritpion(const wxMutexError& err, bool processLog)
	{
		wxString msg = wxEmptyString;

		switch (err)
		{
		case wxMUTEX_NO_ERROR: break;
		case wxMUTEX_INVALID: msg = _("The mutex hasn't been initialized successfully"); break;
		case wxMUTEX_DEAD_LOCK: msg = _("The mutex is already locked by the calling thread"); break;
		case wxMUTEX_BUSY: msg = _("The mutex is already locked by another thread"); break;
		case wxMUTEX_UNLOCKED: msg = _("Attempt to unlock a mutex which is not locked"); break;
		case wxMUTEX_TIMEOUT: msg = _("Timeout"); break;
		case wxMUTEX_MISC_ERROR: msg = _("Some other error"); break;
		default: break;
		}

		if (!msg.IsEmpty())
		{
			msg = wxString::Format(_("Mutex error: %s"), msg);

			if (processLog)
			{
				//wxLogSdkCoreError(msg);
			}
		}

		return msg;
	}

} // namespace bwx_sdk
