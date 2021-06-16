/********************************************************************************
** Form generated from reading UI file 'Mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MainwindowClass
{
public:
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *lineEdit_3;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QSpinBox *spinBox;
    QLabel *label;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *MainwindowClass)
    {
        if (MainwindowClass->objectName().isEmpty())
            MainwindowClass->setObjectName(QStringLiteral("MainwindowClass"));
        MainwindowClass->resize(322, 343);
        MainwindowClass->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 10pt \"\345\215\216\346\226\207\344\273\277\345\256\213\";"));
        horizontalLayout_8 = new QHBoxLayout(MainwindowClass);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(MainwindowClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_6 = new QHBoxLayout(groupBox);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_3->addWidget(pushButton);


        horizontalLayout_6->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(MainwindowClass);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_7 = new QHBoxLayout(groupBox_2);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        lineEdit_2 = new QLineEdit(groupBox_2);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_4->addWidget(lineEdit_2);

        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_4->addWidget(pushButton_2);


        horizontalLayout_7->addLayout(horizontalLayout_4);


        verticalLayout_2->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(MainwindowClass);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        horizontalLayout_5 = new QHBoxLayout(groupBox_3);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        lineEdit_3 = new QLineEdit(groupBox_3);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_5->addWidget(lineEdit_3);


        verticalLayout_2->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(MainwindowClass);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        horizontalLayout = new QHBoxLayout(groupBox_4);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        spinBox = new QSpinBox(groupBox_4);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setValue(10);

        verticalLayout->addWidget(spinBox);

        label = new QLabel(groupBox_4);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addWidget(groupBox_4);

        progressBar = new QProgressBar(MainwindowClass);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        verticalLayout_2->addWidget(progressBar);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_3 = new QPushButton(MainwindowClass);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_2->addWidget(pushButton_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_2);


        horizontalLayout_8->addLayout(verticalLayout_2);


        retranslateUi(MainwindowClass);

        QMetaObject::connectSlotsByName(MainwindowClass);
    } // setupUi

    void retranslateUi(QDialog *MainwindowClass)
    {
        MainwindowClass->setWindowTitle(QApplication::translate("MainwindowClass", "\350\247\206\351\242\221\350\247\243\345\270\247", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainwindowClass", "\351\200\211\346\213\251\350\247\206\351\242\221", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainwindowClass", "\351\200\211\346\213\251", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainwindowClass", "\344\277\235\345\255\230\347\233\256\345\275\225", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainwindowClass", "\351\200\211\346\213\251", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainwindowClass", "\350\256\276\347\275\256\346\226\207\344\273\266\345\211\215\347\274\200\346\240\274\345\274\217", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainwindowClass", "\350\256\276\347\275\256\350\247\206\351\242\221\346\212\275\345\217\226\347\216\207", Q_NULLPTR));
        label->setText(QApplication::translate("MainwindowClass", "\346\263\250\357\274\232\350\256\276\347\275\256\346\257\217\345\207\240\345\270\247\346\212\275\345\217\226\344\270\200\345\270\247", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainwindowClass", "\345\244\204\347\220\206", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainwindowClass: public Ui_MainwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
