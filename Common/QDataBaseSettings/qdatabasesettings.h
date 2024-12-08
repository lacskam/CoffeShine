#ifndef QDATABASESETTINGS_H
#define QDATABASESETTINGS_H

#include <QObject>

class QDataBaseSettings
{
    Q_ENUMS(typeDriver)
public:
    QDataBaseSettings();

    enum typeDriver {
        typeQDB2,       //IBM DB2
        typeQIBASE,     //Borland InterBase Driver
        typeQMYSQL,     //MySQL Driver
        typeQOCI,       //Oracle Call Interface Driver
        typeQODBC,      //ODBC Driver (includes Microsoft SQL Server)
        typeQPSQL,      //PostgreSQL Driver
        typeQSQLITE,    //SQLite version 3 or above
        typeQSQLITE2,   //SQLite version 2
        typeQTDS        //Sybase Adaptive Server
    };

    typeDriver type;
    QString host;
    QString dataBaseName;
    int port;
    QString login;
    QString password;
};

#endif // QDATABASESETTINGS_H
