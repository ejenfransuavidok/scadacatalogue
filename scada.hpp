#ifndef SCADA_HPP
#define SCADA_HPP

#include <QtCore>
#include <QFileInfo>
#include <QtXlsx>

#define COUNT_ARGC 3
#define MAX_ROW_INDEX 2<<16
#define DOCUMENT_DOESNOT_EXIST -1
#define DATETIME_COL 1
#define MESSAGE_COL 2
#define SHEET_NAME "Каталог событий"
#define FILENAME 0
#define DATETIME 1
#define MESSAGE 2

namespace scada { // ::scada

class SCADA {

public:
    SCADA(int argc, char *argv[], QCoreApplication &a);
    void init();

private:
    QString prepare_file_name_ext(QString filename, QString ext, QString append="");
    bool parse_input(int argc, char *argv[]);
    int get_first_empty();
    QString decoder(QString inp);

    int current_row;
    QStringList input;
    QString path;
    QString FileName;
    QString DateTime;
    QString Message;
    QCoreApplication &app;
    QXlsx::Document *document;
};

} // ::scada

#endif // SCADA_HPP
