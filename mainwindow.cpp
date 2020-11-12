#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionopen_triggered()
{
    filepath = QFileDialog::getOpenFileName(this,"please choose json files",".","Json(*.json)");
    if(filepath.isEmpty()) return ;
    AnalasisJsonFile(filepath);
    setWindowTitle("SchoolPlanner");
}

void MainWindow::AnalasisJsonFile(QString filepath)
{
    QString content;
    QFile jsonFile(filepath);
    if(!jsonFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"json tips","json open false!"+jsonFile.errorString());
        return ;
    }
    else
    {
        content = jsonFile.readAll();
        jsonFile.close();
    }
    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError))
    {
        QMessageBox::warning(this,"json tips",tr("analasis json file error!%1").arg(parseJsonErr.error));
        return;
    }
    QJsonObject jsonObject = document.object();
    if(jsonObject.contains(QStringLiteral("rooms")))
    {
        roomsList.clear();
        ui->comboBox->clear();
        QJsonArray data = jsonObject["rooms"].toArray();
        foreach (QJsonValue v, data) {
           roomsList.append(v.toString());
        }
        updateRoomCombobox();
    }
    if(jsonObject.contains(QStringLiteral("groups")))
    {
        classList.clear();
        QJsonArray data = jsonObject["groups"].toArray();
        foreach (QJsonValue v, data) {
           classList.append(v.toString());
        }
    }
    if(jsonObject.contains(QStringLiteral("classes")))
    {
        courseList.clear();
        QJsonArray data = jsonObject["classes"].toArray();
        foreach (QJsonValue v, data) {
           courseList.append(v.toString());
        }
    }
    if(jsonObject.contains(QStringLiteral("teachers")))
    {
        teachersList.clear();
        QJsonArray data = jsonObject["teachers"].toArray();
        foreach (QJsonValue v, data) {
           teachersList.append(v.toString());
        }
    }
    if(jsonObject.contains(QStringLiteral("slots")))
    {
        slotsList.clear();
        QJsonArray data = jsonObject["slots"].toArray();
        foreach (QJsonValue v, data) {
           slotsList.append(v.toString());
        }
    }
    if(jsonObject.contains(QStringLiteral("weeks")))
    {
        weekList.clear();
        QJsonArray data = jsonObject["weeks"].toArray();
        foreach (QJsonValue v, data) {
           weekList.append(v.toString());
        }
    }
    if(jsonObject.contains(QStringLiteral("activities")))
    {
        activitiesList.clear();
        QJsonArray data = jsonObject["activities"].toArray();
        foreach (QJsonValue v, data) {
            QMap<QString,QString> map;
            map.insert("room",v["room"].toString());
            map.insert("group",v["group"].toString());
            map.insert("class",v["class"].toString());
            map.insert("slot",v["slot"].toString());
            map.insert("teacher",v["teacher"].toString());
            map.insert("week",v["week"].toString());
            activitiesList.append(map);
        }
        qDebug()<<"map length:"<<activitiesList.length();
    }
    if(ui->comboBox->count()>0)
        InsertIntoTableWidget(ui->comboBox->itemText(0));
}

void MainWindow::updateRoomCombobox()
{
    ui->comboBox->clear();
    foreach (QString v, roomsList) {
       ui->comboBox->addItem(v);
    }
}

void MainWindow::InsertIntoTableWidget(QString room)
{
    ui->tableWidget->clearContents();
    for(int i=0;i<activitiesList.length();i++){
        QMap<QString,QString> map(activitiesList.at(i));
        if(map.value("room")==room)
        {
            int row = map.value("slot").toInt()-1;
            int column = weekList.indexOf(map.value("week"));
            ui->tableWidget->setCellWidget(row,column,new QLabel(QString("%1(%2)").arg(map.value("group")).arg(map.value("class"))));
        }
    }
}

void MainWindow::TravelMapList()
{
    for(int i=0;i<activitiesList.count();i++){
        qDebug()<<activitiesList.at(i);
    }
}

void MainWindow::saveJsonFile(QString path)
{
    QJsonObject json;
    QJsonArray room_arr,group_arr,class_arr,teacher_arr,week_arr,slot_arr,activities_arr;
    for(int i=0;i<roomsList.count();i++) room_arr.append(roomsList[i]);
    for(int i=0;i<classList.count();i++) group_arr.append(classList[i]);
    for(int i=0;i<courseList.count();i++) class_arr.append(courseList[i]);
    for(int i=0;i<teachersList.count();i++) teacher_arr.append(teachersList[i]);
    for(int i=0;i<slotsList.count();i++) slot_arr.append(slotsList[i]);
    for(int i=0;i<weekList.count();i++) week_arr.append(weekList[i]);

    for(int i=0;i<activitiesList.count();i++)
    {
        QJsonObject obj;
        obj.insert("room",activitiesList.at(i).value("room"));
        obj.insert("group",activitiesList.at(i).value("group"));
        obj.insert("class",activitiesList.at(i).value("class"));
        obj.insert("slot",activitiesList.at(i).value("slot"));
        obj.insert("teacher",activitiesList.at(i).value("teacher"));
        obj.insert("week",activitiesList.at(i).value("week"));
        activities_arr.append(obj);
    }
    json.insert("rooms",room_arr);
    json.insert("groups",group_arr);
    json.insert("classes",class_arr);
    json.insert("teachers",teacher_arr);
    json.insert("weeks",week_arr);
    json.insert("slots",slot_arr);
    json.insert("activities",activities_arr);
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,"false","save open 1 false :"+file.errorString());
        return;
    }
    file.close();
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,"false","save open 2 false :"+file.errorString());
        return;
    }
    file.write(strJson.toUtf8());
    file.close();
}

