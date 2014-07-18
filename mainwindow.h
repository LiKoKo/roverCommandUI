#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <stdio.h>
#include <QProgressBar>
#include <fstream>
#include <iostream>
#include <vlc/libvlc.h>
#include <vlc/vlc.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // Telemetry Data
    int batt1;
    int batt2;
    int motors1;
    int motors2;
    int temp;
    QString gpsLatval;
    QString gpsLongval;
    int gyroAng;
    int sadlAng;

    // Telemetry Functions
    int acquireTelemetry();

    // Networking Data
    int wifiStrength;
    int roverIP[4];
    int masterIP[4];
    int userCount;
    int roverTelemetryPort;
    int roverCommandPort;
    // Networking Functions

    // Camera Functions
        // Get View IDs
    void getViewIDs(int windowID0,int windowID1,int windowID2,int windowID3);
    int winID0;
    int winID1;
    int winID2;
    int winID3;
    int connectToCameras();

    libvlc_instance_t *inst0;
    libvlc_media_player_t *mp0;
    libvlc_media_t *m0;

    libvlc_instance_t *inst1;
    libvlc_media_player_t *mp1;
    libvlc_media_t *m1;

    libvlc_instance_t *inst3;
    libvlc_media_player_t *mp3;
    libvlc_media_t *m3;


private:
    std::fstream writeData;
    std::ifstream readData;
public slots:

    // Rover Movement
    int sendForward();
    int sendReverse();
    int sendLeft();
    int sendRight();
    int sendBrake();
    // Rover Suspension
    int sendUp();
    int sendDown();
    // GUI
    void aboutDialog();
    void controlsDialog();
    // Rover Connection
    int connectMaster();
    int connectViewer();
    int disconnect();
};


#endif // MAINWINDOW_H
