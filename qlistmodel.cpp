#include "qlistmodel.h"
#include "ui_qlistmodel.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
QListModel::QListModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QListModel)
{
    ui->setupUi(this);
    this->setWindowTitle("EditList");
}

QListModel::~QListModel()
{
    delete ui;
}

void QListModel::updateListWidget(QStringList items)
{
    ui->listWidget->addItems(items);
}

void QListModel::on_btn_close_clicked()
{
    this->close();
}

void QListModel::on_btn_remove_clicked()
{
    emit DeleteItem(ui->listWidget->item(ui->listWidget->currentRow())->text());
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}

void QListModel::on_btn_add_clicked()
{
    QWidget* w = new QWidget;
    w->setGeometry(this->geometry());
    QVBoxLayout* vlayout = new QVBoxLayout(w);
    QLineEdit* le = new QLineEdit();
    QPushButton* btn= new QPushButton("add");
    vlayout->addWidget(le);
    vlayout->addWidget(btn);
    w->show();
    connect(btn,&QPushButton::clicked,[=](){
        if(!le->text().isEmpty())
        {
            ui->listWidget->addItem(le->text());
            emit AddNewItem(le->text());
            w->close();
        }
    });
}
