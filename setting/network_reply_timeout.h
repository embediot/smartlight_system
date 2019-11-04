#ifndef __NETWORK_REPLY_TIMEOUT_H_
#define __NETWORK_REPLY_TIMEOUT_H_

#include <QDebug>
#include <QObject>
#include <QTimer>
#include <QNetworkReply>

class NwtworkReplyTimeout : public QObject
{

    Q_OBJECT

public:
    NwtworkReplyTimeout(QNetworkReply *reply, const int value) : QObject(reply)
    {
        Q_ASSERT(reply);
        if (reply && reply->isRunning())
        {
            //qDebug() << "NwtworkReplyTimeout is running";
            QTimer::singleShot(value, this, SLOT(slot_on_timeout()));
        }
    }

signals:
    void timeout();

private slots:
    void slot_on_timeout()
    {
        qDebug() << "NwtworkReplyTimeout is running";

        QNetworkReply *reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning())
        {
            reply->abort();
            reply->deleteLater();
            emit timeout();
        }
    }
};


#endif

