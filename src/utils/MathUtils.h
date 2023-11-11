#pragma once

#include <QObject>

#include <models/GeometryPrimitives.h>

class MathUtils : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(MathUtils)

public:
	static MathUtils &Get();
	static QObject *Get(class QQmlEngine *, class QJSEngine *);

	explicit MathUtils(QObject * = nullptr);
	~MathUtils() override = default;

	static StreightLine getPolydotTransformedLine(
	    const Line &line, //
	    const QVariantList &origBasises,
	    const QVariantList &resBasises);

	Q_INVOKABLE LineGeometry *getPolydotTransformedLine(
	    const LineGeometry *line, //
	    const QVariantList &origBasises,
	    const QVariantList &resBasises);

	static Mesh getPolydotTransformedMesh(
	    Mesh mesh, //
	    const QVariantList &origBasises,
	    const QVariantList &resBasises);
};
