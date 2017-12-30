#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "misc.h"
#include "marks/landmark.h"
#include "marks/Project.h"
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openVideo();
    void exportFrames();
	void exportStamps();
	void importStamps();
	void changeFrame(int index, bool force=false);
	void changeFrameStep(const QString &);

private:
    Ui::MainWindow *ui;

	// input
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void closeEvent(QCloseEvent * event);

	// ui
	void paintEvent(QPaintEvent *event);

    void message(const char *msg);
    void message(const QString &msg);
    void hideVideoController();
    void showVideoController();

	// file
    void openVideoFromPath(QString &path);
    void exportFramesToPath(QString &path);

	Project m_project;
	bool m_ctrl, m_alt, m_previous;
	QSize m_imageSize;
	QRectF m_canvasRect, m_realRect;
	QRectF m_smallRect;
	SelectRectF m_selectRect;
};

#endif // MAINWINDOW_H
