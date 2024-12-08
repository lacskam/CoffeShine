#ifndef QSENTOLANE_H
#define QSENTOLANE_H
#include <QObject>
#include <QIODevice>
#include <QTextCodec>
#include <QThread>
#include <QVector>
#include <QTimer>

#include <QDebug>

class QSentoLanE  : public QObject
{
    Q_OBJECT
    Q_ENUMS(cotrolByte)
    Q_ENUMS(errorCode)
    Q_ENUMS(typeReport)
    Q_ENUMS(productProcessType)
    Q_ENUMS(vatType)
    Q_ENUMS(opRigths)
    Q_ENUMS(transmissionType)
    Q_ENUMS(discountSurcharge)
    Q_ENUMS(rightsBitsMask)

public:
    QSentoLanE(QIODevice *device,QObject *parent = NULL);
    ~QSentoLanE();



    enum cotrolByte
    {
        cbyteENQ = 0x05, //Инициализация обмена данными (транзакции)
        cbyteACK = 0x06, //подтверждение приема
        cbyteNAK = 0x15, //ответ КСА о том что отправленный запрос некорректен
        cbyteSTX = 0x02, //стартовый байт пакета данных
        cbyteETX = 0x03, //стоповый байт пакета данных
        cbyteETB = 0x17, //байт отправляется КСА в порт при включении для передачи информации оборудованию что КСА включился.
        cbyteEOT = 0x04, //байт окончание обмена данными (транзакции).
        cbyteWACK = 0x09, //КСА занят и ведет обработку данных.
        cbyteRVI = 0x40 //КСА обнаружил ошибку, прерывает обмен данными
    };
    enum errorCode
    {
        errorTypeTask=0x01, //Тип задания не соответствует типу стартового пакета данных
        errorTXData=0x02, //Ошибка передачи данных
        errorSFValue=0x03, //Ошибка начального или конечного значения
        errorValueNotNumber=0x04, //Введенный символ (данные) а полях ЦЕНА (price), КОЛЛИЧЕСТВО (quantity), или СТОИМОСТЬ (value) не цифра
        errorIncChar=0x05, //Запрещенный символ в наименовании товара
        errorInOperMode=0x06, //КСА в эксплуатационном режиме
        errorValueRange=0x07, //Значение вне диапазона. Документ не открыт или нет данных о товаре.
        errorCodeNotFound=0x08, //Код товара не найден
        errorVATValue=0x09, //Некорректное значение ставки НДС
        errorCharAcc=0x0A, //Неправильный символ, описывающий точность или флаг
        errorTableIndex=0x0B, //Ошибка таблицы индекса сумм ограничений HALO
        errorDateTime=0x0C, //Некорректное значение даты \ времени
        errorTypeTaskIO=0x0D, //Ошибка типа задания либо ошибка передачи данных в текущем режиме КСА
        errorInternal=0x0E, //Внутренняя ошибка КСА
        errorProdCodeEx=0x0F, //Товар с таким кодом уже запрограммирован, невозможно изменение товара без гашения товарооборота.
        errorLimitVAT=0x10, //Превышен лимит количества изменений ставок НДС
        errorIncMode=0x11, //Операция недоступна в текущем режиме работы КСА
        errorIncTypeTask=0x13, //Некорректный тип задания.
        errorBlockSale=0x14, //Блокировка продажи. Переполнение значения поля: ЦЕНА, КОЛ-ВО или СУММА
        errorTooLarge=0x15, //Слишком большое значение поля: ЦЕНА (price), КОЛЛИЧЕСТВО (quantity), СТОИМОСТЬ (value)
        errorWritingBEP=0x16, //Ошибка записи в БЭП
        errorPaper=0x17, //Ошибка бумаги
        errorOverPLU=0x18, //Переполнение базы PLU
        errorNonzeroValue=0x19, //Цена запрограммирована (фиксирована) но было введено не нулевое значение цены;
        errorOverVoucher=0x1A, //Переполнение счетчика позиций чека
        errorNonCount=0x1B, //В товарной позиции отсутствует количество товара
        errorOverTotal=0x1C, //При регистрации продажи: переполнение счетчика ИТОГО по документу
        errorDocBuff=0x1D, //Ошибка буфера документа;
        errorDocNotOpen=0x1E, //Документ не открыт или не имеет товарных позиций
        errorIncDisc=0x1F, //Некорректное значение скидки \ надбавки
        errorPackGoods=0x20, //Товар - упаковка (не поддерживается в версиях ПО без поддержки упаковки)
        errorDBUstruct=0x21, //Ошибка структуры БД PLU
        errorHardMem=0x22, //Аппаратная ошибка памяти КСА
        errorManyRelatedProd=0x23, //Слишком много связанных товаров
        errorIncRTC=0x25, //Некорректные настройки RTC (внутреннего таймера);
        errorRAM=0x26, //Ошибка ОЗУ
        errorOverPackKSA=0x27, //Переполнение упаковки КСА
        errorDoubVAT=0x28, //В КСА запрограммированы идентичные ставки НДС
        errorUserRight=0x29, //Не определены права пользователя
        errorNumbPrint=0x2A, //Номер печати меньше, чем наименьшее или больше наибольшего числа в буфере печати
        errorBuffEmp=0x2B, //Буфер online пуст
        errorManOpen=0x2C, //Документ был открыт оператором вручную (с клавиатуры КСА);
        errorNotRegUser=0x2D, //Нет отдела (секции); Требуется регистрация пользователя.
        errorCancVAT=0x2E, //Отмена (СТОРНИРОВАНИЕ) операции позволит снизить стоимость продаж в ставке НДС ниже нуля;
        errorMaxCost=0x2F, //Превышено максимальное значение цены; Переполнение буфера
        errorTextAlrSend=0x30, //Текст уже отправлен;
        errorProdNotFound=0x31, //Товар не найден
        errorIncProdCode=0x32, //Некорректный код связанного товара
        errorPorts=0x33, //Некорректная настройка портов связи
        errorDayNotOpe=0x43, //День не открыт, невозможно выполнить операцию. (Требуется открытие дня)
        errorOpAlrOpen=0x44, //Операция открытия дня уже была выполнена
        errorInit=0x50, //Ошибка инициализации
        errorRAM3=0x51, //Ошибка ОЗУ
        errorCheckAlrOpen=0x52, //Чек уже открыт (или открыт для другой операции)
        errorUPR=0x53, //Ошибка УНП
        errorDisc=0x54, //Ошибка скидки\надбавки
        errorData=0x55, //Ошибка данных
        errorNefarMode=0x56, //Нефискальный режим
        errorNoProgData=0x57, //Отсутствуют программируемые данные
        errorBEPEnd=0x58, //БЭП - окончание
        errorRAM1=0x59, //Ошибка ОЗУ
        errorProdDB=0x5A, //Ошибка базы товаров
        errorCliche=0x5B, //Ошибка клише КСА
        errorData1=0x5C, //Ошибка данных
        errorState=0x5D, //Ошибка состояния КСА
        errorUpData=0x5E, //Ошибка актуализации данных
        errorMisUnNumb=0x5F, //Отсутствует уникальный номер
        errorBEPMis=0x60, //БЭП отсутствует
        errorPrinter=0x61, //Ошибка принтера КСА
        errorIndKCA=0x62, //Ошибка индикатора КСА
        errorBEPChan=0x63, //БЭП изменен!
        errorVATAlrProg=0x64, //Ставки НДС уже запрограммированы
        errorOnlyRead=0x65, //Режим КСА – только чтение
        errorUnkCom=0x66, //Неизвестная команда
        errorData2=0x67, //Ошибка данных
        errorRepDataRange=0x68, //Ошибка диапазона данных отчета
        errorDataNotFound=0x69, //Данные отсутствуют
        errorComBufOver=0x6A, //Переполнение буфера связи
        errorFiscalRegImeNotCom=0x6B, //Фискальный режим КСА не завершен
        errorData3=0x6C, //Ошибка данных
        errorOverBEP=0x6D, //Переполнение БЭП
        errorBEPDataCard=0x6E, //Ошибка карты данных БЭП
        errorComDis=0x6F, //Команда запрещена в данном режиме КСА.
        errorTXData1=0x70, //Ошибка передачи данных
        errorBusBEP=0x71, //Ошибка шины данных БЭП
        errorNoData=0x72, //Данные отсутствуют
        errorDataTAXI=0x73, //Данные тарифов ТАКСИ отсутствуют
        errorIncoCombTax=0x74, //Некорректное сочетание налоговых ставок
        errorNoPermissions=0x75, //Нет прав для данной функции
        errorProdNotFound1=0x76, //Товары не найдены
        errorEURRateNotSet=0x77, //Курс EURO не устновлен
        errorFixedCost=0x78, //Товар имеет фиксированную цену
        errorMuchDiscount=0x79, //Слишком большая скидка
        errorMuchSurcharge=0x7A, //Слишком большая надбавка
        errorLargeAmountDoc=0x7B, //Слишком большая сумма документа
        errorMuchCash=0x7C, //Слишком большое значение наличных в КСА
        errorRequiresSubtotal=0x7D, //Требуется ПИТОГ
        errorPaymenRequired=0x7E, //Требуется оплата
        errorManDiscountNotAllowed=0x7F, //Ручная скидка запрещена
        errorDiscountListForbidden=0x80, //Скидка из списка запрещена
        errorDiscountProhibited=0x81, //Скидка запрещена
        errorBatteryDischarges=0x82, //Идет разряд аккумулятора
        errorAnotherDocumentOpen=0x83, //Операция запрещена. Открыт другой документ.
        errorOverflowBufferOnline=0x84, //Переполнение буфера online
        errorBufferOnline=0x85, //Ошибка буфера online
        errorInvDocumentRange=0x86, //Недопустимый диапазон документов
        errorNoData1=0x87, //Нет данных
        errorSubtotalNotPressed=0x88, //Не нажат ПИТОГ
        errorDiscountCard=0x89, //Ошибка определения дисконтной карты
        errorDiscountCardEntered=0x8A, //Дисконтная карта уже введена
        errorDiscountCardNumbe=0x8B, //Ошибка номера дисконтной карты
        errorExpiredDiscountCard=0x8C, //Истек срок действия дисконтной карты
        errorDiscountProgrammed=0x8D, //Скидка не запрограммирована
        errorDiscountCardDate=0x8E, //Ошибка даты дисконтной карты
        errordDocumentNotOpen=0x8F, //Документ не открыт
        errorTransactionService=0x90, //Ошибка обслуживания транзакции OFPTX
        errorExceptionHandling=0x91, //Ошибка обслуживания исключения OESEX
        errorVATRateValue=0x92, //Ошибка значения ставки НДС
        errorProgKSAOnlyPC=0x93, //Программирование КСА только через ПЭВМ
        errorRepCancellOnlyPC=0x94, //Гашение отчета только через ПЭВМ
        errorOnlyGoodsChanReport=0x95, //Только для отчета об изменении товаров
        errorRepChanGoodsDuringExecution=0x96, //Отчет об изменениях товаров в ходе выполнения
        errorCommPCBlocked=0x97, //Связь с ПЭВМ заблокирована
        errorAccessModemBlocked=0x98, //Доступ к модему заблокирован
        errorNoCashBox=0x99, //Нет наличных в денежном ящике для выплаты
        errorDiscCardRequired=0x9A, //Требуется дисконтная карта
        errorOnlyAfterRepChan=0x9B, //Только после отчета об изменениях тов.
        errorDataExchangePC=0x9C, //Идет обмен данными С ПЭВМ
        errorOverNumbSales=0x9D, //Переполнение числа продаж товаров
        errorData4=0x9E, //Ошибка данных
        errorData5=0x9F, //Ошибка данных
        errorDocDB=0xA0, //Ошибка базы документов
        errorProdDB1=0xA1, //Ошибка базы товаров
        errorDBRelatProd=0xA2, //Ошибка базы связанных товаров
        errorRAM2=0xA3, //Ошибка ОЗУ
        errorCashWithdOnly=0xA4, //Выплата только наличными
        errorNoItemInDoc=0xA5, //Нет товаров в документе
        errorMenuStruct=0xA6, //Ошибка структуры меню
        errorChanAccuracyDis=0xA7, //Изменение точности запрещено
        errorPaymTypeDef=0xB0, //Вид оплаты не определен
        errorNeedProdCode=0xB1, //Требуется ввод кода товара
        errorChanOpDis=0xB2, //Изменение опции запрещено
        errorNumbPayTypeExceededf=0xB3, //Превышено максимальное число видов оплат
        errorTimerNotProg=0xB7, //Таймер КСА не запрограммирован
        errorTransfer=0xB8, //Ошибка передачи
        errorNumbParam=0xB9, //Ошибка количества параметров
        errorParam=0xBA, //Ошибка параметра
        errorTimerRTC=0xBB, //Ошибка таймера RTC
        errorBEP=0xBC, //Ошибка БЭП
        errorDate=0xBD, //Ошибка даты
        errorSalMustZero=0xBE, //Продажи должны быть равны нулю
        errorIO=0xBF, //Ошибка операции I/O
        errorChanDateDis=0xC0, //Изменение даты времени запрещено
        errorNumVAT=0xC1, //Ошибка количества ставок НДС
        errorHeader=0xC2, //Ошибка заголовка
        errorReregDis=0xC3, //Перерегистрация запрещена
        errorChanTitleDis=0xC4, //Смена заголовка запрещена
        errorDocText=0xC5, //Ошибка текста документа
        errorItemName=0xC6, //Ошибка наименования товара
        errorGoodsQuantity=0xC7, //Ошибка количества товара
        errorProdVAT=0xC8, //Ошибка ставки НДС товара
        errorProdPrice=0xC9, //Ошибка цены товара
        errorGoodsAmount=0xCA, //Ошибка суммы товара
        errorTransModeDis=0xCB, //Режим транзакции отключен
        errorOperSTORNO=0xCC, //Ошибка операции СТОРНО
        errorOverNumbItemDoc=0xCD, //Переполнение количества товарных позиций документа
        errorPrintBuffOver=0xCE, //Переполнение буфера печати
        errorData6=0xCF, //Ошибка данных
        errorSumPAYMENT=0xD0, //Ошибка суммы ОПЛАТА
        errorValSumm=0xD1, //Ошибка в значениях СУММА ОПЛАТЫ или СКИДКА
        errorSummCountOver=0xD2, //Переполнение суммирующего счетчика
        errorDocComOper=0xD3, //Нет документа для завершения операции
        errorSumPAY=0xD4, //Ошибка суммы ОПЛАТА
        errorCounOverCASH=0xD5, //Переполнение счетчика НАЛИЧНЫЕ
        errorNegCashResStatus=0xD6, //Отрицательное состояние сброса наличных
        errorCashierDescText=0xD7, //Ошибка текста описания кассира
        errorAmountDesc=0xD8, //Ошибка в сумме или описании (тексте)
        errorSaleResetZero=0xD9, //Продажа обнулена
        errorWritingCurrentDate=0xDA, //Ошибка записи с текущей датой
        errorOperCancel=0xDB, //Операция отменена
        errorName=0xDC, //Ошибка имени
        errorSymbVAT=0xDD, //Ошибка символа ставки НДС
        errorDocHead=0xDE, //Ошибка заголовка документа
        errorTicketNumb=0xDF, //Ошибка номера кассы
        errorCashierNumb=0xE0, //Ошибка номера кассира
        errorDocNumb=0xE1, //Ошибка номера документа
        errorFieldCONTRACTOR=0xE2, //Ошибка поля КОНТРАГЕНТ
        errorFieldTERMINAL=0xE3, //Ошибка поля ТЕРМИНАЛ
        errorFieldCardName=0xE4, //Ошибка поля ИМЯ КАРТЫ
        errorFieldCardNumb=0xE5, //Ошибка поля НОМЕР КАРТЫ
        errorFieldDateMonth=0xE6, //Ошибка поля ДАТА МЕСЯЦ
        errorFieldsDateYear=0xE7, //Ошибка поля ДАТА ГОД
        errorFieldAuthCode=0xE8, //Ошибка поля КОД АВТОРИЗАЦИИ
        errorFieldSUM=0xE9, //Ошибка поля СУММА
        errorPrinterBusy=0xF0, //Принтер КСА занят
        errorUnfinishedShift=0xF1, //В КСА есть незавершенная смена!
        errorFTPActive=0xF2, //Менеджер FTP активен
        errorProgOnlyFtp=0xF3, //Программирование только через FTP
        errorConnFtp=0xF4, //Ошибка связи с FTP сервером
        errorRegFTP=0xF5, //Ошибка регистрации на FTP сервере
        errorIncFTPServ=0xF6, //Некорректный сервер FTP
        errorConfFTP=0xF7, //Конфигурация FTP некорректна
        errorAccessFTPFile=0xF8, //Сутсвует доступ к файлу через FTP
        errorModemNotCon=0xF9, //Модем подключен!
        errorOverOnlineBuf=0xFA, //Переполнение буфера online
        errorOverPP1=0xFB, //PP1 очередь передачи переполнена
        errorOverPP2=0xFD, //PP2 очередь передачи переполнена
        errorOverPP3=0xFE, //PP3 очередь передачи переполнена
        errorOperCancelKCA=0xFF //Операция отменена через КСА
    };
    enum typeReport
    {
        typeRZConfirm=0x00, //Дневной Z-отчет с запросом подтверждения на выполнение оператором КСА
        typeRZNoConfirm=0x01, //Дневной Z-отчет без запросом подтверждения на выполнение оператором КСА
        typeRXNoConfirm=0x02 //Сменный X- отчет
    };
    enum productProcessType
    {
        ptypeReg = 0x05,    //Регистрация товарной позиции
        ptypeReverse = 0x02 //Сторнирование (корректирование) товарной позиции
    };
    enum vatType
    {
        vatA = 'A',
        vatB = 'B',
        vatC = 'C',
        vatD = 'D',
        vatE = 'E',
        vatF = 'F',
        vatZ = 'Z'
    };
    enum opRigths
    {
        rightCashier = 0x00,            //Кассир
        rightSeniorCashier = 0x01,      //Старший кассир
        rightStoreAdministrator = 0x02, //Администратор магазина
        rightCurrent1 = 0x03,
        rightCurrent2 = 0x04,
        rightCurrent3 = 0x05,
        rightCurrent4 = 0x06,
        rightAdministrator = 0x07       //Администратор
    };
    enum transmissionType
    {
        ttypeCMtoPC = 0x00,     //КСА передает данные на ПЭВМ
        ttypePCtoCM = 0x01,     //КСА принимает данные от ПЭВМ
        ttypePCtoCMClear = 0x02 //прием данных отчетов с обнулением
    };
    enum discountSurcharge
    {
        discountD5 = '5',  //относительная скидка на позицию товара
        discountD6 = '6',  //относительная скидка на ПРЕДИТОГ
        discountD7 = '7',  //абсолютная скидка на позицию товара
        discountD8 = '8',  //абсолютная скидка на ПРЕДИТОГ

