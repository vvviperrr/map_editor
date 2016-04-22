#ifndef RMAPOBJECTTABLEVIEWDELEGATE_H
#define RMAPOBJECTTABLEVIEWDELEGATE_H

#include <QItemDelegate>

class RMapObjectTableViewDelegate : public QItemDelegate {
     Q_OBJECT
 public:
     RMapObjectTableViewDelegate(QObject *parent = 0);

     QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

     void setEditorData(QWidget *editor, const QModelIndex &index) const;

	 void setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const;

	 void updateEditorGeometry(QWidget *editor,
							   const QStyleOptionViewItem &option, const QModelIndex &index) const;

	 void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

     QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:
     void commitAndCloseEditor(int id);
};

#endif // RMAPOBJECTTABLEVIEWDELEGATE_H
