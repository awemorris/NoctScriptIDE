#include <Cocoa/Cocoa.h>

#include <QWindow>
#include <QWidget>

void enableDarkTitleBar(QWidget* widget)
{
    QWindow* window = widget->windowHandle();
    NSView* nsView = (NSView*)window->winId();
    NSWindow* nsWindow = [nsView window];

    [nsWindow setTitlebarAppearsTransparent:YES];
    [nsWindow setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameVibrantDark]];
}

QString getBundlePath()
{
    NSString *bundlePath = [[NSBundle mainBundle] bundlePath];
    return QString::fromUtf8([bundlePath UTF8String]);
}
