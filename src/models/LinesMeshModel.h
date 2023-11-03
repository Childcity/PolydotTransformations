#pragma once

#include <QAbstractListModel>

#include <models/GeometryPrimitives.h>

class LinesMeshModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum LinesMeshRoles
	{
		LineGeometryRole = Qt::UserRole + 1,
	};

	explicit LinesMeshModel(QObject *parent = nullptr);

	QHash<int, QByteArray> roleNames() const override;
	int rowCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role) const override;

private:
	std::vector<std::unique_ptr<LineGeometry>> m_geometry;
};
