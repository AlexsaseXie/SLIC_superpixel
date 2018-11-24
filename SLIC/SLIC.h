#pragma once

#include <QtWidgets/QWidget>
#include "ui_SLIC.h"
#include "SLIC_function.h"

class SLIC : public QWidget
{
	Q_OBJECT

public:
	SLIC(QWidget *parent = Q_NULLPTR);
	SLIC_Function f = SLIC_Function("lena512color.bmp", 1000, 30);

private:
	Ui::SLICClass ui;

public slots:
	void choose_input_image();
	void choose_output_path();
	void do_slic();
};
