#pragma once

#include <QAbstractListModel>

#include <GeometryPrimitives.h>

class PolydotGeometryModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum PolydotGeometryRoles
	{
		LineGeometryRole = Qt::UserRole + 1,
	};

	explicit PolydotGeometryModel(QObject *parent = nullptr);

	QHash<int, QByteArray> roleNames() const override;
	int rowCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role) const override;

private:
	std::vector<std::unique_ptr<LineGeometry>> m_geometry;
};
