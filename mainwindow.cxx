#include "mainwindow.hxx"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->graphicsView->setScene(& scene);
	Word * w;
	scene.addItem(w = new Word("hello")); w->adjustPosition();
	scene.addItem(w = new Word("world")); w->adjustPosition();
	scene.addItem(w = new Word("test1")); w->adjustPosition();
	scene.addItem(w = new Word("test2")); w->adjustPosition();
	w->moveBy(0, w->getFontHeight() + 3);
	scene.addItem(w = new Word("hello\nworld")); w->adjustPosition();

	ui->graphicsView->setSceneRect(0, 0, 100, 100);
}

MainWindow::~MainWindow()
{
	delete ui;
}
