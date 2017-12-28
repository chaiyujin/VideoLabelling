#pragma once
#include <QRect>
#include <QSize>
#include <QImage>
#include <opencv2/opencv.hpp>

class ImageProcess
{
public:
	ImageProcess();
	~ImageProcess();
	static QRectF calcValidTargetRect(QSize imageSize, QSize windowSize);
	static QImage Mat2QImage(cv::Mat const& src);
	static cv::Mat QImage2Mat(QImage const& src);
	static void zeroRect(QRectF &rect);
	static void rectMergePoint(QRectF &rect, float x, float y);
	static void rectNewCorner(QRectF &rect, float x, float y);
	static void boundRect(QRectF &rect, const QRectF &bound);
};
