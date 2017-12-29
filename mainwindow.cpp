#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "marks/landmark.h"
#include "utils/ImageProcess.h"
#include "utils/MyString.h"
#include <QPainter>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	m_ctrl(false), m_alt(false), m_previous(false)
{
    ui->setupUi(this);

    {
        // init window
        hideVideoController();
        message("Please choose video.");
    }
    {
        // connect signals
        connect(ui->actionOpen_video,            SIGNAL(triggered()), this, SLOT(openVideo()));
        connect(ui->actionExport_labeled_frames, SIGNAL(triggered()), this, SLOT(exportFrames()));
		connect(ui->horizontalSlider,			 SIGNAL(valueChanged(int)), this, SLOT(changeFrame(int)));
		connect(ui->comboBox,					 SIGNAL(activated(const QString &)), this, SLOT(changeFrameStep(const QString &)));
		changeFrameStep(ui->comboBox->currentText());
    }
    {
		// init ui
		m_smallRect.setLeft(0);  m_smallRect.setTop(0);
		m_smallRect.setWidth(1); m_smallRect.setHeight(1);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* slots */
void MainWindow::openVideo() {
    QString fileName = QFileDialog::getOpenFileName(
            this, tr("Open video"),
            "C:/Users/Public/Videos",
            tr("Videoes (*.mpg *.mov *.mp4 *.avi)"));
    if (!fileName.isEmpty()) {
        openVideoFromPath(fileName);
    }
}

void MainWindow::exportFrames() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "C:/Users/Public/Documents",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        exportFramesToPath(dir);
    }
}

/* main functions */

void MainWindow::openVideoFromPath(QString &path) {
	// open a video
	bool success = false;
	{
		// build a project
		if (m_project.openVideo(path)) {
			message(QString("Opened video: ") + path);
			success = true;
			changeFrame(0, true); // init at start
		}
		else {
			message(QString("Fail to open video from: ") + path);
		}
	}
	if (success) {
		// if successfully load
		setWindowTitle(path);
		showVideoController();
	}
}

void MainWindow::exportFramesToPath(QString &path) {
	QString dir = path;
	if (LastChar(dir) != '\\' && LastChar(dir) != '/') dir += '/';
	For(i, m_project.frameCount()) {
		if (m_project.isModified(i))
			m_project.saveFrameAt(i, dir + QString::number(i));
	}
}

