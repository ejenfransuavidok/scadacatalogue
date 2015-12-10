#ifndef SCADA_HPP
#define SCADA_HPP

#include <QFileInfo>
#include <ods/ods>

#define FILE_PATH "/home/vidok/file.ods"
#define COUNT_ARGC 3
#define MAX_ROW_INDEX 2<<16
#define DOCUMENT_DOESNOT_EXIST -1
#define DATETIME_COL 0
#define MESSAGE_COL 1

namespace scada { // ::scada

class SCADA {

public:
    SCADA(int argc, char *argv[]);
    void init();

private:
    bool fileExists(QString path);
    int get_first_empty();
    void prepare_new_book(QString append="");
    void write_data();
    void save_file();
    QString prepare_file_name_ods(QString filename, QString append="");
    void set_even_style(ods::Style * style);
    void set_odd_style(ods::Style * style);

    int argc;
    char **argv;
    int docstatus;
    int current_row;
    QString path;
    QString FileName;
    QString DateTime;
    QString Message;
    ods::Book *book;
    ods::Sheet *sheet;
};

} // ::scada

#endif // SCADA_HPP
