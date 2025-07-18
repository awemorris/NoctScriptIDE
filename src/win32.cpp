#include <windows.h>

#include <QWidget>
#include <QWindow>

void enableDarkTitleBar(QWidget* widget)
{
    QWindow *window = widget->windowHandle();

    HWND hwnd = (HWND)window->winId();

    HMODULE hUser = GetModuleHandleA("dwmapi.dll");
    if (hUser != NULL)
    {
        HRESULT (*DwmSetWindowAttribute)(HWND, DWORD, LPCVOID, DWORD) =
            (HRESULT (*)(HWND, DWORD, LPCVOID, DWORD))GetProcAddress(hUser, "DwmSetWindowAttribute");
        if (DwmSetWindowAttribute != NULL)
        {
            BOOL value = TRUE;
            DwmSetWindowAttribute(hwnd,
                                  20 /* DWMWA_USE_IMMERSIVE_DARK_MODE */,
                                  &value,
                                  sizeof(value));
        }
    }
}
