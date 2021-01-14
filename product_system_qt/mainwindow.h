#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"infer.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include<QListWidgetItem>
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

    void on_infer_clicked();

    void on_restore_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *ItemModel;
    infer* inf;
};
#endif // MAINWINDOW_H
