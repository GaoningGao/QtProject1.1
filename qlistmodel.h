#ifndef QLISTMODEL_H
#define QLISTMODEL_H

#include <QWidget>

namespace Ui {
class QListModel;
}

class QListModel : public QWidget
{
    Q_OBJECT

public:
    explicit QListModel(QWidget *parent = nullptr);
    ~QListModel();

    void updateListWidget(QStringList items);
private slots:
    void on_btn_close_clicked();

    void on_btn_remove_clicked();

    void on_btn_add_clicked();

private:
    Ui::QListModel *ui;
signals:
    void AddNewItem(QString item);
    void DeleteItem(QString item);
};

#endif // QLISTMODEL_H
