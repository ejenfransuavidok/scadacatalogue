#include "scada.hpp"

namespace scada { // ::scada

SCADA::SCADA(int argc, char *argv[]) {
    /**
     *
     * input format: FileName DateTime Message
     *
     */
    if (argc - 1 != COUNT_ARGC)
        throw new QString("argv string must contains 3 elements, but contains " + QString::number(argc));
    this->argc = argc;
    this->argv = argv;
    this->FileName = this->prepare_file_name_ods(argv[1]);
    this->DateTime = argv[2];
    this->Message = argv[3];
    this->path = QDir(QDir::currentPath()).filePath(this->FileName);
}

void SCADA::init() {
    QDate date;
    int first_empty_row = this->get_first_empty();
    switch (first_empty_row) {
    case DOCUMENT_DOESNOT_EXIST:
        this->prepare_new_book();
        break;
    case MAX_ROW_INDEX:
        this->prepare_new_book(date.currentDate().toString("dd_MM_yyyy"));
        break;
    default:
        this->current_row = first_empty_row;
    }
    this->write_data();
    this->save_file();
}

void SCADA::save_file() {
    QFile target(this->path);
    QString err = this->book->Save(target);
    if (!err.isEmpty())
        qDebug() << "Error saving ods file:" << err;
    else
        qDebug() << "Saved to" << target.fileName();
}

void SCADA::write_data() {
    qDebug() << "current_row = " << this->current_row;
    auto *row = this->sheet->CreateRow();

    auto *cell_date = row->CreateCell(DATETIME_COL);
    cell_date->SetValue(this->DateTime);
    auto *cell_message = row->CreateCell(MESSAGE_COL);
    cell_message->SetValue(this->Message);

    if (this->current_row % 2 == 0) {
        auto * style = cell_date->style();
        //this->set_even_style(style);
    }
    else {
        auto * style = cell_message->style();
        //this->set_odd_style(style);
    }
    //cell_date->SetStyle(style);
    //cell_message->SetStyle(style);
}

void SCADA::set_even_style(ods::Style * style) {
    const int c = 200;
    style->SetBackgroundColor(QColor(c, c, c));
    style->SetHAlignment(ods::HAlign::Center);
    style->SetVAlignment(ods::VAlign::Middle);
    style->SetItalic(true);
}

void SCADA::set_odd_style(ods::Style * style) {
    const int c = 255;
    style->SetBackgroundColor(QColor(c, c, c));
    style->SetHAlignment(ods::HAlign::Center);
    style->SetVAlignment(ods::VAlign::Middle);
    style->SetItalic(true);
}

int SCADA::get_first_empty() {
    QFile target(this->path);
    if (! target.exists()) {
        return DOCUMENT_DOESNOT_EXIST;
    }
    else {
        this->book = new ods::Book(this->path);
        this->sheet = this->book->sheet("CATALOG");
        if (this->sheet == nullptr)
            throw new QString("sheet 0 is null");
        //qDebug() << "CalcRowCount = " << this->sheet->CalcRowCount();
        return this->sheet->CalcRowCount();
        /*
        for (int at_row=0, col=0; at_row < MAX_ROW_INDEX; at_row++) {

            auto *row = this->sheet->row(col);
            if (row == nullptr) {
                qDebug() << "row = " << at_row ;
                return at_row;
            }
        }
        */
    }
    return MAX_ROW_INDEX;
}

void SCADA::prepare_new_book(QString append) {
    this->book = new ods::Book();
    if (this->book == nullptr)
        throw new QString("can' create new book");
    this->sheet = this->book->CreateSheet("CATALOG");
    if (this->sheet == nullptr)
        throw new QString("can' create new sheet");
    this->FileName = this->prepare_file_name_ods(this->FileName, append);
    this->path = QDir(QDir::currentPath()).filePath(this->FileName);
    if (this->fileExists(this->path))
        throw new QString("file " + this->path + " exist in that directory");
    this->current_row = this->sheet->CalcRowCount();
}

QString SCADA::prepare_file_name_ods(QString filename, QString append) {
    QStringList pieces = filename.split( "." );
    QString fname;
    for (int i=0; i<pieces.size() - 1; i++)
        fname += pieces.at(i);
    fname += append;
    if (pieces.size() == 1) {
        fname += pieces.at(0) + ".ods";
    }
    else {
        fname +=  ".ods";
    }
    return fname;
}

bool SCADA::fileExists(QString path) {
    QFileInfo checkFile(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}

} // ::scada
