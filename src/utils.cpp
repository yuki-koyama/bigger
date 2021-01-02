#include <bigger/utils.hpp>
#include <bimg/decode.h>
#include <bx/file.h>
#include <stdexcept>

#define DBG(_format, ...) printf("Error")

// The following functions in the "bgfx_utils" namespace were originally distributed in the bgfx repository (in the
// examples directory) under the BSD 2-clause license. Yuki Koyama modified them slightly and redistribute them under
// the MIT license here.
namespace bgfx_utils
{
    /*
     * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
     * License: https://github.com/bkaradzic/bimg#license-bsd-2-clause
     */

    bx::AllocatorI* getAllocator()
    {
        static bx::DefaultAllocator s_allocator;
        return &s_allocator;
    }

    bx::FileReaderI* getFileReader()
    {
        static bx::FileReader s_file_reader;
        return &s_file_reader;
    }

    void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
    {
        if (bx::open(_reader, _filePath))
        {
            uint32_t size = (uint32_t) bx::getSize(_reader);
            void*    data = BX_ALLOC(_allocator, size);
            bx::read(_reader, data, size);
            bx::close(_reader);
            if (NULL != _size)
            {
                *_size = size;
            }
            return data;
        }
        else
        {
            DBG("Failed to open: %s.", _filePath);
        }

        if (NULL != _size)
        {
            *_size = 0;
        }

        return NULL;
    }

    void* load(const char* _filePath, uint32_t* _size)
    {
        return load(getFileReader(), getAllocator(), _filePath, _size);
    }

    void unload(void* _ptr) { BX_FREE(getAllocator(), _ptr); }

    static void imageReleaseCb(void* _ptr, void* _userData)
    {
        BX_UNUSED(_ptr);
        bimg::ImageContainer* imageContainer = (bimg::ImageContainer*) _userData;
        bimg::imageFree(imageContainer);
    }

    bgfx::TextureHandle loadTexture(bx::FileReaderI*         _reader,
                                    const char*              _filePath,
                                    uint64_t                 _flags,
                                    uint8_t                  _skip,
                                    bgfx::TextureInfo*       _info,
                                    bimg::Orientation::Enum* _orientation)
    {
        BX_UNUSED(_skip);
        bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

        uint32_t size;
        void*    data = load(_reader, getAllocator(), _filePath, &size);
        if (NULL != data)
        {
            bimg::ImageContainer* imageContainer = bimg::imageParse(getAllocator(), data, size);

            if (NULL != imageContainer)
            {
                if (NULL != _orientation)
                {
                    *_orientation = imageContainer->m_orientation;
                }

                const bgfx::Memory* mem =
                    bgfx::makeRef(imageContainer->m_data, imageContainer->m_size, imageReleaseCb, imageContainer);
                unload(data);

                if (imageContainer->m_cubeMap)
                {
                    handle = bgfx::createTextureCube(uint16_t(imageContainer->m_width),
                                                     1 < imageContainer->m_numMips,
                                                     imageContainer->m_numLayers,
                                                     bgfx::TextureFormat::Enum(imageContainer->m_format),
                                                     _flags,
                                                     mem);
                }
                else if (1 < imageContainer->m_depth)
                {
                    handle = bgfx::createTexture3D(uint16_t(imageContainer->m_width),
                                                   uint16_t(imageContainer->m_height),
                                                   uint16_t(imageContainer->m_depth),
                                                   1 < imageContainer->m_numMips,
                                                   bgfx::TextureFormat::Enum(imageContainer->m_format),
                                                   _flags,
                                                   mem);
                }
                else if (bgfx::isTextureValid(0,
                                              false,
                                              imageContainer->m_numLayers,
                                              bgfx::TextureFormat::Enum(imageContainer->m_format),
                                              _flags))
                {
                    handle = bgfx::createTexture2D(uint16_t(imageContainer->m_width),
                                                   uint16_t(imageContainer->m_height),
                                                   1 < imageContainer->m_numMips,
                                                   imageContainer->m_numLayers,
                                                   bgfx::TextureFormat::Enum(imageContainer->m_format),
                                                   _flags,
                                                   mem);
                }

                if (bgfx::isValid(handle))
                {
                    bgfx::setName(handle, _filePath);
                }

                if (NULL != _info)
                {
                    bgfx::calcTextureSize(*_info,
                                          uint16_t(imageContainer->m_width),
                                          uint16_t(imageContainer->m_height),
                                          uint16_t(imageContainer->m_depth),
                                          imageContainer->m_cubeMap,
                                          1 < imageContainer->m_numMips,
                                          imageContainer->m_numLayers,
                                          bgfx::TextureFormat::Enum(imageContainer->m_format));
                }
            }
        }

        return handle;
    }

} // namespace bgfx_utils

std::string bigger::getShaderDirectoryPath(const bgfx::RendererType::Enum renderer_type)
{
    switch (renderer_type)
    {
    case bgfx::RendererType::Direct3D9:
        return "shaders/dx9";
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        return "shaders/dx11";
    case bgfx::RendererType::Metal:
        return "shaders/metal";
    case bgfx::RendererType::OpenGL:
        return "shaders/glsl";
    case bgfx::RendererType::OpenGLES:
        return "shaders/essl";
    case bgfx::RendererType::Nvn:
    case bgfx::RendererType::Gnm:
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Vulkan:
    case bgfx::RendererType::Count:
        break;
    }
    throw std::runtime_error("Renderer type not supported.");
}

bgfx::TextureHandle bigger::loadTexture(const char* file_path)
{
    return bgfx_utils::loadTexture(
        bgfx_utils::getFileReader(), file_path, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, 0, nullptr, nullptr);
}
