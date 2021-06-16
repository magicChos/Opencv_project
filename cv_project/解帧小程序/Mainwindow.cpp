#include "Mainwindow.h"
#include <opencv.hpp>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
Mainwindow::Mainwindow(QWidget *parent)
	: QDialog(parent)
	, m_currentPath("")
	, m_video("")
	, m_savedir("")
	, m_file_prefix("")
	, m_sec(10)
	, m_currentPath2("")
{
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()) , this , SLOT( slot_select_video()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slot_select_dir()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(slot_process()));

}

void Mainwindow::slot_select_video()
{
	m_video = QFileDialog::getOpenFileName(this, QStringLiteral("选取视频") , m_currentPath2 , QStringLiteral("视频文件(*.mp4 *.avi)"));
	if (m_video.isEmpty())
	{
		return;
	}
	
	ui.lineEdit->setText(m_video);
	QFileInfo fileinfo(m_video);
	m_currentPath2 = fileinfo.absoluteFilePath();
}

void Mainwindow::closeEvent(QCloseEvent * event)
{
	close();
	QDialog::accept();
}

void Mainwindow::slot_select_dir()
{
	m_savedir = QFileDialog::getExistingDirectory(this, QStringLiteral("设定保存目录"), m_currentPath);
	if (m_savedir.isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("没有设定照片保存目录"));
		return;
	}
	ui.lineEdit_2->setText(m_savedir);
	QFileInfo fileinfo(m_savedir);
	m_currentPath = fileinfo.absoluteFilePath();

	ui.pushButton_3->setEnabled(true);
}

void Mainwindow::slot_process()
{
	ui.pushButton_3->setEnabled(false);
	m_file_prefix = ui.lineEdit_3->text();
	cv::VideoCapture capture;
//	capture.open(m_video.toLocal8Bit().data());
	capture.open(ui.lineEdit->text().toLocal8Bit().data());
	if (!capture.isOpened())
	{
/*		std::cout << "fail to open" << std::endl;*/
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("fail to open!"));
		return;
	}

	cv::Mat frame;
	char image_name[256] = { 0 };
	long frameNumber = (long)capture.get(CV_CAP_PROP_FRAME_COUNT);

	ui.progressBar->setRange(0 , frameNumber);
	int i = 0 , k = 0 ;
	m_sec = ui.spinBox->value();
	int pp = frameNumber / 10;
	while (true)
	{
		if (!capture.read(frame))
		{
			break;
		}

		if (i % pp == 0)
		{
			qDebug() << ".";
		}
		if (i %m_sec == 0)
		{
			sprintf(image_name, "%s/%s-%d%s", ui.lineEdit_2->text().toLocal8Bit().data(), m_file_prefix.toLocal8Bit().data() , i, ".jpg");
			k++;
			cv::imwrite(image_name, frame);
		}
		i++;
		ui.progressBar->setValue(i);
	}
	capture.release();

	QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("完成"));
	ui.progressBar->reset();
	ui.pushButton_3->setEnabled(true);
}

