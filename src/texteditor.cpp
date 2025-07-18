#include "texteditor.h"
#include "mainwindow.h"

#include <QPainter>
#include <QTextBlock>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

TextEditor::TextEditor(const QString& fileName)
    : QPlainTextEdit(nullptr), m_fileName(fileName)
{
    // Initialize the font.
    QFont font;
#if defined(Q_OS_WIN)
    font.setFamily("Consolas");
    font.setPointSize(13);
#elif defined(Q_OS_MAC)
    font.setFamily("Monaco");
    font.setPointSize(14);
#else
    font.setFamily("Noto Sans Mono");
    font.setPointSize(13);
#endif
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    this->setFont(font);

    // Initialize the line number area.
    m_lineNumberArea = new LineNumberArea(this);
    connect(this, &TextEditor::blockCountChanged, this, &TextEditor::updateLineNumberAreaWidth);
    connect(this, &TextEditor::updateRequest, this, &TextEditor::updateLineNumberArea);
    connect(this, &TextEditor::cursorPositionChanged, this, &TextEditor::highlightCurrentLine);
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    // Initialize the text highlight.
    Highlighter *highlighter = new Highlighter(this->document());

    // Load the file content.
    if (!m_fileName.isEmpty() && QFile::exists(m_fileName)) {
        QFile file(m_fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString contents = QString::fromUtf8(file.readAll());
            file.close();
            this->setPlainText(contents);
        } else {
            QMessageBox::warning(this, "Error", "Failed to load the file.");
        }
    }
}

TextEditor::~TextEditor()
{
}

QString TextEditor::getFileName()
{
    return m_fileName;
}

// Slot: blockCountChanged
void TextEditor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


// Slot: updateRequest
void TextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

// Slot: cursorPositionChanged
void TextEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(45, 45, 45);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void TextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    // Fill by gray.
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), QColor(30, 30, 30));

    // Draw line numbers.
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::white);
            painter.drawText(0,
                             top,
                             m_lineNumberArea->width() - 20,
                             fontMetrics().height(),
                             Qt::AlignRight,
                             number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        blockNumber++;
    }
}

void TextEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(
        QRect(cr.left(),
              cr.top(),
              lineNumberAreaWidth(),
              cr.height()));
}

void TextEditor::keyPressEvent(QKeyEvent *event)
{
    // Shift+Return or Ctrl+O:
    // insert a hard return instead of a soft return on Shift+Return
    if (
#if defined(Q_OS_MAC)
        (event->key() == Qt::Key_Return && event->modifiers() == Qt::ShiftModifier) ||
        (event->key() == Qt::Key_O && event->modifiers() == Qt::MetaModifier)
#else
        (event->key() == Qt::Key_Return && event->modifiers() == Qt::ShiftModifier)
#endif
        ) {
        QTextCursor cursor = textCursor();
        cursor.insertBlock();
        setTextCursor(cursor);
        return;
    }

    // Ctrl+S: save to file.
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S) {
        saveToFile();
        return;
    }

    // Ctrl+Shift+R: run a batch script.
    if (event->key() == Qt::Key_R && event->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier)) {
        if (!m_fileName.isEmpty())
            MainWindow::runBatchScript(m_fileName);
        return;
    }

    // Ctrl+R: run a game.
    if (event->key() == Qt::Key_R && event->modifiers() == Qt::ControlModifier) {
        MainWindow::runGameScript();
        return;
    }

    QPlainTextEdit::keyPressEvent(event);
}

int TextEditor::lineNumberAreaWidth()
{
    // Get the digits of the max line number.
    int digits = 1;
    int maxLineNumber = qMax(1, blockCount());
    while (maxLineNumber >= 10) {
        maxLineNumber /= 10;
        digits++;
    }

    // Calc the width by the 9's width.
    int width = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + 20;

    return width;
}

void TextEditor::saveToFile()
{
    if (m_fileName.isEmpty()) {
        m_fileName = QFileDialog::getSaveFileName(
            this,
            "Save File",
            "",
            "Text Files (*.txt);;All Files (*)");
        if (m_fileName.isEmpty())
            return;
    }

    QFile file(m_fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << this->toPlainText();
        file.close();
    } else {
        QMessageBox::warning(this,
                             "Error",
                             "Failed to save the file.");
    }
}

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::gray);
    //keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywordPatterns = {
        "\\bfunc\\b", "\\bif\\b", "\\belse\\b", "\\bwhile\\b", "\\breturn\\b",
        "\\bbreak\\b", "\\bcontinue\\b", "\\bvar\\b"
    };

    for (const QString &pattern : keywordPatterns) {
        HighlightingRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::darkGreen);
    commentFormat.setFontItalic(true);

    HighlightingRule commentRule;
    commentRule.pattern = QRegularExpression("//[^\n]*");
    commentRule.format = commentFormat;
    highlightingRules.append(commentRule);
}

void Highlighter::highlightBlock(const QString& text)
{
    for (HighlightingRule &rule : highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

TextEditTab::TextEditTab(const QString& fileName)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_textEdit = new TextEditor(fileName);

    layout->addWidget(m_textEdit);
}

TextEditTab::~TextEditTab()
{
}

QString TextEditTab::getFileName()
{
    return m_textEdit->getFileName();
}
