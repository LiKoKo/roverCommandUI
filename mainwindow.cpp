#include "mainwindow.h"
#include <QDialog>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QProgressBar>
#include <QtNetwork/QTcpSocket>
#include <vlc/libvlc.h>
#include <vlc/vlc.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

}

void MainWindow::getViewIDs(int windowID0,int windowID1,int windowID2,int windowID3)
{
    winID0 = windowID0;
    winID1 = windowID1;
    winID2 = windowID2;
    winID3 = windowID3;
}

int MainWindow::connectToCameras()
{

    /// Camera HTTP stream

            // Example URL http://72.253.170.172 for beach cam
            // rtsp://inet.orban.com:554/tropic.mp4 for video
    /// Test Video Player

            /// Camera 0

            // Load VLC engine
            inst0 = libvlc_new(0, NULL);

            // Open Media
            m0 = libvlc_media_new_path(inst0,"test.mp4");
            ///m = libvlc_media_new_path(inst,"rtsp://inet.orban.com:554/tropic.mp4"); /// Sound only, no video?
            mp0 = libvlc_media_player_new_from_media(m0);

            // Linux player
            libvlc_media_player_set_xwindow(mp0, winID0);
///                libvlc_media_player_set_hwnd(mp0,(void*)view0->winId());///Windows option

            // release
            libvlc_media_release(m0);
            libvlc_media_player_play(mp0);

            /// Camera 1

            // Load VLC engine
            inst1 = libvlc_new(0, NULL);

            // Open Media
            m1 = libvlc_media_new_path(inst1,"test.mp4");

            mp1 = libvlc_media_player_new_from_media(m1);

            libvlc_media_player_set_xwindow(mp1, winID1);

            // release
            libvlc_media_release(m1);

            libvlc_media_player_play(mp1);


            /// Camera 3

            // Load VLC engine
            inst3 = libvlc_new(0, NULL);

            // Open Media
            m3 = libvlc_media_new_path(inst3,"rtsp://quicktime.uvm.edu:1554/waw/wdi05hs2b.mov");

            mp3 = libvlc_media_player_new_from_media(m3);

            libvlc_media_player_set_xwindow(mp3, winID3);

            // release
            libvlc_media_release(m3);

            libvlc_media_player_play(mp3);

        return 0;
}

int MainWindow::acquireTelemetry()
{
    // This should be a threaded loop to read in from sockets
    ///CAN Bus
    batt1 = 1;
    batt2 = 2;
    motors1 = 3;
    motors2 = 4;
    sadlAng = 7;
    ///Arduino Yun
    temp = 5;
    gyroAng = 6;


    return 0;
}

int MainWindow::connectMaster()
{
    QDialog *masterConnect = new QDialog;
    masterConnect->setWindowTitle("Attempting to Connect as Master");
    masterConnect->setFixedSize(500,200);

    QVBoxLayout *connectLayout = new QVBoxLayout;
    masterConnect->setLayout(connectLayout);

    QPushButton *cancel = new QPushButton;
    cancel->setFixedSize(50,25);
    cancel->setText("Abort");

    QProgressBar *connectStatus = new QProgressBar;

    QLabel *connectDetails = new QLabel;
    connectDetails->setText("Checking Connection Status...");
    // Check Connection Status
    connectStatus->setValue(1);

    connectLayout->addWidget(connectStatus);
    connectLayout->addWidget(connectDetails);
    connectLayout->addWidget(cancel);

    QObject::connect(cancel,SIGNAL(clicked()),masterConnect,SLOT(close()));

    masterConnect->show();

    connectToCameras();

    return 0;
}

int MainWindow::connectViewer()
{
    QDialog *viewerConnect = new QDialog;
    viewerConnect->setWindowTitle("Attempting to Connect as Viewer");
    viewerConnect->setFixedSize(500,200);

    QVBoxLayout *connectLayout = new QVBoxLayout;
    viewerConnect->setLayout(connectLayout);

    QPushButton *cancel = new QPushButton;
    cancel->setFixedSize(50,25);
    cancel->setText("Abort");

    QProgressBar *connectStatus = new QProgressBar;

    QLabel *connectDetails = new QLabel;
    connectDetails->setText("Checking Connection Status...");
    // Check Connection Status
    connectStatus->setValue(1);

    connectLayout->addWidget(connectStatus);
    connectLayout->addWidget(connectDetails);
    connectLayout->addWidget(cancel);

    QObject::connect(cancel,SIGNAL(clicked()),viewerConnect,SLOT(close()));

    viewerConnect->show();

    connectToCameras();

    return 0;
}

