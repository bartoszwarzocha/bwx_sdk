/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_image_loader.h
// Purpose:     BWX_SDK Libarary; OpenGL Image Loader
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_IMAGE_LOADER_H_
#define _BWX_GL_IMAGE_LOADER_H_

#include <wx/image.h>
#include <vector>

#if defined(__APPLE__)
    #error OpenGL functionality is not available for macOS.
#else
    #include <GL/glew.h>
#endif

namespace bwx_sdk {

    enum bwxGL_IMG_ROTATE_MODE {
        IMG_ROTATE0,
        IMG_ROTATE90,
        IMG_ROTATE180,
        IMG_ROTATE270
    };

    class bwxGLImgLoader : public wxImage {
    public:
        bwxGLImgLoader();
        ~bwxGLImgLoader();

        bool Load(const wxString& file, bwxGL_IMG_ROTATE_MODE rotate = bwxGL_IMG_ROTATE_MODE::IMG_ROTATE0, bool forcePowerOf2 = false);

        const std::vector<GLubyte>& Data() const;
        int GetBytesPerPixel() const;

        inline int Width() const { return m_width; }
        inline int Height() const { return m_height; }

    private:
        void RotateImage(bwxGL_IMG_ROTATE_MODE rotate);

        std::vector<GLubyte> m_data;
        int m_width;
        int m_height;
    };

}

#endif