        surchargeD9 = '9', //относительная надбавка на позицию товара
        surchargeDA = 'A', //относительная надбавка на ПРЕДИТОГ
        surchargeDB = 'B', //абсолютная надбавка на позицию товара
        surchargeDC = 'C'  //абсолютная надбавка на ПРЕДИТОГ
    };
    enum rightsBitsMask
    {
        maskSale                = 0b0000000000000000000000000000000000000000000000000000000000000001,
        maskCancelPayDocs       = 0b0000000000000000000000000000000000000000000000000000000000000010,
        maskServAttach          = 0b0000000000000000000000000000000000000000000000000000000000000100,
        maskDiscFromDB          = 0b0000000000000000000000000000000000000000000000000000000000001000,
        maskDiscManual          = 0b0000000000000000000000000000000000000000000000000000000000010000,
        maskSetReports          = 0b0000000000000000000000000000000000000000000000000000000000100000,
        maskSetZReports         = 0b0000000000000000000000000000000000000000000000000000000001000000,
        maskRepZReport          = 0b0000000000000000000000000000000000000000000000000000000010000000,
        maskRepXReport          = 0b0000000000000000000000000000000000000000000000000000000100000000,
        maskRepBEP              = 0b0000000000000000000000000000000000000000000000000000001000000000,
        maskKepFundsKSA         = 0b0000000000000000000000000000000000000000000000000000010000000000,
        maskKepZFundsKSA        = 0b0000000000000000000000000000000000000000000000000000100000000000,
        maskRepAmend            = 0b0000000000000000000000000000000000000000000000000001000000000000,
        maskRepCashier          = 0b0000000000000000000000000000000000000000000000000010000000000000,
        maskRepZCashier         = 0b0000000000000000000000000000000000000000000000000100000000000000,
        maskRepSales            = 0b0000000000000000000000000000000000000000000000001000000000000000,
        maskRepZSales           = 0b0000000000000000000000000000000000000000000000010000000000000000,
        maskBT                  = 0b0000000000000000000000000000000000000000000000100000000000000000,
        maskChangePrice         = 0b0000000000000000000000000000000000000000000001000000000000000000,
        maskDiscountDB          = 0b0000000000000000000000000000000000000000000010000000000000000000,
        maskCashiers            = 0b0000000000000000000000000000000000000000000100000000000000000000,
        maskPrintDB             = 0b0000000000000000000000000000000000000000001000000000000000000000,
        maskProgClock           = 0b0000000000000000000000000000000000000000010000000000000000000000,
        maskCurrencyProgII      = 0b0000000000000000000000000000000000000000100000000000000000000000,
        maskProgOption          = 0b0000000000000000000000000000000000000001000000000000000000000000,
        maskProgOtherOptions    = 0b0000000000000000000000000000000000000010000000000000000000000000,
        maskPCDataExch          = 0b0000000000000000000000000000000000000100000000000000000000000000,
        maskModemDataExch       = 0b0000000000000000000000000000000000001000000000000000000000000000,
        maskEquipDataExch       = 0b0000000000000000000000000000000000010000000000000000000000000000,
        maskFullMenu            = 0b0000000000000000000000000000000000100000000000000000000000000000,
        maskDefDisc             = 0b0000000000000000000000000000000001000000000000000000000000000000,
        maskServExtrad          = 0b0000000000000000000000000000000010000000000000000000000000000000,
        maskCorrection          = 0b0000000000000000000000000000000100000000000000000000000000000000,
        maskSalesByName         = 0b0000000000000000000000000000001000000000000000000000000000000000,
        maskListedSales         = 0b0000000000000000000000000000010000000000000000000000000000000000,
        maskOrders              = 0b0000000000000000000000000000100000000000000000000000000000000000,
        maskConfFTP             = 0b0000000000000000000000000001000000000000000000000000000000000000,
        maskFTPConnect          = 0b0000000000000000000000000010000000000000000000000000000000000000,
        maskOpenCashDrawer      = 0b0000000000000000000000000100000000000000000000000000000000000000,
        maskELFunctions         = 0b0000000000000000000000001000000000000000000000000000000000000000,
        maskRepEH               = 0b0000000000000000000000010000000000000000000000000000000000000000,
        maskEHMemCard           = 0b0000000000000000000000100000000000000000000000000000000000000000,
        maskReturn              = 0b0000000000000000000001000000000000000000000000000000000000000000,
        maskCancelPolicy        = 0b0000000000000000000010000000000000000000000000000000000000000000
    };
    struct productPosition
    {
        quint64 code = 0;
        vatType vat;
        quint16 priceWhole = 0;
        quint8 priceFract = 0;
        quint32 quantityWhole = 0;
        quint16 quantityFract = 0;
        quint32 sumWhole = 0;
        quint32 sumFract = 0;
        QString name;
    };
    struct payment
    {
        //Сумма чека
        quint32 valueWhole=0;
        quint8 valueFract=0;
        //Оплата наличными
        quint32 cashWhole=0;
        quint8 cashFract=0;
        //Оплата чеком
        quint32 checkWhole=0;
        quint8 checkFract=0;
        //Оплата картой
        quint32 cardWhole=0;
        quint8 cardFract=0;
        //Оплата купоном
        quint32 couponWhole=0;
        quint8 couponFract=0;
        //Оплата кредитом
        quint32 creditWhole=0;
        quint8 creditract=0;
    };
    struct operatorCM
    {
        quint8 number;
        QString name;
        int pasword;
        opRigths right;
    };
    struct startPacket
    {
        QChar taskType;
        transmissionType transType;
        QString startNum;
        QString endNum;
    };
    struct startExtandedPacket
    {
        QChar taskType;
        quint8 subTask;
        transmissionType transType;
        QString startNum;
        QString endNum;
    };
    struct statDocument
    {
        quint8 flagOpenDoc;
        quint32 sumWhole=0;
        quint8 sumFract=0;
        quint16 countItems;
        quint32 number;
    };
    struct discountSurchargeInfo
    {
        discountSurcharge operation;
        quint32 valueWhole=0;
        quint8 valueFract=0;
    };
    struct returnDoc
    {
        quint64 code;
        quint32 quantityWhole = 0;
        quint16 quantityFract = 0;
    };

