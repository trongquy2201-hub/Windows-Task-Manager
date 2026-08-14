#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <QLayout>
#include <QQuickItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ramQmlWidget = new QQuickWidget(this);
    //tro duong dan den file RamCharts
    ramQmlWidget->setSource(QUrl("qrc:/RamChart.qml"));
    ramQmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // 3. Đưa QML Widget vào giao diện UI (thêm vào layout hoặc cửa sổ)
    // Giả sử đưa vào layout chính của MainWindow
    ramQmlWidget->setGeometry(490,10,250,90);

    updateSystemData();// update data khi mo app
    timer=new QTimer(this);//cau hinh QTimer

    //Nối sự kiện timeout() của timer vào hàm updateSystemData()
    connect(timer,QTimer::timeout,this,&MainWindow::updateSystemData);
    timer->start(2000);//2s update 1 lan
    ui->tableProcess->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableProcess->setSelectionMode(QAbstractItemView::SingleSelection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSystemData(){
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        DWORDLONG totalMem = memInfo.ullTotalPhys / (1024 * 1024);
        DWORDLONG usedMem = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024 * 1024);

        int ramPercent = memInfo.dwMemoryLoad;
        ui->lblRAM->setText(QString("RAM USage: %1 MB / %2 MB (%3%)")
                            .arg(usedMem)
                            .arg(totalMem)
                            .arg(ramPercent));

        if (ramQmlWidget && ramQmlWidget->rootObject()){
            ramQmlWidget->rootObject()->setProperty("ramUsage",ramPercent);

            if (ramPercent>=80){
                ramQmlWidget->rootObject()->setProperty("statusColor","#FF4D4D");
            }else{
                ramQmlWidget->rootObject()->setProperty("statusColor","#00FF7F");
            }
        }   
    }

    //Chup snapshot toan bo process he thong
    HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if (hProcessSnap==INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe32;
    pe32.dwSize=sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap,&pe32)){
        CloseHandle(hProcessSnap);
        return;
    }
    ui->tableProcess->setUpdatesEnabled(false);
    ui->tableProcess->setRowCount(0);
    int row =0;
        //pe32.th32ProcessID: ID cua tien trinh(PID)
        //pe32.szExeFile: ten file exe(vd: chrome.exe, explorer.exe)
    do{
        ui->tableProcess->insertRow(row);
        ui->tableProcess->setItem(row,0,new QTableWidgetItem(QString::number(pe32.th32ProcessID)));//day PID vao cot 0
        ui->tableProcess->setItem(row,1,new QTableWidgetItem(QString::fromWCharArray(pe32.szExeFile)));//day ten Process vao cot 1
        row++;
    }while(Process32Next(hProcessSnap,&pe32));

    CloseHandle(hProcessSnap);
    ui->tableProcess->setUpdatesEnabled(true);
}
void MainWindow::on_btnRefresh_clicked()
{
    updateSystemData();
}


void MainWindow::on_btnKillProcess_clicked()
{
    //lay danh sach cac o duoc boi xanh tren bang
    QList<QTableWidgetItem*> selectedItems = ui->tableProcess->selectedItems();
    //neu chua chon ngung xu ly
    if(selectedItems.isEmpty()) return;

    //lay so dong cua o dau tien
    int row = selectedItems.first()->row();
    //lay PID tu cot 0 cua dong dang chon
    QTableWidgetItem *pidItem = ui->tableProcess->item(row,0);
    if(!pidItem) return;

    bool ok = false;
    DWORD pid = pidItem->text().toULong(&ok);

    //Thuc hien kill process
    if(ok && pid > 0){
        HANDLE hprocess = OpenProcess(PROCESS_TERMINATE,FALSE,pid);
        if (hprocess != NULL){
            TerminateProcess(hprocess,0);//ep buoc dung process
            CloseHandle(hprocess);//dong handle giai phong bo nho
        }

    }
    updateSystemData();//update ngay lap tuc

}



