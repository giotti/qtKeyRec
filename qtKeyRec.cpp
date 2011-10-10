#include "./qtKeyRec.h"
#include "./ui_qtKeyRec.h"

#ifdef QTOPIA
#include <QtopiaServiceRequest>
#include <QValueSpaceItem>
#endif

//------
//MyProcess::MyProcess( QObject* )
//: QProcess()
//{
//    _item = NULL;
//}

//MyProcess::~MyProcess() {}

//------



QtKeyRec::QtKeyRec(QWidget *parent)
: QWidget(parent),
    ui(new Ui::QtKeyRec) {
    ui->setupUi(this);
    //program = "/home/jack/GIT/qtmoko/src/3rdparty/applications/qtKeyRec/bin/om";

    _volume = 100;

//    connect(ui->volume, SIGNAL(valueChanged(int)), this,  SLOT(onVolumeChanged(int)));

//    _rh->start(); // the default sample rate is 500ms

//    rh->stop();

//    rh->restore();

    //--- connections ---

}

//void QtKeyRec::initProcess(QString prog, QStringList args ) {
//    //TODO change absolute path with something smarter...


//    MyProcess* p = new MyProcess();
//    //p->setAssociatedItem(sb);
//    //connect(p, SIGNAL(finished(int)), this, SLOT(readOmOutput()));

//    //launch om
//    p->start(prog, args);
//}

QtKeyRec::~QtKeyRec() {
    delete ui;
}

void QtKeyRec::on_quitButton_clicked()
{
    close();
}

void QtKeyRec::on_startButton_clicked()
{
    //connect( ui->comboBox, SIGNAL( currentIndexChanged(const QString&) ), this, SLOT(const insulto));
    //QString command = "/usr/bin/mplayer -volume " + QString::number(_volume) + " /opt/qtmoko/sounds/" + QString::character(insulto) + ".wav";
    QString command = "/usr/bin/mplayer -volume " + QString::number(_volume) + " /opt/qtmoko/sounds/KeyRec.wav";
    system(qPrintable(command));
    //system( command.toStdString().c_str() );
}
