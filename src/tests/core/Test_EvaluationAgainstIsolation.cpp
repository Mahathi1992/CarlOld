#include "gtest/gtest.h"
#include <map>

#include "carl/core/UnivariatePolynomial.h"
#include "carl/formula/model/ran/RealAlgebraicNumber.h"
#include "carl/formula/model/ran/interval/ran_interval_evaluation.h"

#include "../Common.h"

using namespace carl;

TEST(IsolationAndEvaluation, Comparison) {
	Variable a = freshRealVariable("a");
	Variable b = freshRealVariable("b");
	Variable c = freshRealVariable("c");
	Variable d = freshRealVariable("d");

	MultivariatePolynomial<Rational> poly(Rational(-11) + Rational(49) * b + Rational(10) * d * a + carl::pow(c, 2) + Rational(-49) * b * c);

	Rational a_value = Rational(0);

	Interval<Rational> i1(Rational("-4248605910149717961169920635929051829594223322247294470949631829") / Rational("1102359498361667329021785995346037545330231253734995885016743936"), BoundType::STRICT, -Rational("-3717530171381003216023680556437920350894945406966382662080927793") / Rational("964564561066458912894062745927782852163952347018121399389650944"), BoundType::STRICT);
	UnivariatePolynomial<Rational> p1(freshRealVariable("__r1"), std::initializer_list<Rational>{-11, 1, 1});
	RealAlgebraicNumber<Rational> b_value = RealAlgebraicNumber<Rational>::create_safe(p1, i1);

	Interval<Rational> i2(Rational("-1245") / Rational("76832"), BoundType::STRICT, -Rational("-22409") / Rational("1382976"), BoundType::STRICT);
	UnivariatePolynomial<Rational> p2(freshRealVariable("__r2"), std::initializer_list<Rational>{-11, -1029, -21609});
	RealAlgebraicNumber<Rational> c_value = RealAlgebraicNumber<Rational>::create_safe(p2, i2);

	Rational d_value = Rational(-1);

	std::vector<variable> variableOrder{d, c, b, a};
	std::map<carl::Variable, RAN> varToRANMap;
	varToRANMap[variableOrder[0]] = d_value;
	varToRANMap[variableOrder[1]] = c_value;
	varToRANMap[variableOrder[2]] = b_value;
	varToRANMap[variableOrder[3]] = a_value;

	bool b1 = carl::evaluate(ConstraintT(poly, carl::Relation::EQ), varToRANMap);

	auto isolatedRoots = carl::realRoots(carl::to_univariate_polynomial(poly, variableOrder[3]), prefixPointToStdMap(3));
	std::size_t rootIdx = 0;
	bool b2 = false;
	for (const auto& root : isolatedRoots) {
		rootIdx++;
		if (root == pointComp) {
			b2 = true;
		}
	}

	EXPECT_EQ(b1, b2);
}
