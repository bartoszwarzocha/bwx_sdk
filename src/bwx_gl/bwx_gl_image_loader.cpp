/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_image_loader.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Image Loader
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_image_loader.h"
#include <wx/image.h>
#include <iostream>

#include "bwx_sdk/bwx_globals.h"
#include "bwx_sdk/bwx_core/bwx_math.h"

namespace bwx_sdk {

    bwxGLImgLoader::bwxGLImgLoader() : wxImage(), m_width(0), m_height(0) {}

    bwxGLImgLoader::~bwxGLImgLoader() {
        m_data.clear();
    }

    bool bwxGLImgLoader::Load(const wxString& file, bwxGL_IMG_ROTATE_MODE rotate, bool forcePowerOf2) {
        if (!LoadFile(file)) return false;

        m_width = GetWidth();
        m_height = GetHeight();

        // Rotate before transformations
        RotateImage(rotate);

        // Sprawdzenie, czy OpenGL wymaga tekstur o potêdze 2
        GLint textureCompression;
        glGetIntegerv(GL_TEXTURE_COMPRESSION_HINT, &textureCompression);
        bool openglRequiresPowerOf2 = (textureCompression == GL_NICEST);

        // Skalowanie do wielokrotnoœci 2, jeœli wymagane
        if (forcePowerOf2 || openglRequiresPowerOf2) {
            bool rescale = false;
            int newWidth = m_width;
            int newHeight = m_height;

            if (!math::bwxIsPower2(m_width)) {
                newWidth = math::bwxNextPower2(m_width);
                rescale = true;
            }

            if (!math::bwxIsPower2(m_height)) {
                newHeight = math::bwxNextPower2(m_height);
                rescale = true;
            }

            if (rescale) {
                Rescale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);
                m_width = newWidth;
                m_height = newHeight;
            }
        }

        // Bytes Per Pixel
        int bpp = HasAlpha() ? 4 : 3;
        m_data.resize(m_width * m_height * bpp);

        GLubyte* bitmap = GetData();
        const GLubyte* alpha = HasAlpha() ? GetAlpha() : nullptr;

        // Copy data to OpenGL format
        int mod = m_height - 1;
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                int srcIdx = (x + (mod - y) * m_width) * 3;
                int dstIdx = (x + y * m_width) * bpp;

                m_data[dstIdx + 0] = bitmap[srcIdx + 0]; // Red
                m_data[dstIdx + 1] = bitmap[srcIdx + 1]; // Green
                m_data[dstIdx + 2] = bitmap[srcIdx + 2]; // Blue

                if (bpp == 4) {
                    m_data[dstIdx + 3] = alpha ? alpha[x + (mod - y) * m_width] : 255;
                }
            }
        }
        return true;
    }

    void bwxGLImgLoader::RotateImage(bwxGL_IMG_ROTATE_MODE rotate) {
        if (rotate == bwxGL_IMG_ROTATE_MODE::IMG_ROTATE0) return;

        wxImage rotated;
        switch (rotate) {
        case bwxGL_IMG_ROTATE_MODE::IMG_ROTATE90:
            rotated = Rotate90(false);
            break;
        case bwxGL_IMG_ROTATE_MODE::IMG_ROTATE180:
            rotated = Rotate180();
            break;
        case bwxGL_IMG_ROTATE_MODE::IMG_ROTATE270:
            rotated = Rotate90(true);
            break;
        default:
            return;
        }

        wxImage::operator=(rotated);

        m_width = GetWidth();
        m_height = GetHeight();
    }

    const std::vector<GLubyte>& bwxGLImgLoader::Data() const {
        return m_data;
    }

    int bwxGLImgLoader::GetBytesPerPixel() const {
        return HasAlpha() ? 4 : 3;
    }

} // namespace bwx_sdk
