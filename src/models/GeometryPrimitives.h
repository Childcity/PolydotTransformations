#pragma once

#include <tuple>

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
	Q_DISABLE_COPY(PointGeometry)

	Q_PROPERTY(QVector3D point READ point WRITE setPoint NOTIFY pointChanged FINAL)

public:
	explicit PointGeometry() = default;
	~PointGeometry() override = default;

	QVector3D point() const;
	void setPoint(QVector3D newPoint);

signals:
	void pointChanged();

private:
	void updateData();
};

struct StreightLine;

struct Line
{
	QVector3D p1;
	QVector3D p2;

	static Line FromStreightLine(StreightLine line);

	StreightLine toStraightLine() const;

	bool isNull() const;
};

using Mesh = std::vector<Line>;
using MeshList = std::vector<Mesh>;

struct StreightLine
{
	double A{}; // Ax
	double B{}; // By
	double C{}; // C

	template<typename Container>
	    requires (std::ranges::input_range<Container>)
	static StreightLine FromArray(Container arr)
	{
		assert(arr.size() == 3);
		auto it = arr.begin();
		return {*it++, *it++, *it};
	}

	static StreightLine FromLine(Line line);

	StreightLine &operator*=(double cnst)
	{
		A *= cnst;
		B *= cnst;
		C *= cnst;
		return *this;
	}

	StreightLine &operator+=(double cnst)
	{
		A += cnst;
		B += cnst;
		C += cnst;
		return *this;
	}

	// clang-format off

	auto toTuple() { return std::tuple{A, B, C}; }

	// clang-format on

	bool isNull() const;
	double length() const;

	bool isNormilized() const;
	StreightLine normilized() const;

	double signDistanceToPoint(QVector3D p) const;

	QVector3D intersect(StreightLine other);
};

class LineGeometry : public QQuick3DGeometry
{
	Q_OBJECT
	QML_NAMED_ELEMENT(LineGeometry)
	Q_DISABLE_COPY(LineGeometry)

	Q_PROPERTY(QVector3D p1 READ p1 WRITE setP1 NOTIFY p1Changed FINAL)
	Q_PROPERTY(QVector3D p2 READ p2 WRITE setP2 NOTIFY p2Changed FINAL)

public:
	explicit LineGeometry() = default;
	explicit LineGeometry(StreightLine line);
	explicit LineGeometry(Line line);
	~LineGeometry() override = default;

	QVector3D p1() const;
	void setP1(QVector3D newP1);

	QVector3D p2() const;
	void setP2(QVector3D newP2);

	StreightLine toStraightLine() const;
	Line toLine() const;

signals:
	void p1Changed();
	void p2Changed();

private:
	void updateData();

	Line m_line;
};

QDebug operator<<(QDebug, const PointGeometry &);
QDebug operator<<(QDebug, const StreightLine &);
QDebug operator<<(QDebug, const LineGeometry &);
