#include <assert.h>
#include <QRegExp>
#include "scada.hpp"

namespace scada { // ::scada

SCADA::SCADA(int argc, char *argv[], QCoreApplication &a)
    : app(a)
{
    /**
     *
     * input format: [FileName][DateTime][Message]
     *
     */
    if (! this->parse_input(argc, argv))
        throw new QString("input format: [FileName][DateTime][Message]");
    this->FileName = this->prepare_file_name_ext(this->input[FILENAME], "xlsx");
    this->DateTime = this->input[DATETIME];
    this->Message = this->input[MESSAGE];
    this->path = QDir(QDir::currentPath()).filePath(this->FileName);
    this->document = new QXlsx::Document(this->path);
    assert(this->document);
    this->document->addSheet(SHEET_NAME);
}

int SCADA::get_first_empty() {
    for (int row = 1, col = 1; row < MAX_ROW_INDEX; row++) {
        if (this->document->read(row, col).isNull())
            return row;
    }
    return MAX_ROW_INDEX;
}

void SCADA::init() {
    QXlsx::Format format;
    int first_empty_row = this->get_first_empty();
    int c = first_empty_row % 2 ? 200 : 255;
    format.setPatternBackgroundColor(QColor(c, c, c));
    format.setBorderColor(Qt::black);
    format.setBorderStyle(QXlsx::Format::BorderThin);
    this->document->setColumnWidth(DATETIME_COL, 20.);
    this->document->setColumnWidth(MESSAGE_COL, 40.);
    this->document->write(first_empty_row, DATETIME_COL, this->DateTime, format);
    this->document->write(first_empty_row, MESSAGE_COL, this->decoder(this->Message), format);
    this->document->saveAs(this->path);
    exit(0);
}

QString SCADA::prepare_file_name_ext(QString filename, QString ext, QString append) {
    QStringList pieces = filename.split( "." );
    QString fname;
    for (int i=0; i<pieces.size() - 1; i++)
        fname += pieces.at(i);
    fname += append;
    if (pieces.size() == 1) {
        fname += pieces.at(0) + "." + ext;
    }
    else {
        fname +=  ("." + ext);
    }
    return fname;
}

bool SCADA::parse_input(int argc, char *argv[]) {
    QRegExp rx("\\[([^\\]\\[]+)\\]");
    QString str;
    for(int i=1; i<argc; i++) {
        str += argv[i];
    }
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1) {
        qDebug() << rx.cap(1);
        this->input << rx.cap(1);
        pos += rx.matchedLength();
    }
    return (this->input.size() == 3);
}

QString SCADA::decoder(QString inp) {
    QTextCodec* defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QTextDecoder *decoder = new QTextDecoder(defaultTextCodec);
    return decoder->toUnicode(inp.toLatin1());
}

} // ::scada
