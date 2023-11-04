#include "GeometryPrimitives.h"

#include <QVector3D>

namespace {

using PType = float;
constexpr int cPointDataCount = 3;
constexpr int cPointDataSize = cPointDataCount * sizeof(PType);

// for (auto i = 0; i < vertexes.size(); i += sizeof(int)) {
//	auto number = std::span(vertexes.begin() + i, sizeof(int));
//	qDebug() << qFromLittleEndian<int>(number.data());
// }

// using v = QVector2D;
// const auto lines = std::vector{
//     v{0, 0},
//     v{5, 5},
//     v{6, 5},
//     v{1, 2},
// };

// namespace std {

// template<size_t N>
// span(const QByteArray &b) -> span<const char>;

//} // namespace std

void setPoint(QByteArray &data, int pointIndex, QVector3D point)
{
	PType *ptr = reinterpret_cast<PType *>(data.begin());
	std::advance(ptr, pointIndex * cPointDataCount);
	*ptr++ = static_cast<PType>(point.x());
	*ptr++ = static_cast<PType>(point.y());
	*ptr++ = static_cast<PType>(point.z());
}

QString toString(QVector3D vec)
{
	return QString("X:%1 Y:%2 Z:%3")
	    .arg(QString::number(vec.x()), QString::number(vec.y()), QString::number(vec.z()));
}

} // namespace

ExampleTriangleGeometry::ExampleTriangleGeometry()
{
	updateData();
}

void ExampleTriangleGeometry::setNormals(bool enable)
{
	if (m_hasNormals == enable) {
		return;
	}

	m_hasNormals = enable;
	emit normalsChanged();
	updateData();
	update();
}

void ExampleTriangleGeometry::setNormalXY(float xy)
{
	if (m_normalXY == xy) {
		return;
	}

	m_normalXY = xy;
	emit normalXYChanged();
	updateData();
	update();
}

void ExampleTriangleGeometry::setUV(bool enable)
{
	if (m_hasUV == enable) {
		return;
	}

	m_hasUV = enable;
	emit uvChanged();
	updateData();
	update();
}

void ExampleTriangleGeometry::setUVAdjust(float f)
{
	if (m_uvAdjust == f) {
		return;
	}

	m_uvAdjust = f;
	emit uvAdjustChanged();
	updateData();
	update();
}

void ExampleTriangleGeometry::updateData()
{
	clear();

	int stride = 3 * sizeof(float);
	if (m_hasNormals) {
		stride += 3 * sizeof(float);
	}
	if (m_hasUV) {
		stride += 2 * sizeof(float);
	}

	QByteArray vertexData(3 * stride, Qt::Initialization::Uninitialized);
	float *p = reinterpret_cast<float *>(vertexData.data());

	// a triangle, front face = counter-clockwise
	*p++ = -1.0f;
	*p++ = -1.0f;
	*p++ = 0.0f;
	if (m_hasNormals) {
		*p++ = m_normalXY;
		*p++ = m_normalXY;
		*p++ = 1.0f;
	}
	if (m_hasUV) {
		*p++ = 0.0f + m_uvAdjust;
		*p++ = 0.0f + m_uvAdjust;
	}
	*p++ = 1.0f;
	*p++ = -1.0f;
	*p++ = 0.0f;
	if (m_hasNormals) {
		*p++ = m_normalXY;
		*p++ = m_normalXY;
		*p++ = 1.0f;
	}
	if (m_hasUV) {
		*p++ = 1.0f - m_uvAdjust;
		*p++ = 0.0f + m_uvAdjust;
	}
	*p++ = 0.0f;
	*p++ = 1.0f;
	*p++ = 0.0f;
	if (m_hasNormals) {
		*p++ = m_normalXY;
		*p++ = m_normalXY;
		*p++ = 1.0f;
	}
	if (m_hasUV) {
		*p++ = 1.0f - m_uvAdjust;
		*p++ = 1.0f - m_uvAdjust;
	}

	setVertexData(vertexData);
	setStride(stride);
	setBounds(QVector3D(-1.0f, -1.0f, 0.0f), QVector3D(+1.0f, +1.0f, 0.0f));

	setPrimitiveType(PrimitiveType::Triangles);

	addAttribute(Attribute::PositionSemantic, 0, Attribute::F32Type);

	if (m_hasNormals) {
		addAttribute(Attribute::NormalSemantic, 3 * sizeof(float), Attribute::F32Type);
	}

	if (m_hasUV) {
		addAttribute(
		    Attribute::TexCoordSemantic,
		    m_hasNormals ? 6 * sizeof(float) : 3 * sizeof(float),
		    Attribute::F32Type);
	}
}

QVector3D PointGeometry::point() const
{
	return {x(), y(), z()};
}