    struct rightsBits
    {
        bool Sale = false;                  //Продажа
        bool CancelPayDocs = false;         //Отмена платежных документов
        bool ServAttach = false;            //Служеб.внесение
        bool DiscFromDB = false;            //Скидки из базы
        bool DiscManual = false;            //Скидка ручн.
        bool SetReports = false;            //Набор отчетов
        bool SetZReports = false;           //Набор Z-отчетов
        bool RepZReport = false;            //Сменный Z-отчет
        bool RepXReport = false;            //Сменный X-отчет
        bool RepBEP = false;                //Отчет по данным из БЭП
        bool KepFundsKSA = false;           //Отчет по ден.средствам в КСА
        bool KepZFundsKSA = false;          //Z-отчет по ден.средствам в КСА
        bool RepAmend = false;              //Отчет по изм.
        bool RepCashier = false;            //Отчет по кассирам
        bool RepZCashier = false;           //Z-отчет по кассирам
        bool RepSales = false;              //Отчет по обороту
        bool RepZSales = false;             //Z-отчет по обороту
        bool BT = false;                    //БТ
        bool ChangePrice = false;           //Изм.цен
        bool DiscountDB = false;            //База скидок
        bool Cashiers = false;              //Кассиры (изменить,удалить)
        bool PrintDB = false;               //Печать баз (БД КСА меню[3])
        bool ProgClock = false;             //Прг.часов (Программирование часов)
        bool CurrencyProgII = false;        //Программирование валюты II
        bool ProgOption = false;            //Прг.опций
        bool ProgOtherOptions = false;      //Программирование других опций
        bool PCDataExch = false;            //ПК. Обмен данными
        bool ModemDataExch = false;         //Модем.Обменданными
        bool EquipDataExch = false;         //Оборудование. Обмен данными
        bool FullMenu = false;              //Полное меню
        bool DefDisc = false;               //Скидки по умолчанию
        bool ServExtrad = false;            //Служебная выдача из КСА
        bool Correction = false;            //Коррекция
        bool SalesByName = false;           //Продажи по наименованию
        bool ListedSales = false;           //Продажи из списка
        bool Orders = false;                //Заказы
        bool ConfFTP = false;               //Конфигурация FTP
        bool FTPConnect = false;            //FTP-соединение
        bool OpenCashDrawer = false;        //Открытие денежного ящика
        bool ELFunctions = false;           //Функции ЭЖ
        bool RepEH = false;                 //Отчеты ЭЖ
        bool EHMemCard = false;             //ЭЖ карты памяти
        bool Return = false;                //Возврат
        bool CancelPolicy = false;          //Аннуляция
    };

