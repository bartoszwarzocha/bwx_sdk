/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_utils.h
// Purpose:     BWX_SDK Libarary; OpenGL Utils
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_UTILS_H_
#define _BWX_GL_UTILS_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <wx/glcanvas.h>

#include <chrono>
#include <string>

namespace bwx_sdk {
	
	/**
	 * @brief OpenGL utilities class.
	 */
	class bwxGLUtils {
	public:
		
		/**
		 * @brief Get window coordinates.
		 * @param canvas OpenGL canvas.
		 * @param pos Position.
		 * @return Window coordinates.
		 */
		static glm::vec2 GetWindowCoordinates(wxGLCanvas* canvas, glm::vec2 pos);
		
		/**
		 * @brief Get OpenGL version.
		 * @return OpenGL version.
		 */
		static std::string GetVersion();
		
		/**
		 * @brief Get OpenGL vendor.
		 * @return OpenGL vendor.
		 */
		static std::string GetVendor();
		
		/**
		 * @brief Get OpenGL renderer.
		 * @return OpenGL renderer.
		 */
		static std::string GetRenderer();
		
		/**
		 * @brief Get default OpenGL context attributes.
		 * @param major Major version.
		 * @param minor Minor version.
		 * @return Default OpenGL context attributes.
		 */
		static wxGLContextAttrs GetDefaultContextAttrs(int major = 3, int minor = 3);
		
		/**
		 * @brief Get default OpenGL canvas attributes.
		 * @param depth Depth.
		 * @return Default OpenGL canvas attributes.
		 */
		static wxGLAttributes GetDefaultCanvasAttrs(int depth = 24);
		
		/**
		 * @brief Get error string.
		 * @param err Error code.
		 * @return Error string.
		 */
		static std::string GetErrorString(int err);
		
		/**
		 * @brief Set default clear color.
		 * @param r Red.
		 * @param g Green.
		 * @param b Blue.
		 * @param a Alpha.
		 */
		static void SetDefaultClearColor(GLfloat r = 0.2f, GLfloat g = 0.3f, GLfloat b = 0.3f, GLfloat a = 1.0f);
	};

	/**
	 * @brief OpenGL FPS monitor class.
	 */
	class bwxGLFPSMonitor
	{
	public:

		/**
		 * @brief Constructor.
		 */
		bwxGLFPSMonitor();

		/**
		 * @brief Get FPS.
		 * @param refresh_ms Refresh time in milliseconds.
		 * @return FPS.
		 */
		GLfloat GetFPS(int refresh_ms = 500);
		
		/**
		 * @brief Get FPS string.
		 * @param refresh_ms Refresh time in milliseconds.
		 * @return FPS string.
		 */
		std::string GetFPSStr(int refresh_ms = 500);
		
		/**
		 * @brief Get delta time.
		 * @return Delta time.
		 */
		GLfloat GetDelta();
		
		/**
		 * @brief Get elapsed time.
		 * @return Elapsed time.
		 */
		GLfloat GetElapsedTime();

	private:
		std::chrono::steady_clock::time_point m_lastUpdate; ///< Last update time.
		std::chrono::steady_clock::time_point m_lastFrame; ///< Last frame time.

		GLint m_frames; ///< Frames.
		GLfloat m_fps; ///< FPS.
	};

} // namespace bwx_sdk

#endif // _BWX_GL_UTILS_H_