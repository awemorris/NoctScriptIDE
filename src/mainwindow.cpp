#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QHeaderView>
#include <QProcess>
#include <QMessageBox>
#include <QFileDialog>

MainWindow *MainWindow::singleton;
QString MainWindow::projectDir;

int main(int argc, char *argv[])
{
    // Make the app.
    QApplication *app;
    app = new QApplication(argc, argv);
    app->setWindowIcon(QIcon(":/icons/icon256.png"));

    // Set the Fusion style to the app..
    QApplication::setStyle("Fusion");

    // Set a dark color palette to the app.
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,        QColor(45, 45, 45));
    darkPalette.setColor(QPalette::WindowText,    Qt::white);
    darkPalette.setColor(QPalette::Base,          QColor(30, 30, 30));
    darkPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ToolTipBase,   Qt::white);
    darkPalette.setColor(QPalette::ToolTipText,   Qt::white);
    darkPalette.setColor(QPalette::Text,          Qt::white);
    darkPalette.setColor(QPalette::Button,        QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText,    Qt::white);
    darkPalette.setColor(QPalette::BrightText,    Qt::red);
    app->setPalette(darkPalette);

    // Create the main window.
    MainWindow *mainWindow;
    mainWindow = new MainWindow();

    // Run the app.
    return app->exec();
}

MainWindow::MainWindow()
{
    MainWindow::singleton = this;

    this->setWindowIcon(QIcon(":/icons/icon256.png"));

    this->createViews();
    this->show();

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    // Set the dark mode to the window title bar if available.
    void enableDarkTitleBar(QWidget* widget);
    enableDarkTitleBar(this);
#endif
}

MainWindow::~MainWindow()
{
}

void MainWindow::createViews()
{
    this->setWindowTitle(tr("NoctCraft IDE"));
    
    this->createProjectView();
    this->createMainTabView();

    m_splitter = new QSplitter();
    m_splitter->addWidget(m_projectView);
    m_splitter->addWidget(m_mainTabView);
    m_splitter->setCollapsible(0, false);
    m_splitter->setCollapsible(1, false);
    m_splitter->setSizes({200, 900});

    this->setCentralWidget(m_splitter);
    this->resize(1280, 720);
}

void MainWindow::createProjectView()
{
    m_projectView = new QWidget();

    this->createToolBar();
    this->createProjectTabView();

    QVBoxLayout *layout = new QVBoxLayout(m_projectView);
    layout->addWidget(m_toolBar);
    layout->addWidget(m_projectTabView);
    layout->setContentsMargins(5, 0, 0, 5);
}

void MainWindow::createToolBar()
{
    m_toolBar = new QWidget();

    QHBoxLayout *layout = new QHBoxLayout(m_toolBar);
//    layout->setContentsMargins(6, 4, 6, 4);
//    layout->setSpacing(12);

    QToolButton *newBtn = new QToolButton();
    newBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    newBtn->setToolTip("New Project");
    newBtn->setAutoRaise(true);

    QToolButton *openBtn = new QToolButton();
    openBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    openBtn->setToolTip("New Project");
    openBtn->setAutoRaise(true);

    QToolButton *someBtn1 = new QToolButton();
    someBtn1->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    someBtn1->setToolTip("New Project");
    someBtn1->setAutoRaise(true);

    QToolButton *someBtn2 = new QToolButton();
    someBtn2->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    someBtn2->setToolTip("New Project");
    someBtn2->setAutoRaise(true);

    QToolButton *someBtn3 = new QToolButton();
    someBtn3->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    someBtn3->setToolTip("New Project");
    someBtn3->setAutoRaise(true);

    layout->addWidget(openBtn);
    layout->addWidget(newBtn);
    layout->addWidget(someBtn1);
    layout->addWidget(someBtn2);
    layout->addWidget(someBtn3);
}

void MainWindow::createProjectTabView()
{
    m_projectTabView = new QTabWidget();
    m_projectTabView->tabBar()->setMovable(true);
    m_projectTabView->setMinimumWidth(200);
    m_projectTabView->setTabPosition(QTabWidget::South);

    this->createFileTreeView();
    m_projectTabView->addTab(m_fileTreeView, tr("Project"));
}

