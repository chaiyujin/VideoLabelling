#include "Project.h"
#include "../utils/MyString.h"
#include "../utils/ImageProcess.h"
#include <QFile>
#include <fstream>
#include <QFileInfo>

Project::Project() : m_isOpened(false), m_lastIndex(0), m_saved(false) {}

Project::~Project() {}

bool Project::openVideo(const QString &videoPath) {
	m_isOpened = false;
	MyString filePath(videoPath.toStdString());
	auto list = filePath.split('.');
	list[list.size() - 1] = "stps";     m_stampsPath = QString(MyString(".").join(list).c_str());
	list[list.size() - 1] = "prj";		m_projectPath = QString(MyString(".").join(list).c_str());
	list[list.size() - 1] = "lms";		m_oldLMPath = QString(MyString(".").join(list).c_str());
	list[list.size() - 2] += "_new";    m_newLMPath = QString(MyString(".").join(list).c_str());
	new (&m_capture) cv::VideoCapture(videoPath.toStdString());
	{
		// reset
		old_landmarks.clear();
		new_landmarks.clear();
	}
#ifdef DEBUG_LOG
	qDebug() << m_projectPath << " " << m_oldLMPath << " " << m_newLMPath;
	if (!m_capture.isOpened()) { qDebug() << "Fail to open video."; system("pause"); }
	else { qDebug() << "Succeed to open video."; }
#endif
	if (m_capture.isOpened()) {
		// successfully opened
		m_isOpened = true;
		if (!load()) {
			m_isOpened &= readFromLMSFile(m_oldLMPath);
			fillWithZero();
		}
		importStamps(m_stampsPath);
	}

	return m_isOpened;
}

bool Project::changeFrame(int frameId) {
	{
		// change capture frame
		m_capture.set(CV_CAP_PROP_POS_FRAMES, frameId);
		m_capture >> m_frameImage;
		m_qImage = ImageProcess::Mat2QImage(m_frameImage);
	}
	{
		// change project information
		m_lastIndex = frameId;
	}
	return true;
}

bool Project::readFromLMSFile(QString path) {
	std::ifstream fin(path.toStdString());
	if (fin.is_open()) {
		// read line by line
		Landmark lm;
		while (lm.loadFromFStream(fin)) {
			old_landmarks.append(lm);
			new_landmarks.append(lm);
			modified.append(0);
		}
		fin.close();
		return true;
	}
	m_saved = false;
	return false;
}

void Project::fillWithZero() {
	int count = frameCount();
	Landmark lm;
	lm.fillZero();
	while (old_landmarks.size() < count) {
		old_landmarks.append(lm);
		new_landmarks.append(lm);
		modified.append(0);
	}
	m_saved = false;
}

bool Project::save(QString path, bool force) {
	if (m_saved && !force) return false;
	if (path.isEmpty()) path = m_projectPath;
	QFile file(path);
	file.open(QIODevice::WriteOnly);
	exportStamps(m_stampsPath);
	if (file.isOpen()) {
		if (force) { old_landmarks = new_landmarks; }
		if (force) {  }
		QDataStream out(&file);
		out << (*this);
		file.close();
		return true;
		m_saved = true;
	}
	return false;
}

bool Project::load(QString path) {
	if (path.isEmpty()) path = m_projectPath;
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	if (file.isOpen()) {
		QDataStream in(&file);
		in >> (*this);
		file.close();
		return true;
	}
	return false;
}

void Project::usePreviousFrame(QString mode) {
	if (m_lastIndex > 0) {
		clearSelected();
		int pi = m_lastIndex - 1;
		int ni = m_lastIndex;
		For(i, Landmark::g_count) {
			if (mode == "A") { new_landmarks[ni][i] = new_landmarks[pi][i]; selected[i] = 1; }
			if (mode == "F" && !Landmark::g_mouthMask[i]) { new_landmarks[ni][i] = new_landmarks[pi][i]; selected[i] = 1; }
			if (mode == "M" && Landmark::g_mouthMask[i])  { new_landmarks[ni][i] = new_landmarks[pi][i]; selected[i] = 1; }
		}
		m_saved = false;
	}
}

