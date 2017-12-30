/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_video;
    QAction *actionExport_labeled_frames;
    QAction *actionExport_stamps;
    QAction *actionImport_stamps;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QWidget *centerWidget;
    QGridLayout *gridLayout;
    QComboBox *comboBox;
    QSlider *horizontalSlider;
    QLabel *label;
    QLabel *statusLabel;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(778, 489);
        actionOpen_video = new QAction(MainWindow);
        actionOpen_video->setObjectName(QStringLiteral("actionOpen_video"));
        actionExport_labeled_frames = new QAction(MainWindow);
        actionExport_labeled_frames->setObjectName(QStringLiteral("actionExport_labeled_frames"));
        actionExport_stamps = new QAction(MainWindow);
        actionExport_stamps->setObjectName(QStringLiteral("actionExport_stamps"));
        actionImport_stamps = new QAction(MainWindow);
        actionImport_stamps->setObjectName(QStringLiteral("actionImport_stamps"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        centerWidget = new QWidget(centralWidget);
        centerWidget->setObjectName(QStringLiteral("centerWidget"));

        gridLayout_2->addWidget(centerWidget, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setMinimumSize(QSize(0, 38));
        comboBox->setMaximumSize(QSize(16777215, 38));
        comboBox->setStyleSheet(QLatin1String("QComboBox {\n"
"	background: rgb(25, 38, 58);\n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"QComboBox::drop-down\n"
"{\n"
"     subcontrol-origin: padding;\n"
"     subcontrol-position: top right;\n"
"     width: 15px;\n"
"     color: white;\n"
"     border-left-width: 0px;\n"
"     border-left-color: darkgray;\n"
"     border-left-style: solid; /* just a single line */\n"
"     border-top-right-radius: 3px; /* same radius as the QComboBox */\n"
"     border-bottom-right-radius: 3px;\n"
"     padding-left: 10px;\n"
" }"));

        gridLayout->addWidget(comboBox, 0, 0, 2, 1);

        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMaximumSize(QSize(16777215, 38));
        horizontalSlider->setStyleSheet(QLatin1String("QSlider {\n"
"	background-color: rgb(25, 38, 58);\n"
"	border: 8px solid rgb(25, 38, 58);\n"
"}\n"
"QSlider::sub-page:horizontal {\n"
"	background-color: rgb(37, 168, 198);\n"
"	height: 2px;width:4px;\n"
"}\n"
"QSlider::add-page:horizontal {\n"
"	background-color: rgb(87, 97, 106);\n"
"	height: 2px;width:4px;\n"
"}\n"
"QSlider::groove:horizontal {\n"
"    background:transparent;  \n"
"    height:4px;\n"
"}\n"
"QSlider::handle:horizontal {\n"
"    height: 10px;\n"
"    width: 10px ;\n"
"    margin: -3 0px;\n"
"}\n"
""));
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 0, 1, 2, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(16777215, 19));
        label->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"	background-color: rgb(25, 38, 58);"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 2, 1, 1);

        statusLabel = new QLabel(centralWidget);
        statusLabel->setObjectName(QStringLiteral("statusLabel"));
        statusLabel->setMaximumSize(QSize(16777215, 19));
        statusLabel->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"	background-color: rgb(25, 38, 58);"));
        statusLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(statusLabel, 1, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 778, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setStyleSheet(QLatin1String("background-color: rgb(146, 77, 194);\n"
"color: rgb(255, 255, 255);"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_video);
        menuFile->addAction(actionExport_labeled_frames);
        menuFile->addAction(actionImport_stamps);
        menuFile->addAction(actionExport_stamps);

        retranslateUi(MainWindow);

        comboBox->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionOpen_video->setText(QApplication::translate("MainWindow", "Open video", Q_NULLPTR));
        actionExport_labeled_frames->setText(QApplication::translate("MainWindow", "Export labeled frames", Q_NULLPTR));
        actionExport_stamps->setText(QApplication::translate("MainWindow", "Export stamps", Q_NULLPTR));
        actionImport_stamps->setText(QApplication::translate("MainWindow", "Import stamps", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "step1", Q_NULLPTR)
         << QApplication::translate("MainWindow", "step25", Q_NULLPTR)
         << QApplication::translate("MainWindow", "step50", Q_NULLPTR)
         << QApplication::translate("MainWindow", "step100", Q_NULLPTR)
         << QApplication::translate("MainWindow", "step150", Q_NULLPTR)
        );
        comboBox->setCurrentText(QApplication::translate("MainWindow", "step100", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        statusLabel->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
