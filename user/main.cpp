#include "welcomewidget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QElapsedTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    WelcomeWidget widget;

    QSplashScreen *splash=new QSplashScreen;

    QPixmap pix(":/picture/apple_jobs");

    splash->setPixmap(pix);
    splash->show();

    int delay_time = 5;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delay_time * 1000))
    {
         app.processEvents();
    }

    widget.show();

    splash->finish(&widget);

    return app.exec();
}






