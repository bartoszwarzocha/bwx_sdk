/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_config_utils.cpp
// Purpose:     BWX_SDK Library; Basic config wrapper
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

#include <wx/confbase.h>
#include <wx/fileconf.h>

#include "bwx_sdk/bwx_globals.h"
#include "bwx_config_utils.h"

namespace bwx_sdk {

	bwxPropertyMap<wxString, bwxConfigEntry> bwxConfigUtils::m_configEntries;

	void bwxConfigUtils::LoadConfig()
	{
		//bwxConfigUtils::ClearConfigEntryMap();

		bwxSetFileConf();

		wxLogMessage("Load app cofiguration to memory...");

		wxString val_str = _ES_;
		int val_int;
		float val_float;

		for (auto it = bwxConfigUtils::m_configEntries.begin(); it != bwxConfigUtils::m_configEntries.end(); it++)
		{
			switch (it->second.GetType())
			{
			case CONFIG_ENTRY_TYPE_STRING:
				config.Read(*(it->second.GetPathPtr()), &val_str, it->second.GetDefaultValuePtr()->As<wxString>());
				it->second.SetValue(val_str);
				break;

			case CONFIG_ENTRY_TYPE_INT:
				config.Read(*(it->second.GetPathPtr()), &val_int, it->second.GetDefaultValuePtr()->As<int>());
				it->second.SetValue(val_int);
				break;

			case CONFIG_ENTRY_TYPE_FLOAT:
				config.Read(*(it->second.GetPathPtr()), &val_float, it->second.GetDefaultValuePtr()->As<float>());
				it->second.SetValue(val_float);
				break;

			case CONFIG_ENTRY_TYPE_DOUBLE:
				it->second.SetValue(config.ReadDouble(*(it->second.GetPathPtr()), it->second.GetDefaultValuePtr()->As<double>()));
				break;

			case CONFIG_ENTRY_TYPE_BOOLEAN:
				it->second.SetValue(config.ReadBool(*(it->second.GetPathPtr()), it->second.GetDefaultValuePtr()->As<bool>()));
				break;
			}
		}

		bwxConfigUtils::ShowInLog();
	}

	void bwxConfigUtils::SaveConfig()
	{
		bwxSetFileConf();

		wxLogMessage("Save app cofiguration to file...");

		for (auto it = bwxConfigUtils::m_configEntries.begin(); it != bwxConfigUtils::m_configEntries.end(); it++)
		{
			switch (it->second.GetType())
			{
			case CONFIG_ENTRY_TYPE_STRING:
				wxLogDebug("Save config entry (string): %s: %s", it->second.GetPathPtr()->c_str(), it->second.GetValuePtr()->As<wxString>().c_str());
				config.Write(*(it->second.GetPathPtr()), it->second.GetValuePtr()->As<wxString>());
				break;

			case CONFIG_ENTRY_TYPE_INT:
				wxLogDebug("Save config entry (int): %s: %d", it->second.GetPathPtr()->c_str(), it->second.GetValuePtr()->As<int>());
				config.Write(*(it->second.GetPathPtr()), it->second.GetValuePtr()->As<int>());
				break;

			case CONFIG_ENTRY_TYPE_FLOAT:
				wxLogDebug("Save config entry (float): %s: %f", it->second.GetPathPtr()->c_str(), it->second.GetValuePtr()->As<float>());
				config.Write(*(it->second.GetPathPtr()), it->second.GetValuePtr()->As<float>());
				break;

			case CONFIG_ENTRY_TYPE_DOUBLE:
				wxLogDebug("Save config entry (double): %s: %lf", it->second.GetPathPtr()->c_str(), it->second.GetValuePtr()->As<double>());
				config.Write(*(it->second.GetPathPtr()), it->second.GetValuePtr()->As<double>());
				break;

			case CONFIG_ENTRY_TYPE_BOOLEAN:
				wxLogDebug("Save config entry (bool): %s: %s", it->second.GetPathPtr()->c_str(), (it->second.GetValuePtr()->As<bool>()) ? "true" : "false");
				config.Write(*(it->second.GetPathPtr()), it->second.GetValuePtr()->As<bool>());
				break;
			}
		}
	}

	void bwxConfigUtils::Set(const wxString& key, wxAny val)
	{
		wxLogInfo("Value: %s", val.As<wxString>().c_str());
		bwxConfigUtils::m_configEntries[key].SetValue(val);
		wxLogDebug("Saved value: %s", bwxConfigUtils::m_configEntries[key].GetValue().As<wxString>());
	}

