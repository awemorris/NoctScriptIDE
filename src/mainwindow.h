#ifndef NOCTSCRIPT_BOOTWINDOW_H
#define NOCTSCRIPT_BOOTWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QSplitter>
#include <QTabWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QVector>

#include "texteditor.h"

class MainWindow : QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    static void runREPL();
    static void runBatchScript(const QString& fileName);
    static void runGameScript(const QString& fileName);

private:
    // Singleton instance.
    static MainWindow *singleton;

    // Tool Bar
    QWidget *m_toolBar;

    // Main Splitter
    QSplitter *m_splitter;

    // Project Tab View
    QWidget *m_projectView;
    QTabWidget *m_projectTabView;

    // File Tree View
    QFileSystemModel *m_fileTreeModel;
    QTreeView *m_fileTreeView;

    // Main Tab View
    QTabWidget *m_mainTabView;

    // Welcome Tab
    QWidget *m_welcomeTab;

    // New File Tab
    QWidget *m_newFileTab;

    // File Name (New File Tab)
    QLineEdit *m_fileNameEdit;

    // Parent Folder (New File Tab)
    QLineEdit *m_parentFolderEdit;

    // Editor Tabs
    QVector<TextEditTab *> m_editorTabs;

    void createViews();
    void createProjectView();
    void createToolBar();
    void createProjectTabView();
    void createFileTreeView();
    void createMainTabView();
    void createWelcomeTab();
    void createNewFileTab();

    void addTextEditorTab(const QString& fileName);

private slots:
    void onClickNewFile();
    void onCloseTab(int index);
    void onFileDoubleClicked(const QModelIndex &index);
    void onClickChooseParentFolder();
    void onClickCreateNewFile();
};

#endif
