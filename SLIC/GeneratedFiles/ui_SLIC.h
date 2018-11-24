/********************************************************************************
** Form generated from reading UI file 'SLIC.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLIC_H
#define UI_SLIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SLICClass
{
public:
    QPushButton *input_img_Button;
    QLabel *input_path_Label;
    QLabel *output_path_Label;
    QPushButton *output_img_Button;
    QPushButton *work_Button;
    QLabel *label;
    QLineEdit *K_Edit;

    void setupUi(QWidget *SLICClass)
    {
        if (SLICClass->objectName().isEmpty())
            SLICClass->setObjectName(QStringLiteral("SLICClass"));
        SLICClass->resize(600, 400);
        input_img_Button = new QPushButton(SLICClass);
        input_img_Button->setObjectName(QStringLiteral("input_img_Button"));
        input_img_Button->setGeometry(QRect(21, 53, 131, 41));
        input_path_Label = new QLabel(SLICClass);
        input_path_Label->setObjectName(QStringLiteral("input_path_Label"));
        input_path_Label->setGeometry(QRect(170, 40, 381, 61));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(input_path_Label->sizePolicy().hasHeightForWidth());
        input_path_Label->setSizePolicy(sizePolicy);
        output_path_Label = new QLabel(SLICClass);
        output_path_Label->setObjectName(QStringLiteral("output_path_Label"));
        output_path_Label->setGeometry(QRect(170, 107, 381, 61));
        output_img_Button = new QPushButton(SLICClass);
        output_img_Button->setObjectName(QStringLiteral("output_img_Button"));
        output_img_Button->setGeometry(QRect(21, 120, 131, 41));
        work_Button = new QPushButton(SLICClass);
        work_Button->setObjectName(QStringLiteral("work_Button"));
        work_Button->setGeometry(QRect(180, 260, 201, 91));
        label = new QLabel(SLICClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 180, 141, 31));
        K_Edit = new QLineEdit(SLICClass);
        K_Edit->setObjectName(QStringLiteral("K_Edit"));
        K_Edit->setGeometry(QRect(170, 180, 311, 31));

        retranslateUi(SLICClass);

        QMetaObject::connectSlotsByName(SLICClass);
    } // setupUi

    void retranslateUi(QWidget *SLICClass)
    {
        SLICClass->setWindowTitle(QApplication::translate("SLICClass", "SLIC", Q_NULLPTR));
        input_img_Button->setText(QApplication::translate("SLICClass", "input image", Q_NULLPTR));
        input_path_Label->setText(QApplication::translate("SLICClass", "lena512color.bmp", Q_NULLPTR));
        output_path_Label->setText(QApplication::translate("SLICClass", "output.png", Q_NULLPTR));
        output_img_Button->setText(QApplication::translate("SLICClass", "output image", Q_NULLPTR));
        work_Button->setText(QApplication::translate("SLICClass", "Do superpixel!", Q_NULLPTR));
        label->setText(QApplication::translate("SLICClass", "Superpixel's K:", Q_NULLPTR));
        K_Edit->setInputMask(QString());
        K_Edit->setText(QApplication::translate("SLICClass", "1000", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SLICClass: public Ui_SLICClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLIC_H