	void bwxConfigUtils::Get(const wxString& key, wxString* val)
	{
		wxLogDebug("Get config entry: %s", key.c_str());
		*val = bwxConfigUtils::m_configEntries[key].GetValue().As<wxString>();
	}

	void bwxConfigUtils::Get(const wxString& key, int* val)
	{
		wxLogDebug("Get config entry: %s", key.c_str());
		*val = bwxConfigUtils::m_configEntries[key].GetValue().As<int>();
	}

	void bwxConfigUtils::Get(const wxString& key, float* val)
	{
		wxLogDebug("Get config entry: %s", key.c_str());
		*val = bwxConfigUtils::m_configEntries[key].GetValue().As<float>();
	}

	void bwxConfigUtils::Get(const wxString& key, bool* val)
	{
		wxLogDebug("Get config entry: %s", key.c_str());
		*val = bwxConfigUtils::m_configEntries[key].GetValue().As<bool>();
	}

	bool bwxConfigUtils::Get(const wxString& key, wxColour* val)
	{
		bool ret = false;
		wxLogDebug("Get config entry: %s", key.c_str());
		ret = wxFromString(bwxConfigUtils::m_configEntries[key].GetValue().As<wxString>(), val);
		if (!ret) *val = wxColour(128, 128, 128);
		return ret;
	}

	void bwxConfigUtils::ShowInLog()
	{
		wxLogMessage("Current config entries:");

		for (auto it = bwxConfigUtils::m_configEntries.begin(); it != bwxConfigUtils::m_configEntries.end(); it++)
		{
			switch (it->second.GetType())
			{
			case CONFIG_ENTRY_TYPE_STRING:
				wxLogDebug("Config entry (string): %s: %s", it->second.GetPathPtr()->c_str(), it->second.GetValuePtr()->As<wxString>().c_str());
				break;

			case CONFIG_ENTRY_TYPE_INT:
				wxLogDebug("Config entry (int): %s: %d", it->second.GetPathPtr()->c_str(), it->second.GetValuePtr()->As<int>());
				break;

			case CONFIG_ENTRY_TYPE_FLOAT:
				wxLogDebug("Config entry (float): %s: %f", it->second.GetPathPtr()->c_str(), it->second.GetValuePtr()->As<float>());
				break;

			case CONFIG_ENTRY_TYPE_DOUBLE:
				wxLogDebug("Config entry (double): %s: %lf", it->second.GetPathPtr()->c_str(), it->second.GetValuePtr()->As<double>());
				break;

			case CONFIG_ENTRY_TYPE_BOOLEAN:
				wxLogDebug("Config entry (bool): %s: %s", it->second.GetPathPtr()->c_str(), (it->second.GetValuePtr()->As<bool>()) ? "true" : "false");
				break;
			}
		}
	}

	void bwxConfigUtils::ClearEntries()
	{
		bwxConfigUtils::m_configEntries.clear();
	}

	void bwxConfigUtils::AddStringEntry(const wxString& key, const wxString& defaultValue)
	{
		bwxConfigUtils::m_configEntries[key] = bwxConfigEntry(CONFIG_ENTRY_TYPE_STRING, key, defaultValue);
	}

	void bwxConfigUtils::AddIntEntry(const wxString& key, const int& defaultValue)
	{
		bwxConfigUtils::m_configEntries[key] = bwxConfigEntry(CONFIG_ENTRY_TYPE_INT, key, defaultValue);
	}

	void bwxConfigUtils::AddFloatEntry(const wxString& key, const float& defaultValue)
	{
		bwxConfigUtils::m_configEntries[key] = bwxConfigEntry(CONFIG_ENTRY_TYPE_FLOAT, key, defaultValue);
	}

	void bwxConfigUtils::AddDoubleEntry(const wxString& key, const double& defaultValue)
	{
		bwxConfigUtils::m_configEntries[key] = bwxConfigEntry(CONFIG_ENTRY_TYPE_DOUBLE, key, defaultValue);
	}

	void bwxConfigUtils::AddBooleanEntry(const wxString& key, const bool& defaultValue)
	{
		bwxConfigUtils::m_configEntries[key] = bwxConfigEntry(CONFIG_ENTRY_TYPE_BOOLEAN, key, defaultValue);
	}

}
