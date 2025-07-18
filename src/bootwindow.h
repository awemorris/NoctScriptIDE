#ifndef STUDIO_BOOTWINDOW_H
#define STUDIO_BOOTWINDOW_H

#include <QMainWindow>

class BootWindow : QMainWindow
{
public:
	BootWindow();
	~BootWindow();

private:
	QTabWidget* tabs;
};

#endif
