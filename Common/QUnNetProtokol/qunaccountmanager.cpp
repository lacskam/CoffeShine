#include "qunaccountmanager.h"

QUnAccountManager::QUnAccountManager(QSqlDatabase *dataBase_ ,QObject *parent) : QObject(parent)
{
    dataBase = dataBase_;

    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    initDB();
}

QUnAccountManager::~QUnAccountManager()
{

}

void QUnAccountManager::initDB()
{
    if(dataBase->driverName()=="QMYSQL")
    {
        qDebug()<<"Mysql init";

        QString textQuery =  "CREATE TABLE IF NOT EXISTS `UNusers` ("
                             "`id_UN` INT(11) NOT NULL AUTO_INCREMENT,"
                             "`UNlogin` VARCHAR(45) NOT NULL,"
                             "`UNpassword` VARCHAR(45) NOT NULL,"
                             "`UNname` VARCHAR(45) NOT NULL,"
                             "`UNpatronymic` VARCHAR(45) NOT NULL,"
                             "`UNsurname` VARCHAR(45) NOT NULL,"
                             "`UNemail` VARCHAR(45) NULL DEFAULT NULL,"
                             "`UNnumber` VARCHAR(45) NULL DEFAULT NULL,"
                             "`UNenable` TINYINT(1) NULL DEFAULT 0,"
                             "`UNvisible` TINYINT(1) NULL DEFAULT 0,"
                             "`UNrights` INT(11) NULL DEFAULT 255,"
                             "`UNregdate` DATETIME NULL DEFAULT NULL,"
                             "`UNactivate` TINYINT(1) NULL DEFAULT 0,"
                             "`UNchangepassword` TINYINT(1) NULL DEFAULT 0,"
                             "`UNdatepassword` DATETIME NULL DEFAULT NULL,"
                             "`UNactivemail` TINYINT(1) NULL DEFAULT 0,"
                             "`UNactivenumber` TINYINT(1) NULL DEFAULT 0,"
                             "`UNemailcode` INT(11) NULL DEFAULT NULL,"
                             "`UNnumbercode` INT(11) NULL DEFAULT NULL,"
                             "PRIMARY KEY (`id_UN`)) "
                             "ENGINE = InnoDB "
                             "DEFAULT CHARACTER SET = utf8;";


        if (dataBase->open())
        {
            QSqlQuery * queryCreateDatabase = new QSqlQuery(*dataBase);
            if (!queryCreateDatabase->exec(textQuery)) qDebug()<<"QUnAccountManager Error create table";
            delete queryCreateDatabase;

            dataBase->close();
        } else qDebug()<<"QUnAccountManager Error open database";
    }

    if(dataBase->driverName()=="QSQLITE")
    {
        qDebug()<<"Sqlite3 init";

        QString textQuery =  "CREATE TABLE IF NOT EXISTS `UNusers` ("
                             "id_UN INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                             "UNlogin text NOT NULL,"
                             "UNpassword text NOT NULL,"
                             "UNname text NOT NULL,"
                             "UNpatronymic text NOT NULL,"
                             "UNsurname text NOT NULL,"
                             "UNemail text NOT NULL,"
                             "UNnumber text NOT NULL,"
                             "UNenable INTEGER NOT NULL DEFAULT 0,"
                             "UNvisible INTEGER NOT NULL DEFAULT 0,"
                             "UNrights INTEGER NOT NULL DEFAULT 255,"
                             "UNregdate DATETIME NOT NULL,"
                             "UNactivate INTEGER NOT NULL DEFAULT 0,"
                             "UNchangepassword INTEGER NOT NULL DEFAULT 0,"
                             "UNdatepassword DATETIME NOT NULL,"
                             "UNactivemail INTEGER NOT NULL DEFAULT 0,"
                             "UNactivenumber INTEGER NOT NULL DEFAULT 0,"
                             "UNemailcode INTEGER NOT NULL DEFAULT 0,"
                             "UNnumbercode INTEGER NOT NULL DEFAULT 0);";


        if (dataBase->open())
        {
            QSqlQuery * queryCreateDatabase = new QSqlQuery(*dataBase);
            if (!queryCreateDatabase->exec(textQuery)) qDebug()<<"QUnAccountManager Error create table";
            delete queryCreateDatabase;

            dataBase->close();
        } else qDebug()<<"QUnAccountManager Error open database";
    }

    if(dataBase->driverName()=="QSQLITE2")
    {
        qDebug()<<"Sqlite2 init";
    }

    if(dataBase->driverName()=="QIBASE")
    {
        qDebug()<<"Firebird init";
    }
}

