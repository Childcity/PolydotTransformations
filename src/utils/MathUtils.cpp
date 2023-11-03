#include "MathUtils.h"

#include <ranges>

#include <QLoggingCategory>
#include <QQmlEngine>

#include <models/GeometryPrimitives.h>

#include "GaussJordanElimination.h"

namespace {

Q_LOGGING_CATEGORY(polydot_line, "utils.polydot_line", QtInfoMsg);

constexpr float eps = std::numeric_limits<float>().epsilon();

//  By|          .
//  Ay| |B|   .       .
//    |    .     .
//    |  .  .
//  1 |.        |A|
//    |_________________
//     1        Bx    Ax

// Dot Product
// https://math.stackexchange.com/questions/805954/what-does-the-dot-product-of-two-vectors-represent
// Ax*Bx + Ay*By = |A||B|cos(th)

float signDistanceToLine(
    QVector3D point, // Point
    QVector3D linePoint, // Point on the Line
    QVector3D direction // Line direction vector
)
{
	auto signLength = [](QVector3D vec) {
		float sign = vec.x() + vec.y() + vec.z() < eps ? -1 : 1;
		return sign * std::hypot(vec.x(), vec.y(), vec.z());
	};

	if (direction.isNull()) {
		auto delta = point - linePoint;
		return signLength(delta);
	}

	QVector3D p = linePoint + QVector3D::dotProduct(point - linePoint, direction) * direction;
	auto delta = point - p;
	return signLength(delta);
}

} // namespace

QObject *MathUtils::CreateInstance(QQmlEngine *engine, QJSEngine *)
{
	return new MathUtils(engine);
}

MathUtils::MathUtils(QObject *parent)
    :

    QObject(parent)
{
}

LineGeometry *MathUtils::getPolydotLine(
    LineGeometry *baseLine, const QVariantList &origBasises, const QVariantList &resBasises) const
{
	namespace solve = gauss_jordan_elimination;

	float a1{}, b1{}, c1{}, r1{}, //
	    a2{}, b2{}, c2{}, r2{}, //
	    a3{}, b3{}, c3{}, r3{};

	// https://brilliant.org/wiki/3d-coordinate-geometry-equation-of-a-line/
	// Subtracting the position vectors of the two points gives the direction vector
	const auto baseLineNormilized = baseLine->toStraightLine().normilized();
	QVector3D baseLineDirection = baseLine->p1() - baseLine->p2();
	baseLineDirection.normalize();

	{
		qCDebug(polydot_line) << "Base Normilized Line: " << baseLineNormilized;
		qCDebug(polydot_line).noquote() << std::format(
		    "\n       x - {}      y - {}     z - {} "
		    "\nLine: -------- = -------- = -------- "
		    "\n       {:.2}       {:.2}      {:.2}  ",
		    baseLine->p1().x(),
		    baseLine->p1().y(),
		    baseLine->p1().z(),
		    baseLineDirection.x(),
		    baseLineDirection.y(),
		    baseLineDirection.z() //
		);
	}

	for (const auto &[originalBasis, resultBasis] : std::views::zip(origBasises, resBasises)) {
		const auto &origBasis = *originalBasis.value<PointGeometry *>();
		const auto &resBasis = *resultBasis.value<PointGeometry *>();

		float betta = baseLineNormilized.signDistanceToPoint(origBasis);
		float betta2 = signDistanceToLine(origBasis, baseLine->p1(), baseLineDirection);
		float betta3 = signDistanceToLine(origBasis, baseLine->p2(), baseLineDirection);

		if (qFuzzyIsNull(betta)) {
			// If distance from origBasis to Line is 0 => we cannot solve system!
			return {};
		}

		assert(qFuzzyCompare(betta, betta2));
		assert(qFuzzyCompare(betta, betta3));

		qCDebug(polydot_line) << "Original Basis" << origBasis;
		qCDebug(polydot_line) << "Result   Basis" << resBasis;
		qCDebug(polydot_line) << "Betta" << betta << betta2 << betta3;
		qCDebug(polydot_line) << "-----------------------------------------";

		float bettaSqure = betta * betta;

		float x = resBasis.x();
		float y = resBasis.y();

		a1 += x * x / bettaSqure;
		b1 += x * y / bettaSqure;
		c1 += x / bettaSqure;
		r1 += x / betta;

		a2 += y * x / bettaSqure;
		b2 += y * y / bettaSqure;
		c2 += y / bettaSqure;
		r2 += y / betta;

		a3 += x / bettaSqure;
		b3 += y / bettaSqure;
		c3 += 1.f / bettaSqure;
		r3 += 1.f / betta;
	}

	const solve::Matrix polidotEquestion = {
	    {a1, b1, c1, r1},
	    {a2, b2, c2, r2},
	    {a3, b3, c3, r3},
	};

	try {
		const auto polidotSolutionArr = solve::SolveSystem(polidotEquestion);
		assert(polidotSolutionArr.size() == 3);

		const auto resLine = StreightLine::FromArray(polidotSolutionArr);
		qCDebug(polydot_line) << "Result line" << resLine;

		return new LineGeometry(resLine);
	} catch (const std::runtime_error &e) {
		qCWarning(polydot_line) << "System cannot be solved: " << e.what();
		return {};
	}
}
