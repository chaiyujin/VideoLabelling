#pragma once
#include "../misc.h"
#include "landmark.h"
#include "../utils/ImageProcess.h"
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QVector>
#include <algorithm>


class Project {
public:
	Project();
	~Project();

	bool openVideo(const QString &videoPath);
	bool changeFrame(int frameId);
	const QImage &currentFrame() const { return m_qImage; }

	int lastIndex() const { return m_lastIndex; }
	bool isOpened() const { return m_isOpened; }
	int frameCount() const { return m_capture.get(CV_CAP_PROP_FRAME_COUNT); }
	
	// landmark
	void clearSelected() { selected.clear(); selected.fill(0, Landmark::g_count); }
	int selectPoint(float x, float y, const QRectF &realRect, const QRectF &canvasRect);
	void selectPoint(const QRectF &selectRect, const QRectF &realRect, const QRectF &canvasRect);
	void movePoint(float deltaX, float deltaY, const QRectF &realRect, const QRectF &canvasRect);
	void movePointTo(float x, float y, const QRectF &realRect, const QRectF &canvasRect);
	void usePreviousFrame(QString mode);
	void saveFrameAt(int index, QString path);

	void resetLandmark(int i=-1) { 
		if (i < 0) i = m_lastIndex;
		new_landmarks[i] = old_landmarks[i];
	}
	Landmark &getOldLandmark(int i = -1) { if (i < 0) i = m_lastIndex; return old_landmarks[i]; }
	Landmark &getNewLandmark(int i = -1) { if (i < 0) i = m_lastIndex; return new_landmarks[i]; }
	const Landmark &getOldLandmark(int i = -1) const { if (i < 0) i = m_lastIndex; return old_landmarks[i]; }
	const Landmark &getNewLandmark(int i = -1) const { if (i < 0) i = m_lastIndex; return new_landmarks[i]; }
	Landmark getCanvasLandmark(const QRectF &realRect, const QRectF &canvasRect) const;
	void setCanvasLandmark(const Landmark &lm, const QRectF &realRect, const QRectF &canvasRect);
	bool isSelected(int index) const {
		if (index >= 0 && index < selected.size()) {
			return selected[index];
		}
		return false;
	}
	bool isModified(int index = -1) const {
		if (index < 0) index = m_lastIndex;
		return modified[index];
	}
	bool save(QString path="", bool force=false);
	bool load(QString path="");

	friend QDataStream &operator >> (QDataStream &, Project &);
	friend QDataStream &operator << (QDataStream &, const Project &);

	// stamps and valid
	int nextStamp() {
		For(i, m_stamps.size() - 1) {
			if (m_stamps[i] <= m_lastIndex && m_stamps[i + 1] > m_lastIndex) {
				return m_stamps[i + 1];
			}
		}
		return m_lastIndex;
	}
	int prevStamp() {
		For(i, m_stamps.size() - 1) {
			if (m_stamps[i] < m_lastIndex && m_stamps[i + 1] >= m_lastIndex) {
				return m_stamps[i];
			}
		}
		return m_lastIndex;
	}
	void appendStamp(int index = -1) {
		if (index < 0) index = m_lastIndex;
		if (!m_stamps.contains(index)) {
			m_stamps.append(index);
			std::sort(m_stamps.begin(), m_stamps.end());
		}
	}
	void removeStamp(int index = -1) {
		if (index < 0) index = m_lastIndex;
		m_stamps.removeOne(index);
	}
	bool isValid(int index=-1) const {
		if (index < 0) index = m_lastIndex;
		for (int i = 0; i < m_stamps.size(); i += 2) {
			if (i + 1 >= m_stamps.size()) continue;
			if (index >= m_stamps[i] && index < m_stamps[i + 1]) return true;
		}
		return false;
	}

	const QVector<qint32> &getStamps() const { return m_stamps; }

private:
	bool m_saved;
	QString m_videoPath;
	QString m_oldLMPath;
	QString m_newLMPath;
	QString m_projectPath;
	cv::VideoCapture m_capture;
	int m_lastIndex;
	bool m_isOpened;
	QImage m_qImage;
	cv::Mat m_frameImage;

	QVector<int> selected;
	QVector<Landmark> old_landmarks;
	QVector<Landmark> new_landmarks;
	QVector<qint8>    modified;
	QVector<qint32>	  m_stamps;

	bool readFromLMSFile(QString);
	void fillWithZero();
};

QDataStream &operator >> (QDataStream &, Project &);
QDataStream &operator << (QDataStream &, const Project &);