void MainWindow::on_actionRooms_triggered()
{
    QListModel* listModel = new QListModel;
    listModel->updateListWidget(roomsList);
    listModel->show();
    connect(listModel,&QListModel::AddNewItem,[=](QString content){
        roomsList.append(content);
        updateRoomCombobox();
    });
    connect(listModel,&QListModel::DeleteItem,[=](QString content){
        roomsList.removeOne(content);
        updateRoomCombobox();
    });
}

void MainWindow::on_actionTeachers_triggered()
{
    QListModel* listModel = new QListModel;
    listModel->updateListWidget(teachersList);
    listModel->show();
    connect(listModel,&QListModel::AddNewItem,[=](QString content){
        teachersList.append(content);
    });
    connect(listModel,&QListModel::DeleteItem,[=](QString content){
        teachersList.removeOne(content);
    });
}

void MainWindow::on_actionCourses_triggered()
{
    QListModel* listModel = new QListModel;
    listModel->updateListWidget(classList);
    listModel->show();
    connect(listModel,&QListModel::AddNewItem,[=](QString content){
        classList.append(content);
    });
    connect(listModel,&QListModel::DeleteItem,[=](QString content){
        classList.removeOne(content);
    });
}

void MainWindow::on_actionClasses_triggered()
{
    QListModel* listModel = new QListModel;
    listModel->updateListWidget(courseList);
    listModel->show();
    connect(listModel,&QListModel::AddNewItem,[=](QString content){
        courseList.append(content);
    });
    connect(listModel,&QListModel::DeleteItem,[=](QString content){
        courseList.removeOne(content);
    });
}

void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    Assignment* assignment = new Assignment;
    bool hasContent=false;
    int activitiedIndex=-1;
    assignment->TransportData(ui->comboBox->currentText(),classList,courseList,teachersList);


    if(index.data()!="")
    {
        for(int i=0;i<activitiesList.count();i++)
        {
            QMap<QString,QString> map(activitiesList.at(i));
            //qDebug()<<map.value("room")<<ui->comboBox->currentText()<<map.value("week")<<weekList.at(index.column())<<map.value("slot")<<index.row();
            if(map.value("room")==ui->comboBox->currentText()&&map.value("week")==weekList.at(index.column())&&map.value("slot").toInt()==index.row()+1)
            {
                assignment->ShowUnassignButton();
                assignment->UpdateData(map.value("room"),map.value("group"),map.value("class"),map.value("teacher"));
                hasContent=true;
                activitiedIndex=i;
                break;
            }
        }
    }
    assignment->show();


    connect(assignment,&Assignment::UnAssign,[=](){
        if(hasContent)
        {
            activitiesList.remove(activitiedIndex);
            InsertIntoTableWidget(ui->comboBox->currentText());
        }
    });
    connect(assignment,&Assignment::Assign,[=](QString clas,QString course,QString teacher){
        QMap<QString,QString> newMap;
        newMap.insert("teacher",teacher);
        newMap.insert("group",clas);
        newMap.insert("class",course);
        if(hasContent)
        {
            QMap<QString,QString> oldmap = activitiesList.at(activitiedIndex);
            newMap.insert("room",oldmap.value("room"));
            newMap.insert("slot",oldmap.value("slot"));
            newMap.insert("week",oldmap.value("week"));
            activitiesList.remove(activitiedIndex);
        }
        else
        {
            newMap.insert("room",ui->comboBox->currentText());
            newMap.insert("slot",QString::number(index.row()+1));
            newMap.insert("week",weekList.at(index.column()));
        }
        activitiesList.append(newMap);
        InsertIntoTableWidget(ui->comboBox->currentText());
    });
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    InsertIntoTableWidget(arg1);
}

void MainWindow::on_actionsave_triggered()
{
    saveJsonFile(filepath);
}

void MainWindow::on_actionSave_as_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,"please choose save path",".","json(*.json)");
    if(path.isEmpty())
    {
        QMessageBox::warning(this,"tips","path not empty!");
        return;
    }
    saveJsonFile(path);
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}
