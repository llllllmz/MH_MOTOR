#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QtGlobal>

// 自定义日志处理函数
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString logFilePath = "app.log"; // 日志文件路径

    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&logFile);
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

        switch (type) {
        case QtDebugMsg:
            stream << time << " [DEBUG] (" << context.file << ":" << context.line << ", " << context.function << ") " << localMsg.constData() << endl;
            break;
        case QtInfoMsg:
            stream << time << " [INFO] (" << context.file << ":" << context.line << ", " << context.function << ") " << localMsg.constData() << endl;
            break;
        case QtWarningMsg:
            stream << time << " [WARNING] (" << context.file << ":" << context.line << ", " << context.function << ") " << localMsg.constData() << endl;
            break;
        case QtCriticalMsg:
            stream << time << " [CRITICAL] (" << context.file << ":" << context.line << ", " << context.function << ") " << localMsg.constData() << endl;
            break;
        case QtFatalMsg:
            stream << time << " [FATAL] (" << context.file << ":" << context.line << ", " << context.function << ") " << localMsg.constData() << endl;
            break;
        }

        logFile.close();
    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    qInstallMessageHandler(customMessageHandler);
    MainWindow w;
    w.show();
    w.monitor();
    return a.exec();
}
