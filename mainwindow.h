#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QTableWidgetItem>
#include "qlistmodel.h"
#include "assignment.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionopen_triggered();

    void on_actionRooms_triggered();

    void on_actionTeachers_triggered();

    void on_actionCourses_triggered();

    void on_actionClasses_triggered();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_comboBox_activated(const QString &arg1);

    void on_actionsave_triggered();

    void on_actionSave_as_triggered();

    void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;
    QStringList roomsList,teachersList,courseList,classList,slotsList,weekList;
    QVector< QMap<QString,QString> >activitiesList;
    int currentOperate;
    QString filepath;
private:
    void AnalasisJsonFile(QString filepath);
    void updateRoomCombobox();
    void InsertIntoTableWidget(QString room);
    void TravelMapList();
    void saveJsonFile(QString path);
};
#endif // MAINWINDOW_H
