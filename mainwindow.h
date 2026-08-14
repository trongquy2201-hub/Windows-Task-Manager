#include <QTimer>
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTableWidgetItem>
#include <QQuickWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_btnRefresh_clicked();

    void on_btnKillProcess_clicked();


private:
    Ui::MainWindow *ui;
    void updateSystemData();
    QTimer *timer;
    QQuickWidget *ramQmlWidget;
};
#endif // MAINWINDOW_H
