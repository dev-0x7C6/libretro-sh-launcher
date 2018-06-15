#include <QApplication>
//#include <QCommandLineParser>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QDesktopWidget>
#include <QFont>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QVariant>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);

    QVariant pixsize = 10;
    QString p_gtkrc = QDir::home().path() + "/.gtkrc-2.0";
    QFile fgtkrc(p_gtkrc);
    if (fgtkrc.exists())
    {
        QSettings gtkrc(p_gtkrc, QSettings::IniFormat);
        QString fname = gtkrc.value("gtk-font-name").toString();
        QStringList tmp = fname.split(" ");
        pixsize = tmp.at(tmp.size()-1);
    }

    QDialog widget(0, Qt::Dialog | Qt::FramelessWindowHint | Qt::Popup);

    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *label = new QLabel;
    QFont font;
    font.setPixelSize(pixsize.toInt() * 2);
    label->setFont(font);
    label->setText("Loading...");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    widget.setMinimumWidth(app.desktop()->geometry().width());
    widget.setMinimumHeight(app.desktop()->geometry().height());
    widget.setLayout(layout);

    widget.setStyleSheet("QWidget{background: black;} QLabel{color: white;}");

    widget.show();

    return app.exec();
}
