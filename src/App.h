#pragma once
class App
{
    static App *active_channel;
    void *window;

  protected:
    App(int width, int height, const char *title);
    void run();
    virtual void resizeCallback(int width, int height) = 0;
    virtual void init(int width, int height) = 0;
    virtual const unsigned int *render(int width, int height) = 0;
    virtual void cleanup() = 0;
    virtual ~App();

  public:
    App *getActiveApp() const noexcept
    {
        return active_channel;
    }

    bool IsKeyDown(unsigned short key) const noexcept;
};