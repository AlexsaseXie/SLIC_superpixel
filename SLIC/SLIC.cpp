#include "SLIC.h"
#include<opencv2/opencv.hpp>
#include<QFileDialog>
#include<QMessageBox>

using namespace cv;


SLIC::SLIC(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	this->ui.input_path_Label->setWordWrap(true);
	this->ui.output_path_Label->setWordWrap(true);

	connect(this->ui.input_img_Button, SIGNAL(clicked()), this, SLOT(choose_input_image()));
	connect(this->ui.output_img_Button, SIGNAL(clicked()), this, SLOT(choose_output_path()));
	connect(this->ui.work_Button, SIGNAL(clicked()), this, SLOT(do_slic()));
}

void SLIC::choose_input_image() {
	QString path = QFileDialog::getOpenFileName(this, tr("Open Image"),
		".", tr("Image Files (*.png *.jpg *.bmp)"));

	if (path == "")
		return;
	
	this->ui.input_path_Label->setText(path);
}

void SLIC::choose_output_path() {
	QString path = QFileDialog::getExistingDirectory(this,"save path", "./");

	if (path.isEmpty())
		return;

	this->ui.output_path_Label->setText(path + "/output.png");
}

void SLIC::do_slic() {
	int K;

	K = this->ui.K_Edit->text().toInt();

	if (K <= 0) {
		QMessageBox::critical(NULL, "SLIC failure!", "Please input a positive integer!!(basically > 200)");
		return;
	}

	f.set_K(K);

	f.set_img_path(this->ui.input_path_Label->text());
	f.set_output_path(this->ui.output_path_Label->text());

	f.load_img();
	f.do_slic();
}
