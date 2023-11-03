#include "PolydotGeometryModel.h"

#include <ColladaFormatImporter.h>

PolydotGeometryModel::PolydotGeometryModel(QObject *parent)
    : QAbstractListModel(parent)
{
	const auto path = "C:\\Users\\Ariel\\Documents\\AAScetch\\exp\\Untitled.dae";
	ColladaFormatImporter importer(path);
	importer.importGeometries();

	const auto geometries = importer.getGeometries();

	for (auto &geom : geometries.front()) {
		m_geometry.emplace_back(std::make_unique<LineGeometry>(geom));
	}
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
