#pragma once

#include <GeometryPrimitives.h>

struct QXmlStreamReader;
struct Line;

using Mesh = std::vector<Line>;
using Meshes = std::vector<Mesh>;

class ColladaFormatImporter
{
public:
	explicit ColladaFormatImporter(const QString path);

	void importGeometries();
	Meshes getGeometries();

private:
	void readCollada(QXmlStreamReader &reader);
	void readLibraryGeometries(QXmlStreamReader &reader);
	void readMesh(QXmlStreamReader &reader);

private:
	QString m_path;
	Meshes m_meshes;
};
