#include "mainwindow.h"
#include <QApplication>
#include <QAction>
#include <QIODevice>
#include <QDataStream>
#include <QLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>
#include <QtNetwork/QTcpSocket>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QSignalMapper>
#include <unistd.h>

void displayValue(QLCDNumber *a,int i)
{
    a->display(i);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow;

    //Setup Main Window
        QHBoxLayout *mainLayout = new QHBoxLayout;
        QWidget *centralWidget = new QWidget;
        centralWidget->setLayout(mainLayout);
        w->setCentralWidget(centralWidget);
        w->setWindowTitle("Argo Rover Control - Version 0.0.2");

        // Setup Menu Bar
        QMenuBar *menuBar = new QMenuBar;

        w->setMenuBar(menuBar);

        QMenu *connection = new QMenu;
        connection->setTitle("Connection");
        connection->setToolTip("Manage Connections");

        QAction *connectMaster = new QAction(0);
        connectMaster->setText("Connect As Master");
        connection->addAction(connectMaster);
        QObject::connect(connectMaster,SIGNAL(triggered()),w,SLOT(connectMaster()));

        QAction *connectViewer = new QAction(0);
        connectViewer->setText("Connect As Viewer");
        connection->addAction(connectViewer);
        QObject::connect(connectViewer,SIGNAL(triggered()),w,SLOT(connectViewer()));

        QAction *disconnect = new QAction(0);
        disconnect->setText("Disconnect");
        connection->addAction(disconnect);
        QObject::connect(disconnect,SIGNAL(triggered()),w,SLOT(disconnect()));

        connection->addSeparator();

        QAction *exitSession = new QAction(0);
        exitSession->setText("Quit");
        connection->addAction(exitSession);
        QObject::connect(exitSession,SIGNAL(triggered()),w,SLOT(close()));

        QMenu *window = new QMenu;
        window->setTitle("Window");
        window->setToolTip("Manage Window Settings");

        QAction *fullscreen = new QAction(0);
        fullscreen->setText("Fullscreen");
        window->addAction(fullscreen);
        QObject::connect(fullscreen,SIGNAL(triggered()),w,SLOT(showFullScreen()));

        QAction *normalizeView = new QAction(0);
        normalizeView->setText("Normal View");
        window->addAction(normalizeView);
        QObject::connect(normalizeView,SIGNAL(triggered()),w,SLOT(showNormal()));

        QMenu *help = new QMenu;
        help->setTitle("Help");
        help->setToolTip("More Information");

        QAction *controls = new QAction(0);
        controls->setText("Controls");
        help->addAction(controls);
        QObject::connect(controls,SIGNAL(triggered()),w,SLOT(controlsDialog()));

        QAction *about = new QAction(0);
        about->setText("About");
        help->addAction(about);
        QObject::connect(about,SIGNAL(triggered()),w,SLOT(aboutDialog()));

        menuBar->addMenu(connection);
        menuBar->addMenu(window);
        menuBar->addMenu(help);

        // Status Bar

        QStatusBar *roverStatus = new QStatusBar;
        w->setStatusBar(roverStatus);

        roverStatus->showMessage("Not Connected",0);

    //Setup telemetry

        //Build Telemetry display
        QVBoxLayout *stack0 = new QVBoxLayout;

        QWidget *dataColumn = new QWidget;
        dataColumn->setLayout(stack0);
        dataColumn->setFixedSize(300,625);

        QWidget *dataRow0 = new QWidget;
        QWidget *dataRow1 = new QWidget;
        QWidget *dataRow2 = new QWidget;
        QWidget *dataRow3 = new QWidget;
        QWidget *dataRow31 = new QWidget;
        QWidget *dataRow4 = new QWidget;
        QWidget *dataRow5 = new QWidget;
        QWidget *dataRow6 = new QWidget;

        QHBoxLayout *dataCells0 = new QHBoxLayout;
        QHBoxLayout *dataCells1 = new QHBoxLayout;
        QHBoxLayout *dataCells2 = new QHBoxLayout;
        QHBoxLayout *dataCells3 = new QHBoxLayout;
        QHBoxLayout *dataCells31 = new QHBoxLayout;
        QHBoxLayout *dataCells4 = new QHBoxLayout;
        QHBoxLayout *dataCells5 = new QHBoxLayout;
        QHBoxLayout *dataCells6 = new QHBoxLayout;

        dataRow0->setLayout(dataCells0);
        dataRow1->setLayout(dataCells1);
        dataRow2->setLayout(dataCells2);
        dataRow3->setLayout(dataCells3);
        dataRow31->setLayout(dataCells31);
        dataRow4->setLayout(dataCells4);
        dataRow5->setLayout(dataCells5);
        dataRow6->setLayout(dataCells6);

        //System Diagnostics Title
        QLabel *diagnosticsLabel = new QLabel;
        diagnosticsLabel->setText("Status & Diagnostics");
        diagnosticsLabel->setAlignment(Qt::AlignCenter);
        stack0->addWidget(diagnosticsLabel);

        //Battery Objects
        QLabel *batteriesLabel = new QLabel;
        batteriesLabel->setText("Batteries");

        QLCDNumber *batteryValue1 = new QLCDNumber;
        batteryValue1->setSegmentStyle(QLCDNumber::Filled);
        batteryValue1->setFixedSize(75,25);
        QLCDNumber *batteryValue2 = new QLCDNumber;
        batteryValue2->setSegmentStyle(QLCDNumber::Filled);
        batteryValue2->setFixedSize(75,25);


        //Motors Objects
        QLabel *motorsLabel = new QLabel;
        motorsLabel->setText("Motors");

        QLCDNumber *motorsValue1 = new QLCDNumber;
        motorsValue1->setFixedSize(75,25);
        motorsValue1->setSegmentStyle(QLCDNumber::Filled);
        QLCDNumber *motorsValue2 = new QLCDNumber;
        motorsValue2->setFixedSize(75,25);
        motorsValue2->setSegmentStyle(QLCDNumber::Filled);

        //Temperature Objects
        QLabel *temperatureLabel = new QLabel;
        temperatureLabel->setText("Temperature");

        QLCDNumber *temperatureValue = new QLCDNumber;
        temperatureValue->setFixedSize(100,25);
        temperatureValue->setSegmentStyle(QLCDNumber::Filled);

        //GPS Objects
        QLabel *gpsLabel = new QLabel;
        gpsLabel->setText("GPS");

        QLabel *gpsLat = new QLabel;
        gpsLat->setText("Latitude");
        QLabel *gpsLong = new QLabel;
        gpsLong->setText("Longitude");

        QLabel *gpsLongValue = new QLabel;
        QLabel *gpsLatValue = new QLabel;

        w->gpsLatval = "20.8261011";
        w->gpsLongval = "-156.92342416666665";

        gpsLatValue->setText(w->gpsLatval);
        gpsLongValue->setText(w->gpsLongval);


        //Gyroscope Objects
        QLabel *gyroLabel = new QLabel;
        gyroLabel->setText("Gyroscope");

        QLCDNumber *gyroValue = new QLCDNumber;
        gyroValue->setFixedSize(100,25);
        gyroValue->setSegmentStyle(QLCDNumber::Filled);
        gyroValue->display(10);

        //SADL Objects
        QLabel *sadlLabel = new QLabel;
        sadlLabel->setText("SADL Suspension Angle");

        QLCDNumber *sadlValue = new QLCDNumber;
        sadlValue->setFixedSize(100,25);
        sadlValue->setSegmentStyle(QLCDNumber::Filled);

        //Brake status
        QLabel *movementLabel = new QLabel;
        movementLabel->setText("Movement Debug");


        mainLayout->addWidget(dataColumn);

        //Stack the Elements
        stack0->addWidget(batteriesLabel);
        stack0->addWidget(dataRow0);
        stack0->addWidget(motorsLabel);
        stack0->addWidget(dataRow1);
        stack0->addWidget(temperatureLabel);
        stack0->addWidget(dataRow2);
        stack0->addWidget(gpsLabel);
        stack0->addWidget(dataRow3);
        stack0->addWidget(dataRow31);
        stack0->addWidget(gyroLabel);
        stack0->addWidget(dataRow4);
        stack0->addWidget(sadlLabel);
        stack0->addWidget(dataRow5);

        ///stack0->addWidget(dataRow6);

        // Labels for the Duo-displays
        QLabel *r0 = new QLabel;
        r0->setText("R");
        QLabel *r1 = new QLabel;
        r1->setText("R");
        QLabel *l0 = new QLabel;
        l0->setText("L");
        QLabel *l1 = new QLabel;
        l1->setText("L");

        QLabel *volts0 = new QLabel;
        volts0->setText("Volts");
        QLabel *volts1 = new QLabel;
        volts1->setText("Volts");
        QLabel *rpm0 = new QLabel;
        rpm0->setText("RPM");
        QLabel *rpm1 = new QLabel;
        rpm1->setText("RPM");
        QLabel *centigrade = new QLabel;
        centigrade->setText("Centigrade");
        QLabel *angleNote0 = new QLabel;
        angleNote0->setText("Degrees");
        QLabel *angleNote1 = new QLabel;
        angleNote1->setText("Degrees");

        // "Glue" the widgets on
        dataCells0->addWidget(l0);
        dataCells0->addWidget(batteryValue1);
        dataCells0->addWidget(volts0);
        dataCells0->addWidget(r0);
        dataCells0->addWidget(batteryValue2);
        dataCells0->addWidget(volts1);
        dataCells1->addWidget(l1);
        dataCells1->addWidget(motorsValue1);
        dataCells1->addWidget(rpm0);
        dataCells1->addWidget(r1);
        dataCells1->addWidget(motorsValue2);
        dataCells1->addWidget(rpm1);
        dataCells2->addWidget(temperatureValue);
        dataCells2->addWidget(centigrade);
        dataCells3->addWidget(gpsLat);
        dataCells3->addWidget(gpsLatValue);
        dataCells31->addWidget(gpsLong);
        dataCells31->addWidget(gpsLongValue);
        dataCells4->addWidget(gyroValue);
        dataCells4->addWidget(angleNote0);
        dataCells5->addWidget(sadlValue);
        dataCells5->addWidget(angleNote1);

        // Bottom Filler
        QWidget *bottomFiller = new QWidget;
        bottomFiller->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        stack0->addWidget(bottomFiller);

// Create Middle column
        QWidget *middleColumn = new QWidget;
        QVBoxLayout *middleStack = new QVBoxLayout;
        middleColumn->setLayout(middleStack);
        mainLayout->addWidget(middleColumn);

        QWidget *middleFiller = new QWidget;
        middleFiller->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

//Setup Connection Display

        /// Default the mIP
        w->roverIP[0] = 0;
        w->roverIP[1] = 0;
        w->roverIP[2] = 0;
        w->roverIP[3] = 0;

        // Connection Display
        QWidget *connectionColumn = new QWidget;
        QVBoxLayout *connectionStack = new QVBoxLayout;

        connectionColumn->setLayout(connectionStack);

        QLabel *connectionsLabel = new QLabel;

        connectionsLabel->setText("Connections");
        connectionsLabel->setAlignment(Qt::AlignCenter);

        connectionStack->addWidget(connectionsLabel);

        QLabel *wifiLabel = new QLabel;
        QLabel *masterIPlabel = new QLabel;
        QLabel *usersLabel = new QLabel;

        wifiLabel->setText("Wifi Strength");
        masterIPlabel->setText("Master IP");
        usersLabel->setText("Users Connected");

        QProgressBar *wifiHealth = new QProgressBar;
        wifiHealth->setValue(w->wifiStrength);

        QString mIP0 = QString::number(w->masterIP[0]);
        QString mIP1 = QString::number(w->masterIP[1]);
        QString mIP2 = QString::number(w->masterIP[2]);
        QString mIP3 = QString::number(w->masterIP[3]);

        QLabel *masterIP = new QLabel;

        masterIP->setText(mIP0 + "." + mIP1 + "." + mIP2 + "." + mIP3);

        QString users = QString::number(w->userCount);

        QLabel *usersConnected = new QLabel;
        usersConnected->setText(users);

        connectionStack->addWidget(wifiLabel);
        connectionStack->addWidget(wifiHealth);
        connectionStack->addWidget(masterIPlabel);
        connectionStack->addWidget(masterIP);
        connectionStack->addWidget(usersLabel);
        connectionStack->addWidget(usersConnected);

        middleStack->addWidget(connectionColumn);

// Setup Connections

        /// Set Rover IP to 127.0.0.1
        w->roverIP[0] = 127;
        w->roverIP[1] = 0;
        w->roverIP[2] = 0;
        w->roverIP[3] = 1;

        // Tcp Sockets setup

            // Assumed the arduino will provide 4 end points, we will provide 4 end points



            // CAN connections


// Middle Column Spacer
        middleStack->addWidget(middleFiller);

// Controls System
        /// For debug of the controls

            QLabel *upAnnounce = new QLabel;
            QLabel *downAnnounce = new QLabel;
            QLabel *leftAnnounce = new QLabel;
            QLabel *rightAnnounce = new QLabel;
            QLabel *brakeAnnounce = new QLabel;

            upAnnounce->setText("Forward Movement");
            downAnnounce->setText("Backward Movement");
            leftAnnounce->setText("Port-side Turn");
            rightAnnounce->setText("Starboard-side Turn");
            brakeAnnounce->setText("Brake On");

        //Setup User Controls

            // initialize the SADL control buttons
            QPushButton *saddleUp = new QPushButton;
            QPushButton *saddleDown = new QPushButton;

            saddleUp->setText(">");
            saddleDown->setText("<");

            saddleUp->setFixedSize(50,50);
            saddleDown->setFixedSize(50,50);

            // build widget and layouts

            QWidget *saddleWidget = new QWidget;
            QWidget *saddleGroup = new QWidget;
            QHBoxLayout *saddleLayout = new QHBoxLayout;
            QVBoxLayout *saddleGroupLayout = new QVBoxLayout;

            QLabel *saddleLabel = new QLabel;

            saddleLabel->setText("Suspension Control");
            saddleLabel->setAlignment(Qt::AlignCenter);

            saddleGroup->setLayout(saddleGroupLayout);

            saddleGroupLayout->addWidget(saddleLabel);

            saddleWidget->setLayout(saddleLayout);

            saddleGroupLayout->addWidget(saddleWidget);

            middleStack->addWidget(saddleGroup);

            saddleLayout->addWidget(saddleDown);
            saddleLayout->addWidget(saddleUp);

            saddleWidget->setFixedSize(200,125);
            saddleGroup->setFixedSize(200,125);

            // connect control keys

            QAction *saddleKeyUp = new QAction(saddleUp);
            QAction *saddleKeyDown = new QAction(saddleDown);

            saddleKeyUp->setShortcut(Qt::Key_E);
            saddleKeyDown->setShortcut(Qt::Key_Q);

            saddleUp->addAction(saddleKeyUp);
            saddleDown->addAction(saddleKeyDown);

            QObject::connect(saddleKeyUp,SIGNAL(triggered()),saddleUp,SLOT(click()));
            QObject::connect(saddleKeyDown,SIGNAL(triggered()),saddleDown,SLOT(click()));

            // Connect SADL clicks to send commands

            QObject::connect(saddleUp,SIGNAL(clicked()),w,SLOT(sendUp()));
            QObject::connect(saddleDown,SIGNAL(clicked()),w,SLOT(sendDown()));


            // initialize the directional buttons
            QPushButton *up = new QPushButton;
            QPushButton *down = new QPushButton;
            QPushButton *left = new QPushButton;
            QPushButton *right = new QPushButton;
            QPushButton *brake = new QPushButton;

            // build the buttons
            up->setText("/\\");
            down->setText("\\/");
            left->setText("<");
            right->setText(">");
            brake->setText("Brake");

            QLabel *directionsLabel = new QLabel;

            directionsLabel->setText("Directional Controls");
            directionsLabel->setAlignment(Qt::AlignCenter);

            // arts and crafts time again
            QHBoxLayout *row2 = new QHBoxLayout;
            QHBoxLayout *row3 = new QHBoxLayout;
            QVBoxLayout *stack2 = new QVBoxLayout;

            QWidget *directionsBlock = new QWidget;
            QWidget *controlTop = new QWidget;
            QWidget *controlBottom = new QWidget;

            directionsBlock->setFixedSize(200,200);
            controlTop->setFixedHeight(100);
            controlBottom->setFixedHeight(100);
            up->setFixedSize(50,50);
            down->setFixedSize(50,50);
            left->setFixedSize(50,50);
            right->setFixedSize(50,50);
            brake->setFixedSize(200,50);

            controlTop->setLayout(row2);
            controlBottom->setLayout(row3);
            directionsBlock->setLayout(stack2);
            stack2->addWidget(directionsLabel);
            stack2->addWidget(controlTop);
            stack2->addWidget(controlBottom);


            // tape the buttons to the fancy board
            row2->addWidget(up);
            row3->addWidget(left);
            row3->addWidget(down);
            row3->addWidget(right);

            // final resting place of the directions block
            middleStack->addWidget(directionsBlock);

            middleStack->addWidget(brake);

            // set directional control keys
            QAction *keyUp = new QAction(up);
            QAction *keyDown = new QAction(down);
            QAction *keyLeft = new QAction(left);
            QAction *keyRight = new QAction(right);
            QAction *keyBrake = new QAction(brake);

            keyUp->setShortcut(Qt::Key_W);
            keyDown->setShortcut(Qt::Key_S);
            keyLeft->setShortcut(Qt::Key_A);
            keyRight->setShortcut(Qt::Key_D);
            keyBrake->setShortcut(Qt::Key_Space);

            up->addAction(keyUp);
            down->addAction(keyDown);
            left->addAction(keyLeft);
            right->addAction(keyRight);
            brake->addAction(keyBrake);

            // connect control keys
            QObject::connect(keyUp,SIGNAL(triggered()),up,SLOT(click()));
            QObject::connect(keyDown,SIGNAL(triggered()),down,SLOT(click()));
            QObject::connect(keyLeft,SIGNAL(triggered()),left,SLOT(click()));
            QObject::connect(keyRight,SIGNAL(triggered()),right,SLOT(click()));
            QObject::connect(keyBrake,SIGNAL(triggered()),brake,SLOT(click()));

            // Connect Clicks to send commands

            QObject::connect(up,SIGNAL(clicked()),w,SLOT(sendForward()));
            QObject::connect(down,SIGNAL(clicked()),w,SLOT(sendReverse()));
            QObject::connect(left,SIGNAL(clicked()),w,SLOT(sendLeft()));
            QObject::connect(right,SIGNAL(clicked()),w,SLOT(sendRight()));
            QObject::connect(brake,SIGNAL(clicked()),w,SLOT(sendBrake()));

    //Setup camera view containers

        //Camera Quadrant Layout
        QHBoxLayout *row0 = new QHBoxLayout;
        QHBoxLayout *row1 = new QHBoxLayout;
        QVBoxLayout *stack1 = new QVBoxLayout;

        QVBoxLayout *camera0Group = new QVBoxLayout;
        QVBoxLayout *camera1Group = new QVBoxLayout;
        QVBoxLayout *camera2Group = new QVBoxLayout;
        QVBoxLayout *camera3Group = new QVBoxLayout;

        QWidget *cameraQuad = new QWidget;
        cameraQuad->setFixedSize(600,600);
        cameraQuad->setLayout(stack1);

        QWidget *quadRow0 = new QWidget;
        quadRow0->setFixedSize(600,300);
        quadRow0->setLayout(row0);

        QWidget *quadRow1 = new QWidget;
        quadRow1->setFixedSize(600,300);
        quadRow1->setLayout(row1);

        QWidget *cam0Group = new QWidget;
        QWidget *cam1Group = new QWidget;
        QWidget *cam2Group = new QWidget;
        QWidget *cam3Group = new QWidget;

        cam0Group->setLayout(camera0Group);
        cam1Group->setLayout(camera1Group);
        cam2Group->setLayout(camera2Group);
        cam3Group->setLayout(camera3Group);

        stack1->addWidget(quadRow0);
        stack1->addWidget(quadRow1);

        mainLayout->addWidget(cameraQuad);

        row0->addWidget(cam0Group);
        row0->addWidget(cam1Group);
        row1->addWidget(cam2Group);
        row1->addWidget(cam3Group);

        QLabel *camera0Label = new QLabel;
        QLabel *camera1Label = new QLabel;
        QLabel *camera2Label = new QLabel;
        QLabel *camera3Label = new QLabel;

        camera0Label->setText("Mast Left");
        camera1Label->setText("Mast Right");
        camera2Label->setText("Aerial View");
        camera3Label->setText("Rear View");

// Camera Disconnected Widgets
        //Camera 0 Container
        QGraphicsView *view0 = new QGraphicsView;
        QGraphicsScene *scene0 = new QGraphicsScene;
        view0->setFixedSize(275,275);
        view0->setScene(scene0);
        camera0Group->addWidget(camera0Label);
        camera0Group->addWidget(view0);
        view0->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view0->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


        //Camera 1 Container
        QGraphicsView *view1 = new QGraphicsView;
        QGraphicsScene *scene1 = new QGraphicsScene;
        view1->setFixedSize(275,275);
        view1->setScene(scene1);
        camera1Group->addWidget(camera1Label);
        camera1Group->addWidget(view1);
        view1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        //Camera 2 Container
        QGraphicsView *view2 = new QGraphicsView;
        QGraphicsScene *scene2 = new QGraphicsScene;
        view2->setFixedSize(275,275);
        view2->setScene(scene2);
        camera2Group->addWidget(camera2Label);
        camera2Group->addWidget(view2);
        view2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        //Camera 3 Container - To be used as the Aerial overhead Display
        QGraphicsView *view3 = new QGraphicsView;
        QGraphicsScene *scene3 = new QGraphicsScene;
        view3->setFixedSize(275,275);
        view3->setScene(scene3);
        camera3Group->addWidget(camera3Label);
        camera3Group->addWidget(view3);
        view3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Camera Disconnected Screens
        QGraphicsScene *cameraDisconnected = new QGraphicsScene;
        cameraDisconnected->addPixmap(QPixmap("/home/besalt/roverCommandUI/roverCommandUI/images/notConnected"));
        /// Change the file paths according to your system
        QGraphicsScene *cameraError = new QGraphicsScene;
        cameraError->addPixmap(QPixmap("/home/besalt/roverCommandUI/roverCommandUI/images/error"));

    // Camera Feeds Default State
        view0->setScene(cameraDisconnected);
        view1->setScene(cameraDisconnected);
        view2->setScene(cameraDisconnected);
        view3->setScene(cameraDisconnected);

        w->getViewIDs(view0->winId(),view1->winId(),view2->winId(),view3->winId());

//Connect Telemetry Variables

        // Set Temp values
        w->acquireTelemetry();

        // Display Values - This needs to be threaded
        batteryValue1->display(w->batt1);
        batteryValue2->display(w->batt2);
        motorsValue1->display(w->motors1);
        motorsValue2->display(w->motors2);
        temperatureValue->display(w->temp);
        gyroValue->display(w->gyroAng);
        sadlValue->display(w->sadlAng);

        /// Debug Display
        ///
        displayValue(motorsValue1,1337);

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
        w->show();///////// Pour the Champagne ///////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

    return a.exec();
}