void PointGeometry::setPoint(QVector3D newPoint)
{
	if (point() == newPoint) {
		return;
	}

	setX(newPoint.x());
	setY(newPoint.y());
	setZ(newPoint.z());

	emit pointChanged();
	updateData();
}

void PointGeometry::updateData()
{
	constexpr auto pointsCount = 1;
	QByteArray vertexes(pointsCount * cPointDataSize, Qt::Initialization::Uninitialized);

	::setPoint(vertexes, 0, point());

	clear();
	setVertexData(vertexes);
	setStride(cPointDataSize);
	setBounds(point(), point());

	setPrimitiveType(PrimitiveType::Points);
	addAttribute(Attribute::PositionSemantic, 0, Attribute::F32Type);
	update();
}

Line Line::FromStreightLine(StreightLine line)
{
	if (line.isNull()) {
		// Q_ASSERT(false);
		return {};
	}
	const auto [A, B, C] = std::tuple{line.A, line.B, line.C};
	return {
	    .p1 = {0, -C / B, 0}, //
	    .p2 = {1, (-A - C) / B, 0} //
	};
}

StreightLine Line::toStraightLine() const
{
	return StreightLine::FromLine(*this);
}

bool Line::isNull() const
{
	return qFuzzyIsNull(p1.distanceToPoint(p2));
}

StreightLine StreightLine::FromLine(Line line)
{
	// https://math.stackexchange.com/questions/637922/how-can-i-find-coefficients-a-b-c-given-two-points
	return {
	    line.p1.y() - line.p2.y(),
	    line.p2.x() - line.p1.x(),
	    line.p1.x() * line.p2.y() - line.p2.x() * line.p1.y(),
	};
}

bool StreightLine::isNull() const
{
	return qFuzzyIsNull(length());
}

float StreightLine::length() const
{
	return std::hypot(A, B); // std::sqrt(A * A + B * B);
}

bool StreightLine::isNormilized() const
{
	const float len = length();
	return qFuzzyIsNull(len - 1.f) || qFuzzyIsNull(len);
}

StreightLine StreightLine::normilized() const
{
	if (isNormilized()) {
		return *this;
	}
	const float len = length();
	return {A / len, B / len, C / len};
}

float StreightLine::signDistanceToPoint(QVector3D p) const
{
	if (!isNormilized()) {
		qFatal("distanceToPoint must be called for normilized line!");
	}
	return A * p.x() + B * p.y() + C;
}

LineGeometry::LineGeometry(StreightLine line)
{
	m_line = Line::FromStreightLine(line);
	updateData();
}

LineGeometry::LineGeometry(Line line)
    : m_line(std::move(line))
{
	updateData();
}

void LineGeometry::updateData()
{
	constexpr auto pointsCount = 2;
	QByteArray vertexes(pointsCount * cPointDataSize, Qt::Uninitialized);

	setPoint(vertexes, 0, m_line.p1);
	setPoint(vertexes, 1, m_line.p2);

	clear();
	setVertexData(vertexes);
	setStride(cPointDataSize);
	setBounds(m_line.p1, m_line.p2);

	setPrimitiveType(PrimitiveType::Lines);
	addAttribute(Attribute::PositionSemantic, 0, Attribute::F32Type);
	update();
}

QVector3D LineGeometry::p1() const
{
	return m_line.p1;
}

void LineGeometry::setP1(QVector3D newP1)
{
	if (m_line.p1 == newP1) {
		return;
	}
	m_line.p1 = newP1;

	emit p1Changed();
	updateData();
}

QVector3D LineGeometry::p2() const
{
	return m_line.p2;
}

void LineGeometry::setP2(QVector3D newP2)
{
	if (m_line.p2 == newP2) {
		return;
	}
	m_line.p2 = newP2;

	emit p2Changed();
	updateData();
}

StreightLine LineGeometry::toStraightLine() const
{
	return StreightLine::FromLine(m_line);
}

Line LineGeometry::toLine() const
{
	return m_line;
}

QDebug operator<<(QDebug dbg, const PointGeometry &geom)
{
	QDebugStateSaver stateSaver(dbg);
	dbg.nospace() << QString("Point{%1}").arg(toString(geom));
	return dbg;
}

QDebug operator<<(QDebug dbg, const StreightLine &sLine)
{
	QDebugStateSaver stateSaver(dbg);
	dbg << std::format("Line{{A={:.2}, B={:.2}, C={:.2}}}", sLine.A, sLine.B, sLine.C);
	return dbg;
}

QDebug operator<<(QDebug dbg, const LineGeometry &geom)
{
	QDebugStateSaver stateSaver(dbg);
	dbg.nospace() << QString("Line{%1, %2}").arg(toString(geom.p1()), toString(geom.p2()));
	return dbg;
}
