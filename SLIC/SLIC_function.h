#pragma once
#include<QString>
#include<QImage>
#include<QVector>
#include<QPoint>

#include<opencv2/opencv.hpp>

using namespace cv;

#define MAX_DISTANCE 10000000

class M_Point {
public:
	int row;
	int col;
public:
	inline M_Point() {}
	inline M_Point(int a, int b) : row(a), col(b) {}
	inline bool operator == (const M_Point &b) const {
		return row == b.row && col == b.col;
	}
};

class Cluster {
public:
	int row;
	int col;
	uchar l;
	uchar a;
	uchar b;

	QVector<M_Point> pts;
public:
	inline Cluster() {}
	inline Cluster(int r, int c, uchar l, uchar a, uchar b) : row(r),col(c),l(l),a(a),b(b) {}

	inline void update(int r, int c, uchar l, uchar a, uchar b){ this->row = r; this->col = c; this->l = l; this->a = a; this->b = b; }
};


class SLIC_Function{

public:
	QString img_path;
	QString save_path = "output.png";

	Mat img;
	Mat lab_img;
	Mat restore_lab_img;
	Mat processed_img;

	Mat dis;
	Mat label;

	int rows;
	int cols;

	int S;
	int K = 200;
	int M = 30;
	int N;

	QVector<Cluster> clusters;

public:
	inline SLIC_Function() {}
	inline SLIC_Function(QString path) : img_path(path) {}
	inline SLIC_Function(QString path, int tK, int tM) : img_path(path), K(tK), M(tM) {}

	void load_img();
	void init_params();
	void init_dis_label();
	void init_clusters();
	void move_clusters();
	void assignment();
	void update_clusters();
	int get_gradient(int r, int c);
	void restore_img();
	void save_img(Mat &bgr_img);
	void generate_lab_img();

	void enforce_connectivity();
	void calc_new_clusters(int num);

	void do_slic();
public:
	inline void set_img_path(QString path) { this->img_path = path; }
	inline void set_output_path(QString path) { this->save_path = path; }
	inline void set_K(int K) { this->K = K; }
	inline void clear() { 
		this->clusters.clear(); 
		this->img.release(); 
		this->lab_img.release(); 
		this->restore_lab_img.release(); 
		this->processed_img.release();
		this->label.release();
		this->dis.release();
	}
};
