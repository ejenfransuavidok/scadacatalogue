#include <assert.h>
#include "scada.hpp"

namespace scada { // ::scada

SCADA::SCADA(int argc, char *argv[], QCoreApplication &a)
    : app(a)
{
    /**
     *
     * input format: FileName DateTime Message
     *
     */
    if (argc - 1 != COUNT_ARGC)
        throw new QString("argv string must contains 3 elements, but contains " + QString::number(argc));
    this->argc = argc;
    this->argv = argv;
    this->FileName = this->prepare_file_name_ext(argv[1], "xlsx");
    this->DateTime = argv[2];
    this->Message = argv[3];
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
    this->document->write(first_empty_row, MESSAGE_COL, this->Message, format);
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

} // ::scada