void MainWindow::createFileTreeView()
{
    m_fileTreeModel = new QFileSystemModel();
    m_fileTreeModel->setRootPath(QDir::homePath() + "/src/");

    m_fileTreeView = new QTreeView();
    m_fileTreeView->setModel(m_fileTreeModel);
    m_fileTreeView->setRootIndex(m_fileTreeModel->index(QDir::homePath()));
    m_fileTreeView->setColumnHidden(1, true);
    m_fileTreeView->setColumnHidden(2, true);
    m_fileTreeView->setColumnHidden(3, true);
    m_fileTreeView->header()->hide();

    QObject::connect(m_fileTreeView,
                     &QTreeView::doubleClicked,
                     this,
                     [=](const QModelIndex &index) {
                         this->onFileDoubleClicked(index);
                     });
}

void MainWindow::createMainTabView()
{
    m_mainTabView = new QTabWidget();
    m_mainTabView->tabBar()->setMovable(true);
    m_mainTabView->setMinimumWidth(200);
    m_mainTabView->setTabsClosable(true);

    QObject::connect(m_mainTabView, &QTabWidget::tabCloseRequested, [=](int index) {
        onCloseTab(index);
    });

    this->createWelcomeTab();
    m_mainTabView->addTab(m_welcomeTab, tr("Welcome"));
}

void MainWindow::createWelcomeTab()
{
    m_welcomeTab = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout(m_welcomeTab);

    layout->addStretch();

    QLabel* title1Label = new QLabel(tr("NoctCraft IDE"));
    title1Label->setStyleSheet("font-size: 36pt;");
    layout->addWidget(title1Label, 0, Qt::AlignHCenter);

    QLabel* title2Label = new QLabel(tr("Version 2025.1"));
    title2Label->setStyleSheet("font-size: 16pt;");
    layout->addWidget(title2Label, 0, Qt::AlignHCenter);

    layout->addStretch();

    QPushButton* newButton = new QPushButton(tr("New Project"));
    newButton->setStyleSheet("font-size: 12pt;");
    QObject::connect(newButton, &QPushButton::clicked, [this]() {
        this->onClickNewProject();
    });
    layout->addWidget(newButton, 0, Qt::AlignHCenter);

    QPushButton* openButton = new QPushButton(tr("Open Project"));
    openButton->setStyleSheet("font-size: 12pt;");
    QObject::connect(openButton, &QPushButton::clicked, [this]() {
        this->onClickOpenProject();
    });
    layout->addWidget(openButton, 0, Qt::AlignHCenter);

    layout->addStretch();
}

void MainWindow::createNewProjectTab()
{
    if (m_newProjectTab != nullptr)
        return;

    m_newProjectTab = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(m_newProjectTab);

    layout->addStretch();

    QLabel* titleLabel = new QLabel(tr("New Project"));
    titleLabel->setStyleSheet("font-size: 24pt;");
    layout->addWidget(titleLabel, 0, Qt::AlignHCenter);

    QLabel* nameLabel = new QLabel(tr("Project Name:"));
    layout->addWidget(nameLabel, 0, Qt::AlignHCenter);

    m_projectNameEdit = new QLineEdit(tr(""));
    layout->addWidget(m_projectNameEdit, 0, Qt::AlignHCenter);

    QLabel* baseLabel = new QLabel(tr("Parent Folder:"));
    layout->addWidget(baseLabel, 0, Qt::AlignHCenter);

    m_parentFolderEdit = new QLineEdit(tr(""));
    m_parentFolderEdit->setFixedWidth(300);
    layout->addWidget(m_parentFolderEdit, 0, Qt::AlignHCenter);

    QPushButton* chooseButton = new QPushButton(tr("Choose Folder"));
    QObject::connect(chooseButton, &QPushButton::clicked, [this]() {
        this->onClickChooseProjectFolder();
    });
    layout->addWidget(chooseButton, 0, Qt::AlignHCenter);

    QPushButton* createButton = new QPushButton(tr("Create"));
    QObject::connect(createButton, &QPushButton::clicked, [this]() {
        this->onClickCreateNewProject();
    });
    layout->addWidget(createButton, 0, Qt::AlignHCenter);

    layout->addStretch();
    layout->addStretch();

    m_mainTabView->addTab(m_newProjectTab, tr("New Project"));
    m_mainTabView->setCurrentWidget(m_newProjectTab);
}

void MainWindow::addTextEditorTab(const QString& fileName)
{
    QString title;
    if (fileName.isEmpty())
        title = tr("New File");
    else
        title = QFileInfo(fileName).fileName();

    TextEditTab *tab = new TextEditTab(fileName);
    m_editorTabs.append(tab);
    m_mainTabView->addTab(tab, title);
    m_mainTabView->setCurrentWidget(tab);
}

