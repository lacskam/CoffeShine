#ifndef COMMONDBTABTYPES_H
#define COMMONDBTABTYPES_H

#include <QString>
#include <QMetaType>

// описание itema в классе QMarkWidgetItem
struct WidgetToMarkItemInfo
{
    int itemId;              // id элемента
    QString itemName;       // имя элемента
    bool isItemMarked = false;     // состояние элемента
    bool itemWasChanged;  // определение был ли включён или исключён элемет
};

struct CategoryForComboBoxInfo
{
    int idCategory;
    QString nameCaregory;
    bool isItemMarked;     // состояние элемента
};
Q_DECLARE_METATYPE(CategoryForComboBoxInfo);

struct VolumeForComboBoxInfo
{
    int idVolume;
    QString nameVolume;
};
Q_DECLARE_METATYPE(VolumeForComboBoxInfo);

// описание itema в классе QPriceWidgetItem
struct PriceAndVolumeInfo
{     
    int volumeId = -1;        // id объёма, -1 если создаётся новый объём
    int volume;              // объём элемента
    QString units;          // единицы измерения объёма
    int priceId;
    double price;          // цена в элементе
    bool itemWasChanged;  // определение был ли включён или исключён элемет
};

#endif // COMMONDBTABTYPES_H
