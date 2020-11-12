#include "assignment.h"
#include "ui_assignment.h"

Assignment::Assignment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Assignment)
{
    ui->setupUi(this);
    this->setWindowTitle("EntryDialog");
    HideUnassignButton();
}

Assignment::~Assignment()
{
    delete ui;
}

void Assignment::HideUnassignButton()
{
    ui->btn_unassign->setVisible(0);
}

void Assignment::ShowUnassignButton()
{
    ui->btn_unassign->setVisible(1);
}

void Assignment::TransportData(QString room, QStringList classList, QStringList courseList, QStringList teacherList)
{
    ui->label_room->setText(room);
    ui->comboBox_class->addItems(classList);
    ui->comboBox_course->addItems(courseList);
    ui->comboBox_teacher->addItems(teacherList);
}

void Assignment::UpdateData(QString room, QString clas, QString course, QString teacher)
{
    ui->label_room->setText(room);
    ui->comboBox_class->setCurrentText(clas);
    ui->comboBox_course->setCurrentText(course);
    ui->comboBox_teacher->setCurrentText(teacher);
}

void Assignment::on_btn_unassign_clicked()
{
    emit UnAssign();
    on_btn_cancel_clicked();
}

void Assignment::on_btn_save_clicked()
{
    emit Assign(ui->comboBox_class->currentText(),ui->comboBox_course->currentText(),ui->comboBox_teacher->currentText());
    on_btn_cancel_clicked();
}

void Assignment::on_btn_cancel_clicked()
{
    this->close();
}
