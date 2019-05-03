#include <bigger/app.hpp>

class MinimalApp final : public bigger::App
{
public:

    MinimalApp() {}

    void initialize(int argc, char** argv) override;
    void updateApp() override;
    void releaseSharedResources() override;
};

void MinimalApp::initialize(int argc, char** argv)
{
    reset(BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X8);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
}

void MinimalApp::updateApp()
{
    ImGui::Begin("Hello World!");
    ImGui::End();
}

void MinimalApp::releaseSharedResources()
{
}

int main(int argc, char** argv)
{
    MinimalApp app;
    return app.run(argc, argv);
}
