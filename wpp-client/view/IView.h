#ifndef IVIEW_H
#define IVIEW_H

class IView
{
public:
    virtual ~IView() = default;
    virtual void drawView() = 0;
    virtual void inactiveView() = 0;
};

#endif // IVIEW_H
