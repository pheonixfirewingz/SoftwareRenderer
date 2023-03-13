#pragma once
class App
{
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
    bool IsKeyDown(unsigned short key) const noexcept;
};