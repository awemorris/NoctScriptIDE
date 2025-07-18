#include "bootwindow.h"

#include <QTabWidget>
#include <QWidget>
#include <QTabBar>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>

BootWindow::BootWindow()
{
    this->setWindowIcon(QIcon(":/icons/icon256.png"));

    this->tabs = new QTabWidget();

    {
        QWidget* welcomePage = new QWidget();
        QVBoxLayout* welcomePageLayout = new QVBoxLayout(welcomePage);

        QLabel* title1Label = new QLabel(tr("NoctScript IDE"));
        title1Label->setStyleSheet("font-size: 36pt;");
        welcomePageLayout->addStretch();
        welcomePageLayout->addWidget(title1Label, 0, Qt::AlignHCenter);

        QLabel* title2Label = new QLabel(tr("Version 2025.1"));
        title2Label->setStyleSheet("font-size: 16pt;");
        welcomePageLayout->addWidget(title2Label, 0, Qt::AlignHCenter);
        welcomePageLayout->addStretch();

        QPushButton* newButton = new QPushButton(tr("New Project"));
        newButton->setStyleSheet("font-size: 12pt;");
        QObject::connect(newButton, &QPushButton::clicked, [this]() {
            this->tabs->setCurrentIndex(1);
        });
        welcomePageLayout->addWidget(newButton, 0, Qt::AlignHCenter);

        QPushButton* openButton = new QPushButton(tr("Open Project"));
        openButton->setStyleSheet("font-size: 12pt;");
        welcomePageLayout->addWidget(openButton, 0, Qt::AlignHCenter);

        welcomePageLayout->addStretch();
        welcomePageLayout->addStretch();

        tabs->addTab(welcomePage, tr("Welcome"));
    }

    {
        QWidget* newPage = new QWidget();
        QVBoxLayout* newPageLayout = new QVBoxLayout(newPage);

        newPageLayout->addStretch();

        QLabel* titleLabel = new QLabel(tr("New Project"));
        titleLabel->setStyleSheet("font-size: 24pt;");
        newPageLayout->addWidget(titleLabel, 0, Qt::AlignHCenter);

        newPageLayout->addStretch();

        QLabel* nameLabel = new QLabel(tr("Project Name:"));
        newPageLayout->addWidget(nameLabel, 0, Qt::AlignHCenter);

        QLineEdit* nameEdit = new QLineEdit(tr(""));
        newPageLayout->addWidget(nameEdit, 0, Qt::AlignHCenter);

        newPageLayout->addStretch();

        QLabel* baseLabel = new QLabel(tr("Base Path:"));
        newPageLayout->addWidget(baseLabel, 0, Qt::AlignHCenter);

        QLineEdit* baseEdit = new QLineEdit(tr(""));
        baseEdit->setFixedWidth(300);
        newPageLayout->addWidget(baseEdit, 0, Qt::AlignHCenter);

        QPushButton* chooseButton = new QPushButton(tr("Choose Folder"));
        QObject::connect(chooseButton, &QPushButton::clicked, []() { });
        newPageLayout->addWidget(chooseButton, 0, Qt::AlignHCenter);

        newPageLayout->addStretch();

        QPushButton* createButton = new QPushButton(tr("Create"));
        QObject::connect(createButton, &QPushButton::clicked, []() { });
        newPageLayout->addWidget(createButton, 0, Qt::AlignHCenter);

        newPageLayout->addStretch();
        newPageLayout->addStretch();

        tabs->addTab(newPage, tr("New Project"));
    }

    {
        QWidget* openPage = new QWidget();
        QVBoxLayout* openPageLayout = new QVBoxLayout(openPage);
        openPageLayout->addWidget(new QLabel(tr("This is a content of the tab to open a project.")));
        tabs->addTab(openPage, tr("Open Project"));
    }
    
    //tabs->tabBar()->setMovable(true);

    this->setCentralWidget(tabs);
    this->resize(600, 400);
    this->setWindowTitle(tr("Welcome to NoctScript IDE"));
    this->show();

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    // Set the dark mode to the window title bar if available.
    void enableDarkTitleBar(QWidget* widget);
    enableDarkTitleBar(this);
#endif
}

BootWindow::~BootWindow()
{
}