bool QUnAccountManager::activateEmail (int id, int code)
{
    bool Output = false;

    if (code == getEmailCode(id))
    {
        setActivateEmail(id,true);
        Output = true;
    }

    return Output;
}

bool QUnAccountManager::activateNumber (int id,int code)
{
    bool Output = false;

    if (code == getNumberCode(id))
    {
        setActivateNumber(id,true);
        Output = true;
    }

    return Output;
}

bool QUnAccountManager::createAccount (UNAccountInfo accountInfo)
{
    bool Output = false;

    if (accountInfo.regdate.isNull()) accountInfo.regdate = QDateTime::currentDateTime();
    if (accountInfo.datepassword.isNull()) accountInfo.datepassword = QDateTime::currentDateTime();

    accountInfo.activate = false;
    accountInfo.activemail = false;
    accountInfo.activenumber = false;
    accountInfo.changepassword = false;
    accountInfo.emailcode = 0;
    accountInfo.enable = false;
    accountInfo.numbercode = 0;
    accountInfo.rights = 255;
    accountInfo.visible = false;

    if (indexOf(accountInfo.login)==-1)
    {
    QString textQuery = "insert into UNusers ("
                        "UNlogin,"
                        "UNpassword,"
                        "UNname,"
                        "UNpatronymic,"
                        "UNsurname,"
                        "UNemail,"
                        "UNnumber,"
                        "UNenable,"
                        "UNvisible,"
                        "UNrights,"
                        "UNregdate,"
                        "UNactivate,"
                        "UNchangepassword,"
                        "UNdatepassword,"
                        "UNactivemail,"
                        "UNactivenumber,"
                        "UNemailcode,"
                        "UNnumbercode) "
                        "VALUES ("
                        "'"+ accountInfo.login +"',"
                        "'"+ accountInfo.password +"',"
                        "'"+ accountInfo.name +"',"
                        "'"+ accountInfo.patronymic +"',"
                        "'"+ accountInfo.surname +"',"
                        "'"+ accountInfo.email +"',"
                        "'"+ accountInfo.number +"',"
                        "'"+ QString::number(accountInfo.enable) +"',"
                        "'"+ QString::number(accountInfo.visible) +"',"
                        "'"+ QString::number(accountInfo.rights) +"',"
                        "'"+ accountInfo.regdate.toString("yyyy-MM-dd hh:mm:ss") +"',"
                        "'"+ QString::number(accountInfo.activate) +"',"
                        "'"+ QString::number(accountInfo.changepassword) +"',"
                        "'"+ accountInfo.datepassword.toString("yyyy-MM-dd hh:mm:ss") +"',"
                        "'"+ QString::number(accountInfo.activemail) +"',"
                        "'"+ QString::number(accountInfo.activenumber) +"',"
                        "'"+ QString::number(accountInfo.emailcode) +"',"
                        "'"+ QString::number(accountInfo.numbercode) +"');";

        if (dataBase->open())
        {
            QSqlQuery * queryCreateAccount = new QSqlQuery(*dataBase);
            if (!queryCreateAccount->exec(textQuery)) qDebug()<<"QUnAccountManager get id user from login";

            qDebug()<<"textQuery="<<textQuery;
            qDebug()<<"queryCreateAccount->lastError()="<<queryCreateAccount->lastError().text();
            delete queryCreateAccount;

            Output = true;

            dataBase->close();
        } else qDebug()<<"QUnAccountManager Error open database";

    } else qDebug()<<"QUnAccountManager Error create acount, already exists";

    if (Output) emit signalCreatedNewAccount();

    return Output;
}

bool QUnAccountManager::checkPassword (int id, QString password)
{
    bool Output = false;

    if (password == getPassword(id)) Output = true;

    return Output;
}

