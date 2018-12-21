#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QFontDatabase>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QTouchEvent>
#include <QGraphicsView>
#include <QGestureEvent>
#include <QDebug>
#include <cmath>

namespace Ui {
class MainWindow;
}

class Word : public QObject, public QGraphicsSimpleTextItem
{
	Q_OBJECT
private:
	int fontWidth, fontHeight;
#ifdef Q_OS_ANDROID
	enum
	{
		ANDROID_WORD_MOVEMENT_DISPLACEMENT_X_FACTOR	= 5,
		ANDROID_WORD_MOVEMENT_DISPLACEMENT_Y_FACTOR	= 3,
	};
	QPointF mousePressPosition;
#endif
public:
	int getFontWidth(void) { return fontWidth; }
	int getFontHeight(void) { return fontHeight; }
	Word(const QString &text, QGraphicsItem *parent = Q_NULLPTR) : QGraphicsSimpleTextItem(text + ' ', parent)
	{
		const QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
		QFontMetrics fm(f);
		fontWidth = fm.width(' ');
		fontHeight = fm.height();
		setFont(f);
		setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
	}

	void adjustPosition(void)
	{
		qDebug() << "colliding items count:" << scene()->collidingItems(this, Qt::IntersectsItemBoundingRect).count();
		auto items = scene()->items();
		QVector<QGraphicsItem *> rowItems;
		for (const auto& i : items)
		{
			if (i == this)
				continue;
			if (pos().y() == i->pos().y())
				rowItems << i;
		}
		auto x = pos().x();
		qreal d= .0;

		/* adjust moved word position*/
		for (auto& i : rowItems)
		{
			auto r = i->pos().x(), w = i->boundingRect().width();
			if (r <= x && x < r + w)
			{
				d = r + w - x;
				break;
			}

		}

		moveBy(d, 0);
		x = pos().x();
		d = .0;

		/* if necessary, adjust the positions of other words on the same row */
		for (auto& i : rowItems)
		{
			auto r = i->pos().x();
			if (x <= r && r < x + boundingRect().width())
			{
				d = x + boundingRect().width() - r;
				break;
			}
		}
		if (d == .0)
			return;

		for (auto& i : rowItems)
		{
			if (i->pos().x() >= x)
				i->moveBy(d, 0);
		}
	}
protected:
	bool sceneEvent(QEvent * event)
	{
		if (event->type() == QEvent::Gesture)
		{
			QGestureEvent * e = static_cast<QGestureEvent *>(event);
			*(int*)0=0;
		}
		return QGraphicsSimpleTextItem::sceneEvent(event);
	}
	void mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
#ifdef Q_OS_ANDROID
		mousePressPosition = event->scenePos();
		// !!! Android only !!!
		moveBy(-ANDROID_WORD_MOVEMENT_DISPLACEMENT_X_FACTOR * fontWidth, -ANDROID_WORD_MOVEMENT_DISPLACEMENT_Y_FACTOR * fontHeight);
#endif
		QGraphicsSimpleTextItem::mousePressEvent(event);
	scene()->views().at(0)->grabGesture(Qt::TapAndHoldGesture);
	}
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
	{
		QGraphicsSimpleTextItem::mouseReleaseEvent(event);
#ifdef Q_OS_ANDROID
		if (event->scenePos() == mousePressPosition)
		{
			// !!! Android only - undo movement in case release position is the same as the press position !!!
			moveBy(ANDROID_WORD_MOVEMENT_DISPLACEMENT_X_FACTOR * fontWidth, ANDROID_WORD_MOVEMENT_DISPLACEMENT_Y_FACTOR * fontHeight);
			return;
		}
#endif
		adjustPosition();
	}
	QVariant itemChange(GraphicsItemChange change,
			    const QVariant &value)
	{
		if (change == ItemPositionChange && scene()) {
			QPointF newPos = value.toPointF();
				qreal x = round(newPos.x()/fontWidth) * fontWidth;
				//if (x < 0) x = 0;
				if (x > 256 * fontWidth) x = 256 * fontWidth;
				qreal y = round(newPos.y()/fontHeight) * fontHeight;
				//if (y < 0) y = 0;
				if (y > 256 * fontHeight) y = 256 * fontHeight;
				return QPointF(x, y);
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
