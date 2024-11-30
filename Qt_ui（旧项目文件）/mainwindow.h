#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QCoreApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QScreen>
#include <QSplitter>
#include <QTextEdit>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>
#include <QSpinBox>
#include <QScrollBar>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // 创建主窗口布局
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        QScreen *screen = QGuiApplication::primaryScreen();
        QSize screenSize = screen->size();

        int rpxWidth = screenSize.width() / 375;
        int rpxHeight = screenSize.height() / 667;
        setFixedSize(rpxWidth * 210, rpxHeight * 500);


        QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);

        // 左侧操作区
        QWidget *leftWidget = new QWidget(this);

        QLabel *inputLabel = new QLabel("Input File Path: ", this);
        QLabel *outputLabel = new QLabel("Output Folder Path: ", this);
        inputLabel->hide();
        outputLabel->hide();
        //文本框前标签
        QLabel *intervalLabel = new QLabel("Input Interval(seconds)", this);
        // 创建数字文本框
        m_spinBox = new QSpinBox(this);
        m_spinBox->setRange(1, 300);  // 设置数值范围
        m_spinBox->setValue(15);

        QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
        QPushButton *selectInputButton = new QPushButton("Select Input File", this);
        QPushButton *selectOutputButton = new QPushButton("Select Output Folder", this);
        QPushButton *runButton1 = new QPushButton("Backup 1", this); // 添加的按钮
        QPushButton *cancelButton1 = new QPushButton("Cancel Backup 1", this);
        cancelButton1->setEnabled(false);

        QPushButton *runButton2 = new QPushButton("Backup 2", this); // 添加的按钮
        QPushButton *cancelButton2 = new QPushButton("Cancel Backup 2", this);
        cancelButton2->setEnabled(false);

        QLabel *statusLabel = new QLabel("Status: Not Running", this);
        QTextEdit *logOutput = new QTextEdit(this);
        logOutput->setReadOnly(true);
        logOutput->hide();
        statusLabel->hide();
        QTimer *monitorTimer1 = new QTimer(this);
        connect(monitorTimer1, &QTimer::timeout, this, [=]() {
            bool isRunning = checkProcessRunning("update.exe");
            runButton1->setEnabled(!isRunning);
            cancelButton1->setEnabled(isRunning);
            statusLabel->setText(QString("Status: %1").arg(isRunning ? "Running" : "Not Running"));
        });

        monitorTimer1->start(1000);

        QTimer *monitorTimer2 = new QTimer(this);
        connect(monitorTimer2, &QTimer::timeout, this, [=]() {
            bool isRunning = checkProcessRunning("freq.exe");
            runButton2->setEnabled(!isRunning);
            cancelButton2->setEnabled(isRunning);
            statusLabel->setText(QString("Status: %1").arg(isRunning ? "Running" : "Not Running"));
        });

        monitorTimer2->start(1000);




        // Connect input button
        connect(selectInputButton, &QPushButton::clicked, this, [this, inputLabel]() {
            QString inputFile = QFileDialog::getOpenFileName(this,
                                                             "Select Input File",
                                                             "",
                                                             "DAT Files (*.dat);;All Files (*.*)");

            if (!inputFile.isEmpty()) {
                inputLabel->setText("Input File Path: " + inputFile);
                m_inputFilePath = inputFile;
            }
        });

        // Connect output button
        connect(selectOutputButton, &QPushButton::clicked, this, [this, outputLabel]() {
            QString outputFolder = QFileDialog::getExistingDirectory(this,
                                                                     "Select Output Folder",
                                                                     "");

            if (!outputFolder.isEmpty()) {
                outputLabel->setText("Output Folder Path: " + outputFolder);
                m_outputFolderPath = outputFolder;
            }
        });
        connect(cancelButton1, &QPushButton::clicked, this, [=]() {
            if (terminateProcess("update.exe")) {
                logOutput->append("update.exe terminated successfully.");
            } else {
                logOutput->append("Failed to terminate update.exe.");
            }
        });

        // Connect run button
        connect(runButton1, &QPushButton::clicked, this, [this]() {
            if (m_inputFilePath.isEmpty() || m_outputFolderPath.isEmpty()) {
                QMessageBox::warning(this, "Warning", "Please select input file and output folder first!");
                return;
            }

            QProcess *process = new QProcess(this);
            QTextEdit *log1TextEdit = findChild<QTextEdit *>("log1TextEdit");

            // Connect process output to log1 area
            connect(process, &QProcess::readyReadStandardOutput, this, [process, log1TextEdit]() {
                QString output = process->readAllStandardOutput();
                if (log1TextEdit) {
                    log1TextEdit->append(output); // Append program output to log1
                }
            });

            // Connect process error output to log1 area
            connect(process, &QProcess::readyReadStandardError, this, [process, log1TextEdit]() {
                QString error = process->readAllStandardError();
                if (log1TextEdit) {
                    log1TextEdit->append("[Error] " + error);
                }
            });

            // Start the executable
            QString currentPath = QCoreApplication::applicationDirPath();
            QString executablePath = currentPath + "/toolkit/update.exe"; // 替换为实际的可执行文件路径
            process->start(executablePath, QStringList() << m_inputFilePath << m_outputFolderPath);

            if (!process->waitForStarted()) {
                QMessageBox::critical(this, "Error", "Failed to start the executable!");
            }
        });

        connect(cancelButton2, &QPushButton::clicked, this, [=]() {
            if (terminateProcess("freq.exe")) {
                logOutput->append("freq.exe terminated successfully.");
            } else {
                logOutput->append("Failed to terminate freq.exe.");
            }
        });

        // Connect run button
        connect(runButton2, &QPushButton::clicked, this, [this]() {
            if (m_inputFilePath.isEmpty() || m_outputFolderPath.isEmpty()) {
                QMessageBox::warning(this, "Warning", "Please select input file and output folder first!");
                return;
            }

            QProcess *process2 = new QProcess(this);
            QTextEdit *log2TextEdit = findChild<QTextEdit *>("log2TextEdit");

            // Connect process output to log2 area
            connect(process2, &QProcess::readyReadStandardOutput, this, [process2, log2TextEdit]() {
                QString output = process2->readAllStandardOutput();
                if (log2TextEdit) {
                    log2TextEdit->append(output); // Append program output to log2
                }
            });

            // Connect process error output to log2 area
            connect(process2, &QProcess::readyReadStandardError, this, [process2, log2TextEdit]() {
                QString error = process2->readAllStandardError();
                if (log2TextEdit) {
                    log2TextEdit->append("[Error] " + error);
                }
            });

            // Start the executable
            QString currentPath = QCoreApplication::applicationDirPath();
            QString executablePath = currentPath + "/toolkit/freq.exe"; // 替换为实际的可执行文件路径
            int interval = m_spinBox->value();
            process2->start(executablePath, QStringList() << m_inputFilePath << m_outputFolderPath << QString::number(interval));

            if (!process2->waitForStarted()) {
                QMessageBox::critical(this, "Error", "Failed to start the executable!");
            }
        });

        leftLayout->addWidget(inputLabel);
        leftLayout->addWidget(selectInputButton);
        leftLayout->addWidget(outputLabel);
        leftLayout->addWidget(selectOutputButton);

        QHBoxLayout *hLayout1 = new QHBoxLayout;
        hLayout1->addWidget(runButton1);
        hLayout1->addWidget(cancelButton1);
        leftLayout->addLayout(hLayout1);

        QHBoxLayout *hLayout2 = new QHBoxLayout;
        hLayout2->addWidget(intervalLabel);
        hLayout2->addWidget(m_spinBox);
        leftLayout->addLayout(hLayout2);

        QHBoxLayout *hLayout3 = new QHBoxLayout;
        hLayout3->addWidget(runButton2);
        hLayout3->addWidget(cancelButton2);
        leftLayout->addLayout(hLayout3);

        leftLayout->addStretch(); // 保持按钮靠上排列
        leftWidget->setLayout(leftLayout);

        // 右侧日志区（垂直分隔器）
        QSplitter *rightSplitter = new QSplitter(Qt::Vertical, mainSplitter);

        // Log 1 区域
        QWidget *log1Widget = new QWidget(this);
        QVBoxLayout *log1Layout = new QVBoxLayout(log1Widget);
        QLabel *log1Label = new QLabel("Log 1", log1Widget);
        QTextEdit *log1TextEdit = new QTextEdit(log1Widget);
        log1TextEdit->setObjectName("log1TextEdit");
        log1Layout->addWidget(log1Label);
        log1Layout->addWidget(log1TextEdit);
        log1Widget->setLayout(log1Layout);

        // Log 2 区域
        QWidget *log2Widget = new QWidget(this);
        QVBoxLayout *log2Layout = new QVBoxLayout(log2Widget);
        QLabel *log2Label = new QLabel("Log 2", log2Widget);
        QTextEdit *log2TextEdit = new QTextEdit(log2Widget);
        log2TextEdit->setObjectName("log2TextEdit");
        log2Layout->addWidget(log2Label);
        log2Layout->addWidget(log2TextEdit);
        log2Widget->setLayout(log2Layout);

        QScrollBar *scrollBar1 = log1TextEdit->verticalScrollBar();
        if (scrollBar1->value() == scrollBar1->maximum()) {
            scrollBar1->setValue(scrollBar1->maximum());
        }

        QScrollBar *scrollBar2 = log2TextEdit->verticalScrollBar();
        if (scrollBar2->value() == scrollBar2->maximum()) {
            scrollBar2->setValue(scrollBar2->maximum());
        }



        log1TextEdit->setFocusPolicy(Qt::NoFocus);  // 禁用焦点事件
        log2TextEdit->setFocusPolicy(Qt::NoFocus);

        // 将 log1 和 log2 添加到右侧分隔器
        rightSplitter->addWidget(log1Widget);
        rightSplitter->addWidget(log2Widget);

        // 将左右部分添加到主分隔器
        mainSplitter->addWidget(rightSplitter);
        mainSplitter->addWidget(leftWidget);

        //split_line


        mainSplitter->setHandleWidth(0);

        // 设置左右部分的初始比例
        mainSplitter->setStretchFactor(0, 4); // 左侧占 3 份
        mainSplitter->setStretchFactor(1, 5); // 右侧占 4 份

        // 设置右侧上下部分的初始比例
        rightSplitter->setStretchFactor(0, 1); // 上半部分占 1 份
        rightSplitter->setStretchFactor(1, 1); // 下半部分占 1 份

        rightSplitter->setHandleWidth(0);
        // 设置中心部件为主分隔器
        setCentralWidget(mainSplitter);

    }
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString m_inputFilePath;  // 成员变量存储输入文件路径
    QString m_outputFolderPath; // 成员变量存储输出文件夹路径
    QSpinBox *m_spinBox;

    bool checkProcessRunning(const QString &processName) {
#ifdef Q_OS_WIN
        // Windows 平台下，使用 QProcessList 或 QWinProcessAPI 检测
        QProcess process;
        process.start("tasklist", QStringList() << "/FI" << QString("IMAGENAME eq %1").arg(processName));
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        return output.contains(processName, Qt::CaseInsensitive);
#else \
    // POSIX 平台下，使用 ps 命令检测
        QProcess process;
        process.start("pgrep", QStringList() << processName);
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        return !output.trimmed().isEmpty();
#endif
    }

    bool terminateProcess(const QString &processName) {
#ifdef Q_OS_WIN
        QProcess process;
        process.start("taskkill", QStringList() << "/IM" << processName << "/F");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        return output.contains("SUCCESS", Qt::CaseInsensitive);
#else
        QProcess process;
        process.start("pkill", QStringList() << processName);
        process.waitForFinished();
        return process.exitCode() == 0;
#endif
    }
};
#endif // MAINWINDOW_H
