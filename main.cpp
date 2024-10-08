#include "mainwindow.h"

#include <QApplication>
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QDateTime>
#include <QtGlobal>

QFile logFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (!logFile.isOpen()) {
        logFile.setFileName("applicationDebug.log");
        logFile.open(QIODevice::Append | QIODevice::Text);
    }

    QTextStream out(&logFile);
    QString timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString logMessage;

    switch (type) {
    case QtDebugMsg:
        logMessage = QString("[%1] Debug: %2").arg(timeStamp, msg);
        break;
    case QtInfoMsg:
        logMessage = QString("[%1] Info: %2").arg(timeStamp, msg);
        break;
    case QtWarningMsg:
        logMessage = QString("[%1] Warning: %2").arg(timeStamp, msg);
        break;
    case QtCriticalMsg:
        logMessage = QString("[%1] Critical: %2").arg(timeStamp, msg);
        break;
    case QtFatalMsg:
        logMessage = QString("[%1] Fatal: %2").arg(timeStamp, msg);
        break;
    }

    out << logMessage << Qt::endl;

    QByteArray localMsg = logMessage.toUtf8();
    fprintf(stdout, "%s\n", localMsg.constData());
    fflush(stdout);

    if (type == QtFatalMsg) {
        abort();
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // setup our message handler
    qInstallMessageHandler(messageHandler);

    qDebug() << "Application statup";

    MainWindow w;
    w.show();
    return a.exec();
}
