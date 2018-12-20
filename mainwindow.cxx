#include "mainwindow.hxx"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->graphicsView->setScene(& scene);
	Word * w;
	scene.addItem(w = new Word("hello"));
	scene.addItem(w = new Word("world"));
	scene.addItem(w = new Word("test1"));
	scene.addItem(w = new Word("test2"));
	w->moveBy(0, w->getFontHeight() + 3);
	scene.addItem(w = new Word("hello\nworld"));
	ui->graphicsView->setSceneRect(0, 0, 100, 100);
}

MainWindow::~MainWindow()
{
	delete ui;
}