/* input functions */
void MainWindow::keyPressEvent(QKeyEvent *event) {
	if (!m_project.isOpened()) return;
	{
		// selecting, conflict with each other
		if (event->key() == Qt::Key_Control && !m_alt && !m_previous) {
			m_ctrl = true;
			message("ctrl + ...");
		}
		if (event->key() == Qt::Key_Alt && !m_ctrl && !m_previous) {
			m_alt = true;
			message("alt + ...");
		}
		if (event->key() == Qt::Key_P && !m_ctrl && !m_alt) {
			m_previous = true;
			message("Using previous frame.");
		}
	}
	{
		if (event->key() == Qt::Key_I) { m_project.movePoint( 0, -1, m_realRect, m_canvasRect); }
		if (event->key() == Qt::Key_K) { m_project.movePoint( 0,  1, m_realRect, m_canvasRect); }
		if (event->key() == Qt::Key_J) { m_project.movePoint(-1,  0, m_realRect, m_canvasRect); }
		if (event->key() == Qt::Key_L) { m_project.movePoint( 1,  0, m_realRect, m_canvasRect); }
	}
	{
		if (event->key() == Qt::Key_Z) {
			if (m_ctrl) {
				// reset frame
				m_project.resetLandmark();
				message(QString("Reset frame ") + QString::number(m_project.lastIndex()));
			}
		}
		else if (event->key() == Qt::Key_S) {
			if (m_ctrl) {
				if (m_project.save()) message("Saved");
			}
		}
	}
	update();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
	if (!m_project.isOpened()) return;
	{
		if (event->key() == Qt::Key_Control && m_ctrl) {
			m_ctrl = false; message("");
		}
		if (event->key() == Qt::Key_Alt && m_alt) {
			m_alt = false; message("");
		}
		if (event->key() == Qt::Key_P && m_previous) {
			m_previous = false; message("");
		}
	}
	if (event->key() == Qt::Key_0) {
		if (m_ctrl) {
			// reset smallRect
			m_smallRect.setLeft(0);  m_smallRect.setTop(0);
			m_smallRect.setWidth(1); m_smallRect.setHeight(1);
		}
	}
	else if (event->key() == Qt::Key_Equal) {
		if (m_ctrl) {
			m_project.appendStamp();
		}
	}
	else if (event->key() == Qt::Key_Minus) {
		if (m_ctrl) {
			m_project.removeStamp();
		}
	}
	else if (event->key() == ']') {
		if (m_ctrl) {
			changeFrame(m_project.nextStamp());
		}
	}
	else if (event->key() == '[') {
		if (m_ctrl) {
			changeFrame(m_project.prevStamp());
		}
	}
	else if (event->key() == Qt::Key_A) {
		if (m_previous) m_project.usePreviousFrame("A");
	}
	else if (event->key() == Qt::Key_M) {
		if (m_previous) m_project.usePreviousFrame("M");
	}
	else if (event->key() == Qt::Key_F) {
		if (m_previous) m_project.usePreviousFrame("F");
	}
	if (event->key() == Qt::Key_Escape) {
		m_project.clearSelected();
	}
	update();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
	if (!m_project.isOpened()) return;
	if (m_ctrl || m_alt) {
		// control mode
		m_selectRect.startPoint(event->x(), event->y());
		m_selectRect.boundWith(m_canvasRect);
		message("Selecting");
	}
	else {
		m_selectRect.reset();
		// point move mode
		int index = m_project.selectPoint(event->x(), event->y(), m_realRect, m_canvasRect);
		if (index >= 0) message(QString("Pick point ") + QString::number(index));
		else message("");
	}
	update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
	if (!m_project.isOpened()) return;
	if (m_ctrl || m_alt) {
		// control mode
		m_selectRect.newCorner(event->x(), event->y());
		m_selectRect.boundWith(m_canvasRect);
		if (m_ctrl) message("Selecting smaller ares...");
		if (m_alt)  message("Selecting points...");
	}
	else {
		m_selectRect.reset();
		// point move mode
		if (m_canvasRect.contains(QPointF(event->x(), event->y())))
			m_project.movePointTo(event->x(), event->y(), m_realRect, m_canvasRect);
	}
	update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
	if (!m_project.isOpened()) return;
	if (m_ctrl || m_alt) {
		// control mode
		m_selectRect.newCorner(event->x(), event->y());
		m_selectRect.boundWith(m_canvasRect);
		if (m_alt) {
			// select points
			QRectF cur(m_selectRect);
			m_project.selectPoint(cur, m_realRect, m_canvasRect);
		}
		else if (m_ctrl) {
			// change smallRect
			// 1 get current percentage
			QRectF cur(m_selectRect);
			float new_w = cur.width() * m_realRect.width() / m_canvasRect.width();
			float new_h = cur.height() * m_realRect.height() / m_canvasRect.height();
			float new_l = (cur.left() - m_canvasRect.left())  * m_realRect.width() / m_canvasRect.width() + m_realRect.left();
			float new_t = (cur.top() - m_canvasRect.top()) * m_realRect.height() / m_canvasRect.height() + m_realRect.top();
			m_smallRect.setLeft(new_l / m_imageSize.width());
			m_smallRect.setTop(new_t / m_imageSize.height());
			m_smallRect.setWidth(new_w / m_imageSize.width());
			m_smallRect.setHeight(new_h / m_imageSize.height());
		}
		// finally clear select rect
		m_selectRect.reset();
	}
	else {
		m_selectRect.reset();
		// point move mode
		if (m_canvasRect.contains(QPointF(event->x(), event->y())))
			m_project.movePointTo(event->x(), event->y(), m_realRect, m_canvasRect);
	}
	update();
}

/* ui functions */

