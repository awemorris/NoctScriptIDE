#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>
#include <QKeyEvent>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

//
// TextEditor.
//
class TextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEditor(const QString& fileName);
    ~TextEditor();

    QString getFileName();

    int lineNumberAreaWidth();
    void lineNumberAreaPaintEvent(QPaintEvent *event);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QString m_fileName;
    QWidget *m_lineNumberArea;

    void saveToFile();
};

//
// Text highlight control for TextEditor.
//
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr);
    
protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
};

//
// Line number widget for TextEditor.
//
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(TextEditor *editor) : QWidget(editor), textEditor(editor) {}

    QSize sizeHint() const override {
        return QSize(textEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        textEditor->lineNumberAreaPaintEvent(event);
    }

private:
    TextEditor *textEditor;
};

//
// Tab Widget for TextEditor.
//
class TextEditTab : public QWidget
{
public:
    TextEditTab(const QString& fileName);
    ~TextEditTab();

    QString getFileName();

private:
    TextEditor *m_textEdit;
};

#endif
