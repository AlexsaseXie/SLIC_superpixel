#include "SLIC_function.h"
#include<math.h>


void SLIC_Function::load_img() {
	img = imread(this->img_path.toStdString());
	cvtColor(img, lab_img, CV_BGR2Lab);

	namedWindow("input image", CV_WINDOW_AUTOSIZE);
	imshow("input image", this->img);
}

void SLIC_Function::init_params() {
	this->rows = img.rows;
	this->cols = img.cols;

	this->N = this->rows * this->cols;
	this->S = (int)sqrt((double)this->N / (double)this->K);
}

void SLIC_Function::init_dis_label() {
	dis = Mat(this->rows, this->cols, CV_32S, cv::Scalar::all(MAX_DISTANCE));
	label = Mat(this->rows, this->cols, CV_32S, cv::Scalar::all(-1));
}

void SLIC_Function::init_clusters() {
	int r = this->S / 2;
	int c = this->S / 2;

	while (r < this->rows) {
		while (c < this->cols) {
			Vec3b lab = this->lab_img.at<Vec3b>(int(r), int(c));
			clusters.push_back(Cluster(int(r), int(c), lab[0], lab[1], lab[2]));
			c += this->S;
		}
		c = this->S / 2;
		r += this->S;
	}
}

void SLIC_Function::move_clusters() {
	for (auto &c : this->clusters) {
		int current_g = this->get_gradient(c.row, c.col);

		int row = c.row;
		int col = c.col;

		for (int dr = -1; dr <= 1; dr++) {
			for (int dc = -1; dc <= 1; dc++) {
				int new_r = row + dr;
				int new_c = col + dc;

				int new_g = this->get_gradient(new_r, new_c);

				if (new_g < current_g) {
					current_g = new_g;
					Vec3b lab = this->lab_img.at<Vec3b>(new_r, new_c);
					c.update(new_r, new_c, lab[0], lab[1], lab[2]);
				}
			}
		}
	}
}

int SLIC_Function::get_gradient(int r, int c) {
	if (c + 1 >= this->cols) {
		c = this->cols - 2;
	}
	if (r + 1 >= this->rows) {
		r = this->rows - 2;
	}

	const Vec3b & r1_c_lab = this->lab_img.at<Vec3b>(r + 1, c);
	const Vec3b & r_c1_lab = this->lab_img.at<Vec3b>(r, c + 1);
	const Vec3b & r_c_lab = this->lab_img.at<Vec3b>(r, c);

	int gradient = 0;
	for (int i = 0; i < 3; i++) {
		gradient += r1_c_lab[i] + r_c1_lab[i] - 2 * r_c_lab[i];
	}

	return gradient;
}

void SLIC_Function::assignment() {
	int k = 0;
	int cluster_size = this->clusters.size();
	for (k = 0; k < cluster_size;k++) {
		Cluster &c = this->clusters[k];
		for (int dr = -2 * this->S; dr <= 2 * this->S; dr++) {
			int current_r = c.row + dr;
			if (current_r < 0 || current_r >= this->rows) continue;

			for (int dc = -2 * this->S; dc <= 2 * this->S; dc++) {
				int current_c = c.col + dc;
				if (current_c < 0 || current_c >= this->cols) continue;

				const Vec3b &current = this->lab_img.at<Vec3b>(current_r, current_c);
				uchar L = current[0];
				uchar A = current[1];
				uchar B = current[2];

				int Dc_square = (int)(L - c.l) * (int)(L - c.l) + (int)(A - c.a) * (int)(A - c.a) + (int)(B - c.b) * (int)(B - c.b);
				int Ds_square = (int)(current_r - c.row) * (int)(current_r - c.row) + (int)(current_c - c.col) * (int)(current_c - c.col);
				int D_square_mul_M = (Dc_square * this->S * this->S) + (Ds_square * this->M * this->M);

				int current_dis = dis.at<int>(current_r, current_c);

				if (D_square_mul_M < current_dis) {
					int pre_k = label.at<int>(current_r, current_c);

					if (pre_k == -1) {
						label.at<int>(current_r, current_c) = k;
						c.pts.push_back(M_Point(current_r, current_c));
					}
					else {
						int pre_index = this->clusters[pre_k].pts.indexOf(M_Point(current_r, current_c));
						if (pre_index >= 0) {
							this->clusters[pre_k].pts.erase(this->clusters[pre_k].pts.begin() + pre_index);
						}
						label.at<int>(current_r, current_c) = k;
						c.pts.push_back(M_Point(current_r, current_c));
					}
					dis.at<int>(current_r, current_c) = D_square_mul_M;
				}
			}
		}
	}
}