Landmark Project::getCanvasLandmark(const QRectF &realRect, const QRectF &canvasRect) const {
	Landmark lm = new_landmarks[m_lastIndex];
	For(i, lm.size()) {
		Point<qreal> &p = lm[i];
		p.x = (p.x - realRect.left()) / realRect.width() * canvasRect.width() + canvasRect.left();
		p.y = (p.y - realRect.top()) / realRect.height() * canvasRect.height() + canvasRect.top();
	}
	return lm;
}

void Project::setCanvasLandmark(const Landmark &lm, const QRectF &realRect, const QRectF &canvasRect) {
	Landmark &cur = new_landmarks[m_lastIndex];
	For(i, lm.size()) {
		cur[i].x = (lm[i].x - canvasRect.left()) * realRect.width() / canvasRect.width() + realRect.left();
		cur[i].y = (lm[i].y - canvasRect.top()) * realRect.height() / canvasRect.height() + realRect.top();
	}
	modified[m_lastIndex] = 1;
	m_saved = false;
}

int Project::selectPoint(float x, float y, const QRectF &realRect, const QRectF &canvasRect) {
	int ret = -1;
	clearSelected();
	Landmark lm = getCanvasLandmark(realRect, canvasRect);
	For(i, lm.size()) {
		auto &p = lm[i];
		if (p.distanceTo(x, y) < 5) {
			selected[i] = 1;
			ret = i;
			break;
		}
	}
	return ret;
}

void Project::selectPoint(const QRectF &selectRect, const QRectF &realRect, const QRectF &canvasRect) {
	clearSelected();
	Landmark lm = getCanvasLandmark(realRect, canvasRect);
	For(i, lm.size()) {
		auto &p = lm[i];
		selected[i] = selectRect.contains(QPointF(p.x, p.y));
	}
}

void Project::movePoint(float deltaX, float deltaY, const QRectF &realRect, const QRectF &canvasRect) {
	bool changed = false;
	Landmark lm = getCanvasLandmark(realRect, canvasRect);
	For(i, lm.size()) {
		if (!selected[i]) continue;
		changed = true;
		lm[i].x += deltaX;
		lm[i].y += deltaY;
	}
	if (changed) setCanvasLandmark(lm, realRect, canvasRect);
}

void Project::movePointTo(float x, float y, const QRectF &realRect, const QRectF &canvasRect) {
	bool changed = false;
	Landmark lm = getCanvasLandmark(realRect, canvasRect);
	For(i, lm.size()) {
		if (!selected[i]) continue;
		changed = true;
		lm[i].x = x;
		lm[i].y = y;
	}
	if (changed) setCanvasLandmark(lm, realRect, canvasRect);
}

void Project::saveFrameAt(int index, QString path) {
	if (ValidIndex(index, new_landmarks.size())) {
		m_capture.set(CV_CAP_PROP_POS_FRAMES, index);
		m_capture >> m_frameImage;
		cv::imwrite(QString(path + ".jpg").toStdString(), m_frameImage);
		/*int i = 1;
		QString prefix(path);
		QFileInfo info(prefix + ".pts");
		while (info.exists()) {
			path = prefix + QString::number(i++);
			new (&info) QFileInfo(path + ".pts");
		}*/
		path += ".pts";
		new_landmarks[index].exportPts(path.toStdString());
	}
}

QDataStream &operator >> (QDataStream &stream, Project &pro) {
	stream >> pro.old_landmarks;
	stream >> pro.new_landmarks;
	stream >> pro.modified;
	stream >> pro.m_stamps;
	qDebug() << pro.m_stamps.size();
	return stream;
}

QDataStream &operator << (QDataStream &stream, const Project &pro) {
	stream << pro.old_landmarks;
	stream << pro.new_landmarks;
	stream << pro.modified;
	stream << pro.m_stamps;
	qDebug() << pro.m_stamps.size();
	return stream;
}

void Project::exportStamps(QString path) {
	ofstream fout(path.toStdString());
	if (fout.is_open()) {
		fout << m_stamps.size() << "\n";
		For(i, m_stamps.size()) fout << m_stamps[i] << " ";
		fout.close();
	}
}

void Project::importStamps(QString path) {
	ifstream fin(path.toStdString());
	if (fin.is_open()) {
		m_stamps.clear();
		int size;
		fin >> size;
		For(i, size) {
			int x;
			fin >> x;
			m_stamps.append(x);
		}
		fin.close();
	}
}