void MainWindow::paintEvent(QPaintEvent *event) {
	if (!m_project.isOpened()) return;
	// the project is opened
	QPainter painter(this);
	const QImage &image = m_project.currentFrame();
	m_imageSize = image.size();
	m_imageSize.setWidth(m_imageSize.width() * m_smallRect.width());
	m_imageSize.setHeight(m_imageSize.height() * m_smallRect.height());
	m_canvasRect = ImageProcess::calcValidTargetRect(m_imageSize, this->size());
	m_imageSize = image.size();
	new (&m_realRect) QRectF(
		image.size().width()  * m_smallRect.left(),
		image.size().height() * m_smallRect.top(),
		image.size().width()  * m_smallRect.width(),
		image.size().height() * m_smallRect.height()
	);
	painter.drawImage(m_canvasRect, image, m_realRect);
	if (m_selectRect.isValid()) {
		// has a select rect
		if (m_ctrl)	    painter.setPen(QPen(Qt::blue, 4, Qt::DashLine));
		else if (m_alt) painter.setPen(QPen(Qt::green, 4, Qt::DashLine));
		painter.drawRect(m_selectRect);
	}
	{
		painter.setPen(QPen(Qt::white, Qt::SolidLine));
		painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
		// draw landmarks
		Landmark lm = m_project.getCanvasLandmark(m_realRect, m_canvasRect);
		For(i, lm.size()) {
			const auto &p = lm[i];
			if (m_project.isSelected(i)) { painter.setPen(QPen(Qt::green, Qt::SolidLine)); painter.setBrush(QBrush(Qt::green, Qt::SolidPattern)); }
			painter.drawEllipse(QPoint(p.x, p.y), 1, 1);
			if (m_project.isSelected(i)) { painter.setPen(QPen(Qt::white, Qt::SolidLine)); painter.setBrush(QBrush(Qt::white, Qt::SolidPattern)); }
		}
	}
	{
		QBrush invalidBrush = QBrush(QColor(87, 97, 106), Qt::SolidPattern);
		QBrush validBrush = QBrush(QColor(37, 168, 198), Qt::SolidPattern);
		painter.setPen(QPen(QColor(87, 97, 106), Qt::SolidLine));
		int width = geometry().width() - 20;
		int height = 10;
		int left = 10, top = 20;
		// draw valid bar
		int maxCount = m_project.frameCount();
		const auto &stamps = m_project.getStamps();
		painter.setBrush(invalidBrush);
		painter.drawRect(QRectF(left, top, width, height));
		painter.setBrush(validBrush);
		for (int i = 0; i < stamps.size() - 1; i += 2) {
			int p0 = (stamps[i] * width / (float)maxCount) + left;
			int p1 = (stamps[i + 1] * width / (float)maxCount) + left;
			painter.drawRect(QRectF(p0, top, p1 - p0, height));
		}
		For(i, stamps.size()) {
			int pos = (stamps[i] * width / (float)maxCount) + left;
			painter.drawEllipse(QPoint(pos, 25), 2, 2);
		}
		int cur = (m_project.lastIndex() * width / (float)maxCount) + left;
		painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
		painter.drawEllipse(QPoint(cur, 25), 3, 3);
		if (m_project.isValid()) ui->statusBar->setStyleSheet("background-color: rgb(37, 168, 198);");
		else ui->statusBar->setStyleSheet("background-color: rgb(87, 97, 106);");
	}
	{
		QString status = "raw";
		if (m_project.isModified()) status = "modified";
		ui->statusLabel->setText(status);
	}
}

void MainWindow::changeFrame(int index, bool force) {
	if (m_project.isOpened()) {
		if (index == m_project.lastIndex() && !force) return;
		// round index
		int reminder = index % ui->horizontalSlider->singleStep();
		if (reminder != 0) {
			// round
			if (reminder <= ui->horizontalSlider->singleStep() / 2) { index -= reminder; }
			else { index += ui->horizontalSlider->singleStep() - reminder; }
		}
		if (index >= m_project.frameCount()) index = m_project.frameCount() - 1;
		if (m_project.changeFrame(index)) {
			{
				// change ui
				ui->horizontalSlider->setValue(index);
				auto frameLabel = MyString::divideString(index, m_project.frameCount() - 1) + " ";
				ui->label->setText(frameLabel.c_str());
			}
			message(QString("Frame ") + QString::number(index));
			update();
		}
	}
}

void MainWindow::changeFrameStep(const QString &stepStr) {
	int step = MyString::string2int(stepStr.toStdString());
	ui->horizontalSlider->setSingleStep(step);
	ui->horizontalSlider->setFocus();
	changeFrame(ui->horizontalSlider->value(), true);
}

// show message
void MainWindow::message(const char *msg) { ui->statusBar->showMessage(msg); }
void MainWindow::message(const QString &msg) { ui->statusBar->showMessage(msg); }

// hide the controller
void MainWindow::hideVideoController(){
    ui->label->hide();
    ui->statusLabel->hide();
    ui->horizontalSlider->hide();
    ui->comboBox->hide();
}

// show the controller
void MainWindow::showVideoController() {
    ui->label->show();
    ui->statusLabel->show();
    ui->horizontalSlider->show();
    ui->comboBox->show();
	ui->horizontalSlider->setFocus();
	int maxCount = m_project.frameCount();
	ui->horizontalSlider->setMaximum(maxCount - 1);
}

void MainWindow::closeEvent(QCloseEvent * event) {
	m_project.save("", true);
	event->accept();
}
