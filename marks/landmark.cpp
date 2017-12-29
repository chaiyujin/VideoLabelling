#include "landmark.h"
#include <fstream>
using namespace std;

/* static */

int *Landmark::g_mouthMask = nullptr;
int  Landmark::g_mouthCount = 20;
int  Landmark::g_count      = 68;

void Landmark::initStaticInfo() {
    if (g_mouthMask == nullptr) {
        ifstream fin("config/points.cfg");
        if (fin.is_open()) {
#ifdef DEBUG_LOG
            qDebug() << "init from cfg";
#endif
            fin >> g_count >> g_mouthCount;
			g_mouthMask = new int[g_count];
			For(i, g_count) g_mouthMask[i] = 0;
			For(i, g_mouthCount) {
				int j; fin >> j;
				g_mouthMask[j] = 1;
            }
			fin.close();
        }
        else {
            // use default indexes;
			g_mouthMask = new int[g_count];
			For(i, g_count) g_mouthMask[i] = 0;
			For(i, g_mouthCount) {
				g_mouthMask[48 + i] = 1;
            }
        }
    }
}

bool Landmark::loadFromFStream(std::ifstream &fin) {
	points.clear();
	For(i, Landmark::g_count) {
		if (fin.eof()) return false;
		float x, y;
		fin >> x >> y;
		points.append(Point<qreal>((qreal)x, (qreal)y));
	}
	return true;
}

void Landmark::exportPts(std::string path) {
	ofstream fout(path);
	fout << "version: 1\nn_points: " << g_count << "\n{\n";
	For(i, g_count) {
		fout << points[i].x << " " << points[i].y << "\n";
	}
	fout << "}";
	fout.close();
}

QDataStream &operator >> (QDataStream &stream, Landmark &lm) {
	stream >> lm.points;
	return stream;
}

QDataStream &operator << (QDataStream &stream, const Landmark &lm) {
	stream << lm.points;
	return stream;
}