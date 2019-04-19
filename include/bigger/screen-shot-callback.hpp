#ifndef screen_shot_callback_hpp
#define screen_shot_callback_hpp

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bx/file.h>

namespace bigger
{
    struct ScreenShotCallback : public bgfx::CallbackI
    {
        enum class ImageType { Png, Tga };

        virtual ~ScreenShotCallback() {}
        virtual void fatal(bgfx::Fatal::Enum, const char*) override {}
        virtual void traceVargs(const char*, uint16_t, const char*, va_list) override {}
        virtual void profilerBegin(const char*, uint32_t, const char*, uint16_t) override {}
        virtual void profilerBeginLiteral(const char*, uint32_t, const char*, uint16_t) override {}
        virtual void profilerEnd() override {}
        virtual uint32_t cacheReadSize(uint64_t) override { return 0; }
        virtual bool cacheRead(uint64_t, void*, uint32_t) override { return false; }
        virtual void cacheWrite(uint64_t, const void*, uint32_t) override {}
        virtual void captureBegin(uint32_t, uint32_t, uint32_t, bgfx::TextureFormat::Enum, bool) override {}
        virtual void captureEnd() override {}
        virtual void captureFrame(const void*, uint32_t) override {}

        virtual void screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data, uint32_t _size, bool _yflip) override
        {
            const std::string full_path = std::string(_filePath) + ((image_type == ImageType::Png) ? ".png" : ".tga");

            bx::FileWriter writer;
            if (bx::open(&writer, full_path.c_str()))
            {
                if (image_type == ImageType::Png)
                {
                    bimg::imageWritePng(&writer, _width, _height, _pitch, _data, bimg::TextureFormat::BGRA8, _yflip, nullptr);
                }
                else
                {
                    bimg::imageWriteTga(&writer, _width, _height, _pitch, _data, false, _yflip);
                }

                bx::close(&writer);
            }
            else
            {
                throw std::runtime_error("");
            }
        }

        ImageType image_type = ImageType::Tga;
    };
}

#endif /* screen_shot_callback_hpp */
