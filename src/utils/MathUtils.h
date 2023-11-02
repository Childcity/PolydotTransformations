#pragma once

#include <QObject>

class MathUtils : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(MathUtils)

public:
	static QObject *CreateInstance(class QQmlEngine *, class QJSEngine *);

	explicit MathUtils(QObject * = nullptr);

	Q_INVOKABLE class LineGeometry *getPolydotLine(
	    LineGeometry *line,
	    const QVariantList &originalBasis,
	    const QVariantList &resultBasis) const;
};
