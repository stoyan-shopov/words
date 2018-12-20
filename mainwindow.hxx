#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QFontDatabase>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
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
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
	{
		QGraphicsSimpleTextItem::mouseReleaseEvent(event);
		adjustPosition();
	}
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
