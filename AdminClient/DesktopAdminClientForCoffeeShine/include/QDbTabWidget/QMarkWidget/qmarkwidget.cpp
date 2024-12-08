#include "qmarkwidget.h"

QMarkWidget::QMarkWidget(QWidget *parent) : QWidget(parent)
{
    createForm();
    setMinAndMaxHeight(200,200);
}

void QMarkWidget::createForm()
{
    mainLayout = new QHBoxLayout(this);
    mainGroupBox = new QGroupBox();
    mainLayout->addWidget(mainGroupBox);
    layForGroupBox = new QVBoxLayout();
    mainGroupBox->setLayout(layForGroupBox);
    layForSearchWidget = new QHBoxLayout();
    layForGroupBox->addLayout(layForSearchWidget);
    searchLine = new QLineEdit(this);
    labelForIcon = new QLabel(this);
    searchIcon = new QPixmap(":/icons/other/Search.png");
    *searchIcon = searchIcon->scaled(24,24,Qt::KeepAspectRatio);
    labelForIcon->setPixmap(*searchIcon);
    layForSearchWidget->addWidget(labelForIcon);
    layForSearchWidget->addWidget(searchLine,1);
    connect(searchLine,SIGNAL(textEdited(QString)),this,SLOT(slotSearchTextChanged(QString)));
    listWidget = new QListWidget();
    layForGroupBox->addWidget(listWidget,1);

    layForButtons = new QHBoxLayout();
    layForGroupBox->addLayout(layForButtons);
    selectAllButton = new QPushButton("Отметить всё", this);
    unselectAllButton = new QPushButton("Снять отметки", this);
    layForButtons->addWidget(selectAllButton);
    layForButtons->addSpacing(5);
    layForButtons->addWidget(unselectAllButton);
}


void QMarkWidget::setItems(QVector<WidgetToMarkItemInfo> infoList)
{
    for (int i=0;i<infoList.count();i++)
    {
        listWidgetItem = new QListWidgetItem(listWidget);
        listWidget->addItem (listWidgetItem);
        markWidgetItem = new QMarkWidgetItem(infoList.at(i),listWidget);
        connect(selectAllButton,SIGNAL(clicked()),markWidgetItem,SLOT(slotChangeStateForChechBoxOnTrue()));
        connect(unselectAllButton,SIGNAL(clicked()),markWidgetItem,SLOT(slotChangeStateForChechBoxOnFalse()));
        connect(markWidgetItem,SIGNAL(signalCheckBoxWasPressed()),this,SIGNAL(signalNumberOfCategoriesUsedHasChanged()));
            //Making sure that the listWidgetItem has the same size as the TheWidgetItem
        listWidgetItem->setSizeHint (markWidgetItem->sizeHint ());
        listWidget->setItemWidget(listWidgetItem, markWidgetItem);
    }
}

QVector<WidgetToMarkItemInfo> QMarkWidget::getListInfoAboutWidgets()
{
    QVector<WidgetToMarkItemInfo> Output;

    for(int i=0;i<listWidget->count();i++)
    {
        WidgetToMarkItemInfo info = dynamic_cast<QMarkWidgetItem*>(listWidget->itemWidget(listWidget->item(i)))->getInfoAboutWidget();
        info.itemId = i+1;

        Output.push_back(info);
    }

    return Output;
}

void QMarkWidget::setTitle(QString title)
{
    mainGroupBox->setTitle(title);
}

void QMarkWidget::setMinAndMaxHeight(int min, int max)
{
    this->setMinimumHeight(min);
    this->setMaximumHeight(max);
}

void QMarkWidget::slotSearchTextChanged(QString searchText)
{
    for(int i=0;i<listWidget->count();i++)
    {
        markWidgetItem = dynamic_cast<QMarkWidgetItem*>(listWidget->itemWidget(listWidget->item(i)));
        if(markWidgetItem->getInfoAboutWidget().itemName.contains(searchText) ||
           searchText==nullptr || searchText=="")
        {
            listWidget->setItemHidden(listWidget->item(i), false);
        }
        else
        {
            listWidget->setItemHidden(listWidget->item(i), true);
        }
    }
}