void SLIC_Function::update_clusters() {
	for (auto &c : this->clusters) {
		int sum_r = 0;
		int sum_c = 0;
		int sum_l = 0;
		int sum_a = 0;
		int sum_b = 0;
		int count = 0;
		for (auto &p : c.pts) {
			sum_r += p.row;
			sum_c += p.col;
			sum_l += this->lab_img.at<Vec3b>(p.row, p.col)[0];
			sum_a += this->lab_img.at<Vec3b>(p.row, p.col)[1];
			sum_b += this->lab_img.at<Vec3b>(p.row, p.col)[2];
			count++;
		}

		if (count == 0)
			continue;

		int _r = sum_r / count;
		int _c = sum_c / count;

		uchar _l = (uchar)(sum_l / count);
		uchar _a = (uchar)(sum_a / count);
		uchar _b = (uchar)(sum_b / count);
		
		c.update(_r, _c, _l, _a, _b);
	}
}

void SLIC_Function::generate_lab_img() {
	this->restore_lab_img = Mat(this->lab_img);

	for (int r = 0; r < this->rows; r++) {
		for (int c = 0; c < this->cols; c++) {
			int current_label = label.at<int>(r, c);
			Cluster & current_cluster = this->clusters[current_label];
			//if (!(r == current_cluster.row && c == current_cluster.col))
				lab_img.at<Vec3b>(r, c) = Vec3b(current_cluster.l, current_cluster.a, current_cluster.b);
			//else
			//	lab_img.at<Vec3b>(r, c) = Vec3b(0, 0, 0);
		}
	}

	/*for (auto &c : this->clusters) {
		for (auto &p : c.pts) {
			restore_lab_img.at<Vec3b>(p.row, p.col) = Vec3b(c.l, c.a, c.b);
		}
		restore_lab_img.at<Vec3b>(c.row, c.col) = Vec3b(0, 0, 0);
	}*/
}

void SLIC_Function::save_img(Mat &bgr_img) {
	imwrite(this->save_path.toStdString(), bgr_img);
}

void SLIC_Function::restore_img() {
	cvtColor(restore_lab_img, processed_img, CV_Lab2BGR);

	namedWindow("processed img", CV_WINDOW_AUTOSIZE);
	imshow("processed img", processed_img);
}

void SLIC_Function::enforce_connectivity() {
	Mat new_label = Mat(this->rows, this->cols, CV_32S, cv::Scalar::all(-1));

	const int dr[4] = { -1, 0, 1, 0 };
	const int dc[4] = { 0, -1, 0, 1 };

	int num_label = 0;
	const int SUPSZ = this->N / this->K;

	int current_label = 0;
	int adjacent_label = 0;

	QVector<M_Point> segment;

	for (int r = 0; r < this->rows; r++) {
		for (int c = 0; c < this->cols; c++) {
			if (new_label.at<int>(r, c) < 0) {
				segment.clear();

				new_label.at<int>(r, c) = current_label;
				segment.push_back(M_Point(r, c));
				for (int n = 0; n < 4; n++) {
					int ad_r = r + dr[n];
					int ad_c = c + dc[n];
					if (ad_r >= 0 && ad_r < this->rows && ad_c >= 0 && ad_c < this->cols) {
						if (new_label.at<int>(ad_r, ad_c) >= 0) {
							adjacent_label = new_label.at<int>(ad_r, ad_c);
						}
					}
				}

				int count = 1;
				for (int _c = 0; _c < count; _c++) {
					for (int n = 0; n < 4; n++) {
						int ad_r = segment[_c].row + dr[n];
						int ad_c = segment[_c].col + dc[n];

						if (ad_r >= 0 && ad_r < this->rows && ad_c >= 0 && ad_c < this->cols) {
							if (new_label.at<int>(ad_r, ad_c) < 0 && label.at<int>(r, c) == label.at<int>(ad_r, ad_c)) {
								segment.push_back(M_Point(ad_r, ad_c));
								new_label.at<int>(ad_r, ad_c) = current_label;
								count++;
							}
						}
					}
				}

				if (count <= SUPSZ >> 2) {
					for (int c = 0; c < count; c++) {
						new_label.at<int>(segment[c].row, segment[c].col) = adjacent_label;
					}
					current_label--;
				}
				current_label++;
			}
		}
	}

	num_label = current_label;
	this->label.release();

	this->label = new_label;

	this->calc_new_clusters(num_label);
}

void SLIC_Function::calc_new_clusters(int num) {
	this->clusters.clear();
	this->clusters = QVector<Cluster>(num);

	for (int r = 0; r < this->rows; r++) {
		for (int c = 0; c < this->cols; c++) {
			int l = this->label.at<int>(r, c);
			this->clusters[l].pts.push_back(M_Point(r, c));
		}
	}

	this->update_clusters();
}

void SLIC_Function::do_slic() {
	this->clusters.clear();

	this->init_params();
	this->init_dis_label();

	this->init_clusters();
	this->move_clusters();


	for (int i = 0; i < 5; i++) {
		this->assignment();
		this->update_clusters();
	}

	this->enforce_connectivity();

	//generate new png
	this->generate_lab_img();
	this->restore_img();

	//save 
	this->save_img(processed_img);

	//release
	this->clear();
}