bool QUnAccountManager::checkLogin (QString login)
{
    bool Output=false;

    QString textQuery = "select id_UN from UNusers where UNlogin = '"+login+"'";

    if (dataBase->open())
    {
        QSqlQuery * queryCheckLogin = new QSqlQuery(*dataBase);
        if (!queryCheckLogin->exec(textQuery)) qDebug()<<"QUnAccountManager error check login";

        if (queryCheckLogin->next()) Output = true;


        delete queryCheckLogin;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

int QUnAccountManager::indexOf(QString login)
{
    int Output=-1;

    QString textQuery = "select id_UN from UNusers where UNlogin = '"+login+"'";

    if (dataBase->open())
    {
        QSqlQuery * queryIndexOf = new QSqlQuery(*dataBase);
        if (!queryIndexOf->exec(textQuery)) qDebug()<<"QUnAccountManager get id user from login";

        if (queryIndexOf->next()) Output = queryIndexOf->value(0).toInt();


        delete queryIndexOf;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::isEnable(int id)
{
    bool Output = false;

    QString textQuery = "select UNenable from UNusers where id_UN = '"+QString::number(id)+"'";

    if (dataBase->open())
    {
        QSqlQuery * queryEnable = new QSqlQuery(*dataBase);
        if (!queryEnable->exec(textQuery)) qDebug()<<"QUnAccountManager error get enable";

        if (queryEnable->next()) Output = queryEnable->value(0).toBool();


        delete queryEnable;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::isActivate(int id)
{
    bool Output = false;

    QString textQuery = "select UNactivate from UNusers where id_UN = '"+QString::number(id)+"'";

    if (dataBase->open())
    {
        QSqlQuery * queryActivate = new QSqlQuery(*dataBase);
        if (!queryActivate->exec(textQuery)) qDebug()<<"QUnAccountManager error get activate";

        if (queryActivate->next()) Output = queryActivate->value(0).toBool();


        delete queryActivate;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::isActivateEmail (int id)
{
    bool Output = false;

    QString textQuery = "select UNactivemail from UNusers where id_UN = '"+QString::number(id)+"'";

    if (dataBase->open())
    {
        QSqlQuery * queryActivateEmail = new QSqlQuery(*dataBase);
        if (!queryActivateEmail->exec(textQuery)) qDebug()<<"QUnAccountManager error get activate email";

        if (queryActivateEmail->next()) Output = queryActivateEmail->value(0).toBool();


        delete queryActivateEmail;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::isActivateNumber (int id)
{
    bool Output = false;

    QString textQuery = "select UNactivenumber from UNusers where id_UN = '"+QString::number(id)+"'";

    if (dataBase->open())
    {
        QSqlQuery * queryActivateNumber = new QSqlQuery(*dataBase);
        if (!queryActivateNumber->exec(textQuery)) qDebug()<<"QUnAccountManager error get activate number";

        if (queryActivateNumber->next()) Output = queryActivateNumber->value(0).toBool();


        delete queryActivateNumber;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::isVisible (int id)
{
    bool Output = false;

    QString textQuery = "select UNvisible from UNusers where id_UN = '"+QString::number(id)+"'";

    if (dataBase->open())
    {
        QSqlQuery * queryVisible = new QSqlQuery(*dataBase);
        if (!queryVisible->exec(textQuery)) qDebug()<<"QUnAccountManager error get visible";

        if (queryVisible->next()) Output = queryVisible->value(0).toBool();


        delete queryVisible;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

QVector<UNAccountInfo> QUnAccountManager::getAccountsList()
{
    QVector<UNAccountInfo> Output;

    QString textQuery = "select id_UN, "
                                "UNlogin, "
                                "UNpassword, "
                                "UNname, "
                                "UNpatronymic, "
                                "UNsurname, "
                                "UNemail, "
                                "UNnumber, "
                                "UNenable, "
                                "UNvisible, "
                                "UNrights, "
                                "UNregdate, "
                                "UNactivate, "
                                "UNchangepassword, "
                                "UNdatepassword, "
                                "UNactivemail, "
                                "UNactivenumber, "
                                "UNemailcode, "
                                "UNnumbercode from UNusers;";

    if (dataBase->open())
    {
        QSqlQuery * queryAccountsList = new QSqlQuery(*dataBase);
        if (!queryAccountsList->exec(textQuery)) qDebug()<<"QUnAccountManager error get accounts list";

        UNAccountInfo A;

        while (queryAccountsList->next())
        {
            A.id = queryAccountsList->value(0).toInt();
            A.login = queryAccountsList->value(1).toString();
            A.password = queryAccountsList->value(2).toString();
            A.name = queryAccountsList->value(3).toString();
            A.patronymic = queryAccountsList->value(4).toString();
            A.surname = queryAccountsList->value(5).toString();
            A.email = queryAccountsList->value(6).toString();
            A.number = queryAccountsList->value(7).toString();
            A.enable = queryAccountsList->value(8).toBool();
            A.visible = queryAccountsList->value(9).toBool();
            A.rights = queryAccountsList->value(10).toInt();

            QString regDate = queryAccountsList->value(11).toString();
            regDate.replace("T"," ");
            A.regdate = QDateTime::fromString(regDate,"yyyy-MM-dd hh:mm:ss");

            A.activate = queryAccountsList->value(12).toBool();
            A.changepassword = queryAccountsList->value(13).toBool();

            QString datePassword = queryAccountsList->value(14).toString();
            datePassword.replace("T"," ");
            A.datepassword = QDateTime::fromString(datePassword,"yyyy-MM-dd hh:mm:ss");

            A.activemail  = queryAccountsList->value(15).toBool();
            A.activenumber  = queryAccountsList->value(16).toBool();
            A.emailcode = queryAccountsList->value(17).toInt();
            A.numbercode = queryAccountsList->value(18).toInt();

            Output.push_back(A);
        }

        delete queryAccountsList;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

UNAccountInfo QUnAccountManager::getAccountInfo(int id)
{
    UNAccountInfo Output;

    QString textQuery = "select id_UN, "
                                "UNlogin, "
                                "UNpassword, "
                                "UNname, "
                                "UNpatronymic, "
                                "UNsurname, "
                                "UNemail, "
                                "UNnumber, "
                                "UNenable, "
                                "UNvisible, "
                                "UNrights, "
                                "UNregdate, "
                                "UNactivate, "
                                "UNchangepassword, "
                                "UNdatepassword, "
                                "UNactivemail, "
                                "UNactivenumber, "
                                "UNemailcode, "
                                "UNnumbercode from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryAccountInfo = new QSqlQuery(*dataBase);
        if (!queryAccountInfo->exec(textQuery)) qDebug()<<"QUnAccountManager error get account info";


        if (queryAccountInfo->next())
        {
            Output.id = queryAccountInfo->value(0).toInt();
            Output.login = queryAccountInfo->value(1).toString();
            Output.password = queryAccountInfo->value(2).toString();
            Output.name = queryAccountInfo->value(3).toString();
            Output.patronymic = queryAccountInfo->value(4).toString();
            Output.surname = queryAccountInfo->value(5).toString();
            Output.email = queryAccountInfo->value(6).toString();
            Output.number = queryAccountInfo->value(7).toString();
            Output.enable = queryAccountInfo->value(8).toBool();
            Output.visible = queryAccountInfo->value(9).toBool();
            Output.rights = queryAccountInfo->value(10).toInt();
            Output.regdate = QDateTime::fromString(queryAccountInfo->value(11).toString(),"yyyy-MM-dd hh:mm:ss");
            Output.activate = queryAccountInfo->value(12).toBool();
            Output.changepassword = queryAccountInfo->value(13).toBool();
            Output.datepassword = QDateTime::fromString(queryAccountInfo->value(14).toString(),"yyyy-MM-dd hh:mm:ss");
            Output.activemail  = queryAccountInfo->value(15).toBool();
            Output.activenumber  = queryAccountInfo->value(16).toBool();
            Output.emailcode = queryAccountInfo->value(17).toInt();
            Output.numbercode = queryAccountInfo->value(18).toInt();
        }

        delete queryAccountInfo;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

UNAccountInfo QUnAccountManager::getAccountInfo(QString login)
{
    UNAccountInfo Output;

    QString textQuery = "select id_UN, "
                                "UNlogin, "
                                "UNpassword, "
                                "UNname, "
                                "UNpatronymic, "
                                "UNsurname, "
                                "UNemail, "
                                "UNnumber, "
                                "UNenable, "
                                "UNvisible, "
                                "UNrights, "
                                "UNregdate, "
                                "UNactivate, "
                                "UNchangepassword, "
                                "UNdatepassword, "
                                "UNactivemail, "
                                "UNactivenumber, "
                                "UNemailcode, "
                                "UNnumbercode from UNusers where UNlogin = '"+login+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryAccountInfo = new QSqlQuery(*dataBase);
        if (!queryAccountInfo->exec(textQuery)) qDebug()<<"QUnAccountManager error get account info";


        if (queryAccountInfo->next())
        {
            Output.id = queryAccountInfo->value(0).toInt();
            Output.login = queryAccountInfo->value(1).toString();
            Output.password = queryAccountInfo->value(2).toString();
            Output.name = queryAccountInfo->value(3).toString();
            Output.patronymic = queryAccountInfo->value(4).toString();
            Output.surname = queryAccountInfo->value(5).toString();
            Output.email = queryAccountInfo->value(6).toString();
            Output.number = queryAccountInfo->value(7).toString();
            Output.enable = queryAccountInfo->value(8).toBool();
            Output.visible = queryAccountInfo->value(9).toBool();
            Output.rights = queryAccountInfo->value(10).toInt();
            Output.regdate = QDateTime::fromString(queryAccountInfo->value(11).toString(),"yyyy-MM-dd hh:mm:ss");
            Output.activate = queryAccountInfo->value(12).toBool();
            Output.changepassword = queryAccountInfo->value(13).toBool();
            Output.datepassword = QDateTime::fromString(queryAccountInfo->value(14).toString(),"yyyy-MM-dd hh:mm:ss");
            Output.activemail  = queryAccountInfo->value(15).toBool();
            Output.activenumber  = queryAccountInfo->value(16).toBool();
            Output.emailcode = queryAccountInfo->value(17).toInt();
            Output.numbercode = queryAccountInfo->value(18).toInt();
        }

        delete queryAccountInfo;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

QString QUnAccountManager::getPassword (int id)
{
    QString Output;

    QString textQuery = "select UNpassword from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryPassword = new QSqlQuery(*dataBase);
        if (!queryPassword->exec(textQuery)) qDebug()<<"QUnAccountManager error get user password";

        queryPassword->next();
        Output = queryPassword->value(0).toString();

        delete queryPassword;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

QString QUnAccountManager::getLogin (int id)
{
    QString Output;

    QString textQuery = "select UNlogin from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryLogin = new QSqlQuery(*dataBase);
        if (!queryLogin->exec(textQuery)) qDebug()<<"QUnAccountManager error get user login";

        queryLogin->next();
        Output = queryLogin->value(0).toString();

        delete queryLogin;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

QString QUnAccountManager::getNumber (int id)
{
    QString Output;

    QString textQuery = "select UNnumber from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryNumber = new QSqlQuery(*dataBase);
        if (!queryNumber->exec(textQuery)) qDebug()<<"QUnAccountManager error get phone number";

        queryNumber->next();
        Output = queryNumber->value(0).toString();

        delete queryNumber;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

QString QUnAccountManager::getEmail (int id)
{
    QString Output;

    QString textQuery = "select UNemail from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryEmail = new QSqlQuery(*dataBase);
        if (!queryEmail->exec(textQuery)) qDebug()<<"QUnAccountManager error get email";

        queryEmail->next();
        Output = queryEmail->value(0).toString();

        delete queryEmail;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

QString QUnAccountManager::getName (int id)
{
    QString Output;

    QString textQuery = "select UNname from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryName = new QSqlQuery(*dataBase);
        if (!queryName->exec(textQuery)) qDebug()<<"QUnAccountManager error get name";

        queryName->next();
        Output = queryName->value(0).toString();

        delete queryName;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

QString QUnAccountManager::getPatronymic (int id)
{
    QString Output;

    QString textQuery = "select UNpatronymic from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryPatronymic = new QSqlQuery(*dataBase);
        if (!queryPatronymic->exec(textQuery)) qDebug()<<"QUnAccountManager error get patronymic";

        queryPatronymic->next();
        Output = queryPatronymic->value(0).toString();

        delete queryPatronymic;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

QString QUnAccountManager::getSurname (int id)
{
    QString Output;

    QString textQuery = "select UNsurname from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * querySurname = new QSqlQuery(*dataBase);
        if (!querySurname->exec(textQuery)) qDebug()<<"QUnAccountManager error get surname";

        querySurname->next();
        Output = querySurname->value(0).toString();

        delete querySurname;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

QString QUnAccountManager::getFullName (int id)
{    QString Output;

     QString textQuery = "select UNsurname,UNname,UNpatronymic from UNusers where id_UN = '"+QString::number(id)+"';";

     if (dataBase->open())
     {
         QSqlQuery * queryFullName = new QSqlQuery(*dataBase);
         if (!queryFullName->exec(textQuery)) qDebug()<<"QUnAccountManager error get full user name";

         queryFullName->next();
         Output = queryFullName->value(0).toString();
         Output = Output +" "+ queryFullName->value(1).toString();
         Output = Output +" "+ queryFullName->value(2).toString();

         delete queryFullName;

         dataBase->close();

     } else qDebug()<<"QUnAccountManager Error open database";

     return Output;

}

int QUnAccountManager::getEmailCode (int id)
{
    int Output = 0;

    QString textQuery = "select UNemailcode from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryEmailCode = new QSqlQuery(*dataBase);
        if (!queryEmailCode->exec(textQuery)) qDebug()<<"QUnAccountManager error get email code";

        queryEmailCode->next();
        Output = queryEmailCode->value(0).toInt();

        delete queryEmailCode;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

int QUnAccountManager::getNumberCode (int id)
{
    int Output = 0;

    QString textQuery = "select UNnumbercode from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryNumberCode = new QSqlQuery(*dataBase);
        if (!queryNumberCode->exec(textQuery)) qDebug()<<"QUnAccountManager error get number code";

        queryNumberCode->next();
        Output = queryNumberCode->value(0).toInt();

        delete queryNumberCode;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

int QUnAccountManager::getRights (int id)
{
    int Output = 0;

    QString textQuery = "select UNrights from UNusers where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryRights = new QSqlQuery(*dataBase);
        if (!queryRights->exec(textQuery)) qDebug()<<"QUnAccountManager error get rights";

        queryRights->next();
        Output = queryRights->value(0).toInt();

        delete queryRights;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

int QUnAccountManager::getCountAccounts ()
{
    int Output;

    QString textQuery = "select id_UN from UNusers;";

    if (dataBase->open())
    {
        QSqlQuery * queryCount = new QSqlQuery(*dataBase);
        if (!queryCount->exec(textQuery)) qDebug()<<"QUnAccountManager error get count";

        UNAccountInfo A;

        while (queryCount->next()) Output ++;

        delete queryCount;

        dataBase->close();
    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

int QUnAccountManager::generateEmailCode (int id)
{
    int Output = qrand() % 99999;

    setEmailCode(id,Output);
    setActivateEmail(id,false);

    return Output;
}

int QUnAccountManager::generateNumberCode (int id)
{
    int Output = qrand() % 99999;

    setnNumberCode(id,Output);
    setActivateNumber(id,false);

    return Output;
}

bool QUnAccountManager::setLogin (int id, QString login)
{
    bool Output = false;

    QString textQuery = "update UNlogin set "
                        "UNlogin = '"+login+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateLogin = new QSqlQuery(*dataBase);
        if (!queryUpdateLogin->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update login";
        }
        else Output = true;

        delete queryUpdateLogin;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setName (int id, QString name)
{
    bool Output = false;

    QString textQuery = "update UNname set "
                        "UNlogin = '"+name+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateName = new QSqlQuery(*dataBase);
        if (!queryUpdateName->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update name";
        }
        else Output = true;

        delete queryUpdateName;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setPatronymic (int id,QString patronymic)
{
    bool Output = false;

    QString textQuery = "update UNpatronymic set "
                        "UNlogin = '"+patronymic+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdatePatronymic = new QSqlQuery(*dataBase);
        if (!queryUpdatePatronymic->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update patronymic";
        }
        else Output = true;

        delete queryUpdatePatronymic;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setSurname (int id, QString surname)
{
    bool Output = false;

    QString textQuery = "update UNsurname set "
                        "UNlogin = '"+surname+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateSurname = new QSqlQuery(*dataBase);
        if (!queryUpdateSurname->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update surname";
        }
        else Output = true;

        delete queryUpdateSurname;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setEmail (int id,QString email)
{
    bool Output = false;

    QString textQuery = "update UNemail set "
                        "UNlogin = '"+email+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateEmail = new QSqlQuery(*dataBase);
        if (!queryUpdateEmail->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update email";
        }
        else Output = true;

        delete queryUpdateEmail;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setNumber (int id,QString number)
{
    bool Output = false;

    QString textQuery = "update UNnumber set "
                        "UNlogin = '"+number+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateNumber = new QSqlQuery(*dataBase);
        if (!queryUpdateNumber->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update number";
        }
        else Output = true;

        delete queryUpdateNumber;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setActiate (int id,bool activate)
{
    bool Output = false;

    QString textQuery = "update UNactivate set "
                        "UNlogin = '"+QString::number((int)activate)+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateActivate = new QSqlQuery(*dataBase);
        if (!queryUpdateActivate->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update activate";
        }
        else Output = true;

        delete queryUpdateActivate;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setActivateEmail (int id,bool activate)
{
    bool Output = false;

    QString textQuery = "update UNactivemail set "
                        "UNlogin = '"+QString::number((int)activate)+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateActivateWmail = new QSqlQuery(*dataBase);
        if (!queryUpdateActivateWmail->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update activate email";
        }
        else Output = true;

        delete queryUpdateActivateWmail;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setActivateNumber (int id,bool activate)
{
    bool Output = false;

    QString textQuery = "update UNactivenumber set "
                        "UNlogin = '"+QString::number((int)activate)+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateActivateNumber = new QSqlQuery(*dataBase);
        if (!queryUpdateActivateNumber->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update activate number";
        }
        else Output = true;

        delete queryUpdateActivateNumber;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setEmailCode (int id,int code)
{
    bool Output = false;

    QString textQuery = "update UNemailcode set "
                        "UNlogin = '"+QString::number(code)+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateEmailCode = new QSqlQuery(*dataBase);
        if (!queryUpdateEmailCode->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update email code";
        }
        else Output = true;

        delete queryUpdateEmailCode;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setnNumberCode (int id,int code)
{
    bool Output = false;

    QString textQuery = "update UNnumbercode set "
                        "UNlogin = '"+QString::number(code)+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateNumberCode = new QSqlQuery(*dataBase);
        if (!queryUpdateNumberCode->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update number code";
        }
        else Output = true;

        delete queryUpdateNumberCode;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::setRights (int id,int rights)
{
    bool Output = false;

    QString textQuery = "update UNrights set "
                        "UNlogin = '"+QString::number(rights)+"', "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery *queryUpdateRights = new QSqlQuery(*dataBase);
        if (!queryUpdateRights->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update rights";
        }
        else Output = true;

        delete queryUpdateRights;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;
}

bool QUnAccountManager::updateAccount (int id, UNAccountInfo accountInfo)
{
    bool Output = false;
    QString currentPassword = getPassword(id);

    if (currentPassword != accountInfo.password)
        accountInfo.datepassword = QDateTime::currentDateTime();

    QString textQuery = "update UNusers set "
                        "UNlogin = '"+accountInfo.login+"', "
                        "UNpassword = '"+accountInfo.password+"', "
                        "UNname = '"+accountInfo.name+"', "
                        "UNpatronymic = '"+accountInfo.patronymic+"', "
                        "UNsurname = '"+accountInfo.surname+"', "
                        "UNemail = '"+accountInfo.email+"', "
                        "UNnumber = '"+accountInfo.number+"', "
                        "UNenable = '"+QString::number(accountInfo.enable)+"', "
                        "UNvisible = '"+QString::number(accountInfo.visible)+"', "
                        "UNrights = '"+QString::number(accountInfo.rights)+"', "
                        "UNactivate = '"+QString::number(accountInfo.activate)+"', "
                        "UNchangepassword = '"+QString::number(accountInfo.changepassword)+"', "
                        "UNdatepassword = '"+accountInfo.datepassword.toString("yyyy-MM-dd hh:mm:ss")+"', "
                        "UNactivemail = '"+QString::number(accountInfo.activemail)+"', "
                        "UNactivenumber = '"+QString::number(accountInfo.activenumber)+"', "
                        "UNemailcode = '"+QString::number(accountInfo.emailcode)+"', "
                        "UNnumbercode = '"+QString::number(accountInfo.emailcode)+"' "
                        "where id_UN = '"+QString::number(id)+"';";

    if (dataBase->open())
    {
        QSqlQuery * queryUpdateAccount = new QSqlQuery(*dataBase);
        if (!queryUpdateAccount->exec(textQuery))
        {
            qDebug()<<"QUnAccountManager error update account information: "
                   <<queryUpdateAccount->lastError().text();
        }
        else Output = true;

        delete queryUpdateAccount;

        dataBase->close();

    } else qDebug()<<"QUnAccountManager Error open database";

    return Output;

}

QByteArray UNAccountInfo::toByteArray()
{
    QByteArray Output;

    quint8 *ptr;
    Output.push_back((quint8)sizeof(id));
    ptr = (quint8*)(&id);
    for (quint8 i=0;i<sizeof(id);i++)
        Output.push_back(*(ptr+i));

    Output.push_back((quint8)login.toUtf8().length());
    Output.push_back(login.toUtf8());

    Output.push_back((quint8)password.toUtf8().length());
    Output.push_back(password.toUtf8());

    Output.push_back((quint8)name.toUtf8().length());
    Output.push_back(name.toUtf8());

    Output.push_back((quint8)patronymic.toUtf8().length());
    Output.push_back(patronymic.toUtf8());

    Output.push_back((quint8)surname.toUtf8().length());
    Output.push_back(surname.toUtf8());

    Output.push_back((quint8)email.toUtf8().length());
    Output.push_back(email.toUtf8());

    Output.push_back((quint8)number.toUtf8().length());
    Output.push_back(number.toUtf8());

    Output.push_back(enable);
    Output.push_back(visible);
    Output.push_back(rights);

    QString textDate = regdate.toString("yyyy-MM-dd hh:mm:ss");
    Output.push_back((quint8)textDate.length());
    Output.push_back(textDate.toUtf8());

    Output.push_back(activate);
    Output.push_back(changepassword);

    textDate = datepassword.toString("yyyy-MM-dd hh:mm:ss");
    Output.push_back((quint8)textDate.length());
    Output.push_back(textDate.toUtf8());

    Output.push_back(activemail);
    Output.push_back(activenumber);

    Output.push_back((quint8)sizeof(emailcode));
    ptr = (quint8*)(&emailcode);
    for (quint8 i=0;i<sizeof(emailcode);i++)
        Output.push_back(*(ptr+i));

    Output.push_back((quint8)sizeof(numbercode));
    ptr = (quint8*)(&numbercode);
    for (quint8 i=0;i<sizeof(numbercode);i++)
        Output.push_back(*(ptr+i));

    return Output;
}

void UNAccountInfo::fromByteArray(QByteArray data)
{
    quint8 *ptr = (quint8*)(&id);
    int size = data.at(0);
    QByteArray buf = data.mid(1,size);
    data.remove(0,size + 1);
    for (int i=0;i<size;i++)
        *(ptr+i) = buf.at(i);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    login = QString(buf);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    password = QString(buf);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    //qDebug()<<"Name size="<<QString::number(size);
    name = QString(buf);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    patronymic = QString(buf);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    surname = QString(buf);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    email = QString(buf);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    number = QString(buf);

    enable = data.at(0);
    data.remove(0,1);

    visible = data.at(0);
    data.remove(0,1);

    rights = data.at(0);
    data.remove(0,1);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    regdate = QDateTime::fromString(QString(buf),"yyyy-MM-dd hh:mm:ss");

    activate = data.at(0);
    data.remove(0,1);

    changepassword = data.at(0);
    data.remove(0,1);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    datepassword = QDateTime::fromString(QString(buf),"yyyy-MM-dd hh:mm:ss");

    activemail = data.at(0);
    data.remove(0,1);

    activenumber = data.at(0);
    data.remove(0,1);

    ptr = (quint8*)(&emailcode);
    size = data.at(0);
    //qDebug()<<"Email code size="<<QString::number(size);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    for (int i=0;i<size;i++)
        *(ptr+i) = buf.at(i);

    ptr = (quint8*)(&numbercode);
    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    for (int i=0;i<size;i++)
        *(ptr+i) = buf.at(i);
}

QString UNAccountInfo::toString(bool *ok)
{
    QString Output;
    Output = Output + "id: " + QString::number(id) + " ";
    Output = Output + "login: " + login + " ";
    Output = Output + "password: " + password + " ";
    Output = Output + "name: " + name + " ";
    Output = Output + "patronymic: " + patronymic + " ";
    Output = Output + "surname: " + surname + " ";
    Output = Output + "email: " + email + " ";
    Output = Output + "number: " + number + " ";

    if (enable) Output = Output + "enable: true" + " ";
        else Output = Output + "enable: false" + " ";

    if (visible) Output = Output + "visible: true" + " ";
        else Output = Output + "visible: false" + " ";

    Output = Output + "rights: " + QString::number(rights) + " ";
    Output = Output + "regdate: " + regdate.toString("yyyy-MM-dd hh:mm:ss") + " ";

    if (activate) Output = Output + "activate: true" + " ";
        else Output = Output + "activate: false" + " ";

    if (changepassword) Output = Output + "changepassword: true" + " ";
        else Output = Output + "changepassword: false" + " ";

    Output = Output + "datepassword: " + datepassword.toString("yyyy-MM-dd hh:mm:ss") + " ";

    if (activemail) Output = Output + "activemail: true" + " ";
        else Output = Output + "activemail: false" + " ";

    if (activenumber) Output = Output + "activenumber: true" + " ";
        else Output = Output + "activenumber: false" + " ";

    Output = Output + "emailcode: " + QString::number(emailcode) +  " ";
    Output = Output + "numbercode: " + QString::number(numbercode) + " ";


    return Output;
}
