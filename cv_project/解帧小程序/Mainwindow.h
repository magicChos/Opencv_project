#pragma once

#include <QtWidgets/QDialog>
#include "ui_Mainwindow.h"

class Mainwindow : public QDialog
{
	Q_OBJECT

public:
	Mainwindow(QWidget *parent = Q_NULLPTR);

protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void slot_select_video();
	void slot_select_dir();
	void slot_process();

private:
	Ui::MainwindowClass ui;
	QString m_video;
	QString m_savedir;
	QString m_currentPath;
	QString m_file_prefix;
	int m_sec;
	QString m_currentPath2;
};
