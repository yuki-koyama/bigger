#include <bigger/app.hpp>

class MinimalApp final : public bigger::App
{
public:

    MinimalApp() {}

    void initialize(int argc, char** argv) override {}
    void updateApp() override {}
    void releaseSharedResources() override {}
};

int main(int argc, char** argv)
{
    MinimalApp app;
    return app.runApp(argc, argv);
}