void MainWindow::runBatchScript(const QString& fileName)
{
#if defined(Q_OS_MAC)
    // Get the noct command path. (Contents/MacOS/noct)
    QString binPath = QCoreApplication::applicationDirPath() + "/noct";

    // Make a command string.
    QString command = binPath + " " + fileName;

    // Make an Apple Script string.
    QString appleScript = QString(
        "tell application \"Terminal\"\n"
        "    activate\n"
        "    do script \"clear; %1; exit > /dev/null 2&>1;\"\n"
        "end tell\n").arg(command);

    // Execute
    QProcess process;
    process.startDetached("/usr/bin/osascript", {"-e", appleScript});
#elif defined(Q_OS_WIN)
    // Get the noct.exe path.
    QString binPath = QCoreApplication::applicationDirPath() + "/noct.exe";

    // Make a command string.
    QString command = binPath + " " + fileName + " & pause";
    command.replace('/', '\\');

    // Execute.
    QProcess process;
    process.startDetached("cmd.exe", {"/c", "start", "/wait", "cmd.exe", "/c", command});
#else
    // Get the noct command path.
    QString binPath = QCoreApplication::applicationDirPath();
    if (binPath.isEmpty()) {
        // On OpenBSD, we can't get the application binary path.
        binPath = "/usr/local/bin";
    }
    binPath = binPath + "/noct";

    // Get a terminal.
    QString term;
    if (QFile::exists("/usr/bin/xterm")) {
        term = "xterm";
    } else if (QFile::exists("/usr/X11R6/bin/xterm")) {
        term = "xterm";
    } else if (QFile::exists("/usr/X11R7/bin/xterm")) {
        term = "xterm";
    } else {
        QMessageBox::warning(nullptr, "Error", "xterm not found.");
        return;
    }

    // Execute.
    QProcess process;
    process.startDetached(term, {"-u8", "-hold", "-e", binPath, fileName});
#endif
}

void MainWindow::runGameScript()
{
    if (MainWindow::projectDir.isEmpty())
        return;

    // Get the noctvm command path.
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    QString binPath = QCoreApplication::applicationDirPath() + "/noctvm";
#elif defined(Q_OS_WIN)
    QString binPath = QCoreApplication::applicationDirPath() + "/noctvm.exe";
    binPath.replace('/', '\\');
#endif

    // Execute.
    QProcess::startDetached(binPath, {".", "."}, MainWindow::projectDir);
}

void MainWindow::onCloseTab(int index)
{
    QWidget *tab = m_mainTabView->widget(index);

    if (tab == m_welcomeTab)
        m_welcomeTab = NULL;
    else if (tab == m_newProjectTab)
        m_newProjectTab = NULL;
    else
        m_editorTabs.removeOne(tab);

    m_mainTabView->removeTab(index);

    delete tab;
}

void MainWindow::onFileDoubleClicked(const QModelIndex &index)
{
    QString filePath = m_fileTreeModel->filePath(index);
    for (TextEditTab *tab : m_editorTabs) {
        if (tab->getFileName() == filePath) {
            m_mainTabView->setCurrentWidget(tab);
            return;
        }
    }

    QFileInfo info(filePath);
    if (info.isFile())
        this->addTextEditorTab(filePath);
}

void MainWindow::onClickNewProject()
{
    this->createNewProjectTab();
}

void MainWindow::onClickChooseProjectFolder()
{
    QString dir = QFileDialog::getExistingDirectory(
        nullptr,
        "Choose a parent folder.",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty())
        return;

    m_parentFolderEdit->setText(dir);
}

void MainWindow::onClickCreateNewProject()
{
    QString name = m_projectNameEdit->text();
    QString base = m_parentFolderEdit->text();
    QString full = base + "/" + name;

    // Create the project directory.
    QDir dir;
    if (!dir.mkpath(full)) {
        QMessageBox::warning(nullptr, "Error", "Cannot create a folder.");
        return;
    }

    // Set the project directory.
    MainWindow::projectDir = full;

    // Update the file tree.
    m_fileTreeModel->setRootPath(MainWindow::projectDir);

    // Open a new main script.
    QString filePath = full + "/main.ns";
    for (TextEditTab *tab : m_editorTabs) {
        if (tab->getFileName() == filePath) {
            m_mainTabView->setCurrentWidget(tab);
            return;
        }
    }
    this->addTextEditorTab(full + "/main.ns");
}

void MainWindow::onClickOpenProject() {
    QString dir = QFileDialog::getExistingDirectory(
        nullptr,
        "Choose a project folder.",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty())
        return;

    // Set the project directory.
    MainWindow::projectDir = dir;

    // Update the file tree.
    m_fileTreeModel->setRootPath(dir);

    // Open the main script.
    this->addTextEditorTab(dir + "/main.ns");
}
