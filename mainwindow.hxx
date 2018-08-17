#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QFontDatabase>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>

namespace Ui {
class MainWindow;
}

class Word : public QObject, public QGraphicsSimpleTextItem
{
	Q_OBJECT
private:
	int fontWidth, fontHeight;
public:
	int getFontWidth(void) { return fontWidth; }
	int getFontHeight(void) { return fontHeight; }
	Word(const QString &text, QGraphicsItem *parent = Q_NULLPTR) : QGraphicsSimpleTextItem(text, parent)
	{
		const QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
		QFontMetrics fm(f);
		fontWidth = fm.width(' ');
		fontHeight = fm.height();
		setFont(f);
		setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
	}
protected:
	QVariant itemChange(GraphicsItemChange change,
			    const QVariant &value)
	{
		if (change == ItemPositionChange && scene()) {
			QPointF newPos = value.toPointF();
				qreal x = round(newPos.x()/fontWidth) * fontWidth;
				if (x < 0) x = 0;
				if (x > 256 * fontWidth) x = 256 * fontWidth;
				qreal y = round(newPos.y()/fontHeight) * fontHeight;
				if (y < 0) y = 0;
				if (y > 256 * fontHeight) y = 256 * fontHeight;
				return QPointF(x, y);
			/*
			QPointF newPos = value.toPointF();
			if(QApplication::mouseButtons() == Qt::LeftButton &&
					qobject_cast<Scene*> (scene())){
				Scene* customScene = qobject_cast<Scene*> (scene());
				int gridSize = customScene->getGridSize();
				qreal xV = round(newPos.x()/gridSize)*gridSize;
				qreal yV = round(newPos.y()/gridSize)*gridSize;
				return QPointF(xV, yV);
			}
			else
				return newPos;
				*/
		}
		else
			return QGraphicsSimpleTextItem::itemChange(change, value);
	}
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	QGraphicsScene scene;
};

#endif // MAINWINDOW_HXX
