#pragma once
class App
{
    unsigned int texture_id;
    void *window;
  protected:
    App(int width, int height, const char *title);
    void run();
    virtual void resizeCallback(int width, int height) = 0;
    virtual void init(int width, int height) = 0;
    virtual void renderDebugUI() = 0;
    virtual const void *render(int width, int height) = 0;
    virtual void cleanup() = 0;
    virtual ~App();
  public:
    bool IsKeyDown(unsigned short key) const noexcept;
};