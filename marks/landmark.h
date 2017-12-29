#ifndef LANDMARK_H
#define LANDMARK_H
#include "../misc.h"
#include <QRectF>
#include <QDataStream>
#include <QVector>
#include <algorithm>
#include <fstream>

template <typename T>
class Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T xx, T yy) : x(xx), y(yy) {}
	float distanceTo(float xx, float yy) {
		return std::sqrt((x - xx) * (x - xx) + (y - yy) * (y - yy));
	}
};

class SelectRectF {
public:
	SelectRectF(float x=0, float y=0) : p0(x, y), p1(x, y) {}
	void startPoint(float x, float y) { p0.x = x; p0.y = y; p1.x = x; p1.y = y; }
	void newCorner(float x, float y) { p1.x = x; p1.y = y; }
	void reset() { p0.x = -1001; p0.y = -1001; p1.x = p0.x; p1.y = p0.y; }
	operator QRectF() const {
		float x0 = std::min(p0.x, p1.x);
		float y0 = std::min(p0.y, p1.y);
		float x1 = std::max(p0.x, p1.x);
		float y1 = std::max(p0.y, p1.y);
		return QRectF(x0, y0, x1 - x0, y1 - y0);
	}

	void boundWith(const QRectF &bound) {
		float x_min = bound.left();
		float y_min = bound.top();
		float x_max = bound.left() + bound.width();
		float y_max = bound.top() + bound.height();
		p0.x = std::max(p0.x, x_min);
		p1.x = std::max(p1.x, x_min);
		p0.y = std::max(p0.y, y_min);
		p1.y = std::max(p1.y, y_min);
		p0.x = std::min(p0.x, x_max);
		p1.x = std::min(p1.x, x_max);
		p0.y = std::min(p0.y, y_max);
		p1.y = std::min(p1.y, y_max);
	}
	bool isValid() const { return (p0.x >= 0 && p0.y >= 0); }

private:
	Point<float> p0;
	Point<float> p1;
};

class Landmark {
public:
    static int *g_mouthMask;
    static int g_mouthCount;
    static int g_count;
    Landmark() { Landmark::initStaticInfo(); }

	bool loadFromFStream(std::ifstream &fin);
	void exportPts(std::string path);
	void fillZero() { 
		points.clear();
		For(i, g_count) { points.append(Point<qreal>()); }
	}

	friend QDataStream &operator >> (QDataStream &, Landmark &);
	friend QDataStream &operator << (QDataStream &, const Landmark &);

	int size() const { return (int)points.size(); }
	Point<qreal> &operator[](int index) { return points[index]; }
	const Point<qreal> &operator[](int index) const { return points[index]; }

private:
    static void initStaticInfo();

	QVector<Point<qreal>> points;
};

QDataStream &operator >> (QDataStream &, Landmark &);
QDataStream &operator << (QDataStream &, const Landmark &);

template <typename T>
inline QDataStream &operator >> (QDataStream &stream, Point<T> &p) {
	stream >> p.x >> p.y;
	return stream;
}

template <typename T>
inline QDataStream &operator << (QDataStream &stream, const Point<T> &p) {
	stream << p.x << p.y;
	return stream;
}
#endif // LANDMARK_H
