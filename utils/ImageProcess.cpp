#include "ImageProcess.h"


ImageProcess::ImageProcess()
{
}


ImageProcess::~ImageProcess()
{
}

QRectF ImageProcess::calcValidTargetRect(QSize imageSize, QSize windowSize) {
	int width = windowSize.width() - 80;
	int height = windowSize.height() - 120;
	double wRate = double(width) / imageSize.width();
	double hRate = double(height) / imageSize.height();
	double rate = (wRate < hRate) ? wRate : hRate;
	double showWidth = rate * imageSize.width();
	double showHeight = rate * imageSize.height();
	double left = (windowSize.width() - showWidth) / 2.0;
	double top = (windowSize.height() - showHeight) / 2.0 - 20;
	return QRectF(left, top, showWidth, showHeight);
}

QImage ImageProcess::Mat2QImage(cv::Mat const& src) {
	cv::Mat temp; // make the same cv::Mat  
	cvtColor(src, temp, CV_BGR2RGB); // cvtColor Makes a copt, that what i need  
	QImage dest((const uchar *)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
	dest.bits(); // enforce deep copy, see documentation  
				 // of QImage::QImage ( const uchar * data, int width, int height, Format format )  
	return dest;
}

cv::Mat ImageProcess::QImage2Mat(QImage const& src) {
	cv::Mat tmp(src.height(), src.width(), CV_8UC3, (uchar*)src.bits(), src.bytesPerLine());
	cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)  
	cvtColor(tmp, result, CV_BGR2RGB);
	return result;
}

void ImageProcess::zeroRect(QRectF &rect) {
	rect.setLeft(0);
	rect.setTop(0);
	rect.setWidth(0);
	rect.setHeight(0);
}

void ImageProcess::rectMergePoint(QRectF &rect, float x, float y) {
	float x0 = MIN(x, rect.left());
	float y0 = MIN(y, rect.top());
	float x1 = MAX(x, rect.left() + rect.width());
	float y1 = MAX(y, rect.top() + rect.height());
	rect.setLeft(x0);
	rect.setTop(y0);
	rect.setWidth(x1 - x0);
	rect.setHeight(y1 - y0);
}

void ImageProcess::rectNewCorner(QRectF &rect, float x, float y) {

}

void ImageProcess::boundRect(QRectF &rect, const QRectF &bound) {

}