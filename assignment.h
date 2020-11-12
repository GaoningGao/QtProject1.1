#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <QWidget>

namespace Ui {
class Assignment;
}

class Assignment : public QWidget
{
    Q_OBJECT

public:
    explicit Assignment(QWidget *parent = nullptr);
    ~Assignment();
    void TransportData(QString room,QStringList classList,QStringList courseList,QStringList teacherList);
    void UpdateData(QString room,QString clas,QString course,QString teacher);
    void HideUnassignButton();
    void ShowUnassignButton();

private slots:
    void on_btn_unassign_clicked();

    void on_btn_save_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::Assignment *ui;
signals:
    void UnAssign();
    void Assign(QString clas,QString course,QString teacher);
};

#endif // ASSIGNMENT_H
