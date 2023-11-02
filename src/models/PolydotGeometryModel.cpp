#include "PolydotGeometryModel.h"

PolydotGeometryModel::PolydotGeometryModel(QObject *parent)
    : QAbstractListModel(parent)
{
	m_geometry.emplace_back(std::make_unique<LineGeometry>(QVector3D{7, 2, 0}, QVector3D{10, 6, 0}));
	m_geometry.emplace_back(std::make_unique<LineGeometry>(QVector3D{7, 4, 0}, QVector3D{10, 8, 0}));
}

QHash<int, QByteArray> PolydotGeometryModel::roleNames() const
{
	return {
	    {LineGeometryRole, "lineGeometry"},
	};
}

int PolydotGeometryModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid()) {
		return 0;
	}

	return static_cast<int>(m_geometry.size());
}

QVariant PolydotGeometryModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return {};
	}

	const auto row = index.row();

	switch (role) {
	case LineGeometryRole:
		return QVariant::fromValue(m_geometry.at(row).get());
	default:
		return {};
	}
}
