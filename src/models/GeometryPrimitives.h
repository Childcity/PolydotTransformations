#pragma once

#include <QQuick3DGeometry>
#include <QVector3D>

class ExampleTriangleGeometry : public QQuick3DGeometry
{
	Q_OBJECT
	QML_NAMED_ELEMENT(ExampleTriangleGeometry)
	Q_PROPERTY(bool normals READ normals WRITE setNormals NOTIFY normalsChanged)
	Q_PROPERTY(float normalXY READ normalXY WRITE setNormalXY NOTIFY normalXYChanged)
	Q_PROPERTY(bool uv READ uv WRITE setUV NOTIFY uvChanged)
	Q_PROPERTY(float uvAdjust READ uvAdjust WRITE setUVAdjust NOTIFY uvAdjustChanged)

public:
	ExampleTriangleGeometry();

	bool normals() const
	{
		return m_hasNormals;
	}

	void setNormals(bool enable);

	float normalXY() const
	{
		return m_normalXY;
	}

	void setNormalXY(float xy);

	bool uv() const
	{
		return m_hasUV;
	}

	void setUV(bool enable);

	float uvAdjust() const
	{
		return m_uvAdjust;
	}

	void setUVAdjust(float f);

signals:
	void normalsChanged();
	void normalXYChanged();
	void uvChanged();
	void uvAdjustChanged();

private:
	void updateData();

	bool m_hasNormals = false;
	float m_normalXY = 0.0f;
	bool m_hasUV = false;
	float m_uvAdjust = 0.0f;
};

class PointGeometry
    : public QQuick3DGeometry
    , public QVector3D
{
	Q_OBJECT
	QML_NAMED_ELEMENT(PointGeometry)

	Q_PROPERTY(QVector3D point READ point WRITE setPoint NOTIFY pointChanged FINAL)

public:
	explicit PointGeometry() = default;

	QVector3D point() const;
	void setPoint(QVector3D newPoint);

signals:
	void pointChanged();

private:
	void updateData();
};

struct StreightLine
{
	float A{}; // Ax
	float B{}; // By
	float C{}; // C

	template<typename Container>
	    requires (std::ranges::input_range<Container>)
	static StreightLine FromArray(Container arr)
	{
		assert(arr.size() == 3);
		auto it = arr.begin();
		return {*it++, *it++, *it};
	}

	static StreightLine FromPoints(QVector3D p1, QVector3D p2);

	float length() const;

	bool isNormilized() const;
	StreightLine normilized() const;

	float signDistanceToPoint(QVector3D p) const;
};

class LineGeometry : public QQuick3DGeometry
{
	Q_OBJECT
	QML_NAMED_ELEMENT(LineGeometry)

	Q_PROPERTY(QVector3D p1 READ p1 WRITE setP1 NOTIFY p1Changed FINAL)
	Q_PROPERTY(QVector3D p2 READ p2 WRITE setP2 NOTIFY p2Changed FINAL)

public:
	explicit LineGeometry() = default;
	explicit LineGeometry(StreightLine line);
	LineGeometry(QVector3D p1, QVector3D p2);

	QVector3D p1() const;
	void setP1(QVector3D newP1);

	QVector3D p2() const;
	void setP2(QVector3D newP2);

	StreightLine toStraightLine();

signals:
	void p1Changed();
	void p2Changed();

private:
	void updateData();

	QVector3D m_p1;
	QVector3D m_p2;
};

QDebug operator<<(QDebug, const PointGeometry &);
QDebug operator<<(QDebug, const StreightLine &);
QDebug operator<<(QDebug, const LineGeometry &);