    struct roleOperator
    {
        quint8 num;
        QString name;
        rightsBits right;
    };

    //Работа с операторами кассового аппарата
    bool addOperator (operatorCM user);
    bool delOperator (operatorCM user);
    bool readOperators ();

    //Стартовый пакет
    bool sendStartPacket (startPacket packetInfo);
    bool sendStartExtendedPacket (startExtandedPacket packetInfo);

    bool openingShift (); //Открытие смены
    bool depositCash (quint32 whole, quint8 fract); //Внесение наличных
    bool payCash (quint32 whole, quint8 fract); //Выплата наличных
    bool printReport(typeReport type);
    bool printNonTaxDo(QStringList *text);
    bool printNonTaxText(QString text);

    //Режим нефискального принтера
    bool addDiscountSurcharge(discountSurchargeInfo info);
    bool regProduct(productPosition product);
    bool revProduct (productPosition product);
    bool cancelDocument();
    bool closeDocument(payment pay);
    bool requestDocStat();
    bool openReturnDoc(returnDoc infoRetDoc);


    bool clearGoodsBase(); //Очистить базу товаров
    bool readSerialNumber(); //Чтение серийного номера кассового аппарата

    //Роли операторов
    bool readRolesOperators();
    bool writeRoleOperator(roleOperator role);

private:
    QIODevice *currentDevice;
    bool flagACK = false;
    QVector<QByteArray> commandLine;
    bool flagCommand = false;
    QTimer *timerACK;
    bool flagReceive = false;