int MainWindow::disconnect()
{
    QDialog *disconnect = new QDialog;
    disconnect->setWindowTitle("Disconnecting");
    disconnect->setFixedSize(500,200);

    QVBoxLayout *connectLayout = new QVBoxLayout;
    disconnect->setLayout(connectLayout);

    QPushButton *cancel = new QPushButton;
    cancel->setFixedSize(50,25);
    cancel->setText("Cancel");

    QProgressBar *connectStatus = new QProgressBar;

    QLabel *connectDetails = new QLabel;
    connectDetails->setText("Checking Connection Status...");
    // Check Connection Status
    connectStatus->setValue(1);

    connectLayout->addWidget(connectStatus);
    connectLayout->addWidget(connectDetails);
    connectLayout->addWidget(cancel);

    QObject::connect(cancel,SIGNAL(clicked()),disconnect,SLOT(close()));

    disconnect->show();

    /// Oddly Enough, the media stops before the dialog pops

    libvlc_media_player_stop(mp0);
    libvlc_media_player_stop(mp1);
    libvlc_media_player_stop(mp3);

    return 0;
}

void MainWindow::controlsDialog()
{
    QDialog *controls = new QDialog;
    controls->setWindowTitle("Argo Rover Control");
    controls->setFixedSize(500,200);

    QPushButton *controlsClose = new QPushButton;
    controlsClose->setText("Close");
    controlsClose->setFixedSize(50,25);

    QObject::connect(controlsClose,SIGNAL(clicked()),controls,SLOT(close()));

    QVBoxLayout *controlsLayout = new QVBoxLayout;
    controls->setLayout(controlsLayout);

    /// Temporary About Text
    QLabel *text = new QLabel;
    text->setText("W - Forward\nS - Reverse\nA - Turn Left\nD - Turn Right\nSpace Bar - Brake\n");

    controlsLayout->addWidget(text);
    controlsLayout->addWidget(controlsClose);


    controls->show();
}

void MainWindow::aboutDialog()
{
    QDialog *aboutBox = new QDialog;
    aboutBox->setWindowTitle("About");
    aboutBox->setFixedSize(500,500);

    QPushButton *aboutClose = new QPushButton;
    aboutClose->setText("Close");
    aboutClose->setFixedSize(50,25);

    QObject::connect(aboutClose,SIGNAL(clicked()),aboutBox,SLOT(close()));

    QVBoxLayout *aboutLayout = new QVBoxLayout;
    aboutBox->setLayout(aboutLayout);

    /// Temporary About Text
    QLabel *text = new QLabel;
    text->setText("PISCES Team\nRodrigo Romo - Project Manager\n\nHILO Team\nMax - Intern\nCasey Pearing - Intern\n\nMaui Team\nLindsay Komai - Lead Intern\nBlayne Morton - Programmer/Intern\nJordan Moore - Programmer/Intern\nDr. Anil Mehta - Team Mentor");

    aboutLayout->addWidget(text);
    aboutLayout->addWidget(aboutClose);


    aboutBox->show();
}

int MainWindow::sendUp()
{
    writeData.open("/home/besalt/roverCommandUI/roverCommandUI/data/roverCommandLog");

    writeData.operator <<("Pitch Up");

    writeData.close();

    statusBar()->showMessage("ROVER: Tilt Up");
    return 0;
}

int MainWindow::sendDown()
{
    writeData.open("/home/besalt/roverCommandUI/roverCommandUI/data/roverCommandLog");

    writeData.operator <<("Pitch Down");

    writeData.close();

    statusBar()->showMessage("ROVER: Tilt Down");
    return 0;
}

int MainWindow::sendForward()
{
    /// file stream write is a placeholder for socket write
    writeData.open("/home/besalt/roverCommandUI/roverCommandUI/data/roverCommandLog");

    writeData.operator <<("forward");

    writeData.close();

    statusBar()->showMessage("ROVER: Forward",0);

    return 0;
}

int MainWindow::sendReverse()
{
    /// file stream write is a placeholder for socket write

    writeData.open("/home/besalt/roverCommandUI/roverCommandUI/data/roverCommandLog");

    writeData.operator <<("reverse");

    writeData.close();

    statusBar()->showMessage("ROVER: Backward",0);

    return 0;
}

int MainWindow::sendLeft()
{
    /// file stream write is a placeholder for socket write
    writeData.open("/home/besalt/roverCommandUI/roverCommandUI/data/roverCommandLog");

    writeData.operator <<("left");

    writeData.close();

    statusBar()->showMessage("ROVER: Left",0);

    return 0;
}

int MainWindow::sendRight()
{
    /// file stream write is a placeholder for socket write
    writeData.open("/home/besalt/roverCommandUI/roverCommandUI/data/roverCommandLog");

    writeData.operator <<("right");

    writeData.close();

    statusBar()->showMessage("ROVER: Right",0);

    return 0;
}

int MainWindow::sendBrake()
{
    /// file stream write is a placeholder for socket write
    writeData.open("/home/besalt/roverCommandUI/roverCommandUI/data/roverCommandLog");

    writeData.operator <<("brake");

    writeData.close();

    statusBar()->showMessage("ROVER: Brake On",0);

    return 0;
}

MainWindow::~MainWindow()
{

}