    bool sendData(QByteArray data);

    short CalcCrc(char *str,int len);
    quint16 CalcCrc(QByteArray *str);
    QString fromCP1251 (QByteArray *input);
    QByteArray toCP1251 (QString *input);

    bool printLine (quint8 number, QString line);
    void eventACK();
    void eventENQ();
    void eventEOT();
    void eventRVI();

    void sendACK();
    void sendEOT();

    void commandHandler(QByteArray command);
    QByteArray inputBuffer;
    bool flagWhile = false;
    QVector<operatorCM> listOperators;
    void CommandReadOperator(QString strCommand);
    void CommandReadSerialNumber(QString strCommand);
    void commandStatDoc(QString strCommand);

    QVector<roleOperator> rolesOperators;
    void commandRolesOperators(QString strCommand);

    quint64 rightsBitsToQuint64 (rightsBits &input);
    QSentoLanE::rightsBits rightsBitsFromQuint64 (quint64 &input);

private slots:
    void slotReadyRead();
    void slotTImerACK();


signals:
    void signalTxData (QByteArray data);
    void signalRxData (QByteArray data);
    void signalOperatorsList(QVector<QSentoLanE::operatorCM>operators);
    void signalSerialNumber(QString serialNumber);
    void signalStatusDocument(QSentoLanE::statDocument status);
    void signalRolesOperators(QVector<QSentoLanE::roleOperator> roles);
};

#endif // QSENTOLANE_H
