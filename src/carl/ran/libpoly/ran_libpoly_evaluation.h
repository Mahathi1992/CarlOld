#pragma once

#include <carl/config.h>

#ifdef RAN_USE_LIBPOLY

#include "ran_libpoly.h"

namespace carl {

template<typename Number>
std::optional<real_algebraic_number_libpoly<Number>> evaluate(
	const MultivariatePolynomial<Number>& polynomial,
	const std::map<Variable, real_algebraic_number_libpoly<Number>>& evalMap) {
	mpz_class denom;
	poly::Polynomial poly = LibpolyConverter::getInstance().toLibpolyPolynomial(polynomial, denom);
	CARL_LOG_DEBUG("carl.ran.libpoly", " Evaluation converted to poly: " << poly << " With denominator: " << denom);

	assert(denom != 0);
	//Turn into poly::Assignment
	poly::Assignment assignment;
	for (const auto& entry : evalMap) {
		lp_value_t val;
		//Turn into value
		lp_value_construct(&val, lp_value_type_t::LP_VALUE_ALGEBRAIC, entry.second.get_internal());
		//That copies the value into the assignment
		assignment.set(VariableMapper::getInstance().getLibpolyVariable(entry.first), poly::Value(&val));
		lp_value_destruct(&val);
	}

	lp_value_t* val = lp_polynomial_evaluate(poly.get_internal(), assignment.get_internal());

	if (val->type == lp_value_type_t::LP_VALUE_NONE) {
		CARL_LOG_DEBUG("carl.ran.libpoly", " Evaluation got nothing ");
		return std::nullopt;
	}

	//Turn value into algebraic number
	real_algebraic_number_libpoly<Number> res = real_algebraic_number_libpoly<Number>::create_from_value(val);

	lp_value_delete(val);

	//easy checks to avoid division
	if (res.is_zero() || denom == 1) {
		return res;
	} else if (denom == -1) {
		lp_algebraic_number_neg(res.get_internal(), res.get_internal());
		return res;
	}

	lp_algebraic_number_t ret_val;
	lp_algebraic_number_t algebraic_denom;

	lp_algebraic_number_construct_zero(&ret_val);

	//make denom an algebraic number
	lp_algebraic_number_construct_from_integer(&algebraic_denom, denom.get_mpz_t());

	lp_algebraic_number_div(&ret_val, res.get_internal(), &algebraic_denom);

	real_algebraic_number_libpoly<Number> ret(std::move(ret_val));

	lp_algebraic_number_destruct(&ret_val);
	lp_algebraic_number_destruct(&algebraic_denom);

	CARL_LOG_DEBUG("carl.ran.libpoly", " Evaluation got " << ret);
	return std::optional<real_algebraic_number_libpoly<Number>>(std::move(ret));
}

template<typename Number, typename Poly>
boost::tribool evaluate(const Constraint<Poly>& constraint, const std::map<Variable, real_algebraic_number_libpoly<Number>>& evalMap) {

	CARL_LOG_DEBUG("carl.ran.libpoly", " Evaluation constraint " << constraint << " for assignment " << evalMap);

	//Easy checks of lhs of constraint is constant
	if (constraint.lhs().isConstant()) {
		auto num = constraint.lhs().constantPart();
		switch (constraint.relation()) {
		case Relation::EQ:
			return num == 0;
		case Relation::NEQ:
			return num != 0;
		case Relation::LESS:
			return num < 0;
		case Relation::LEQ:
			return num <= 0;
		case Relation::GREATER:
			return num > 0;
		case Relation::GEQ:
			return num >= 0;
		default:
			assert(false);
			return false;
		}
	}

	//denominator can be omitted
	poly::Polynomial poly_pol = LibpolyConverter::getInstance().toLibpolyPolynomial(constraint.lhs());

	//Turn into poly::Assignment
	poly::Assignment assignment;
	for (const auto& entry : evalMap) {
		lp_value_t val;
		//Turn into value
		lp_value_construct(&val, lp_value_type_t::LP_VALUE_ALGEBRAIC, entry.second.get_internal());
		//That copies the value into the assignment
		assignment.set(VariableMapper::getInstance().getLibpolyVariable(entry.first), poly::Value(&val));
		lp_value_destruct(&val);
	}

	switch (constraint.relation()) {
	case Relation::EQ:
		return evaluate_constraint(poly_pol, assignment, poly::SignCondition::EQ);
	case Relation::NEQ:
		return evaluate_constraint(poly_pol, assignment, poly::SignCondition::NE);
	case Relation::LESS:
		return evaluate_constraint(poly_pol, assignment, poly::SignCondition::LT);
	case Relation::LEQ:
		return evaluate_constraint(poly_pol, assignment, poly::SignCondition::LE);
	case Relation::GREATER:
		return evaluate_constraint(poly_pol, assignment, poly::SignCondition::GT);
	case Relation::GEQ:
		return evaluate_constraint(poly_pol, assignment, poly::SignCondition::GE);
	default:
		assert(false);
		return false;
	}
}

namespace ran::interval {
template<typename Coeff, typename Number>
bool vanishes(
	const UnivariatePolynomial<Coeff>& poly,
	const std::map<Variable, real_algebraic_number_libpoly<Number>>& varToRANMap) {
	//taken from ran interval
	if (carl::isZero(poly)) {
		CARL_LOG_TRACE("carl.ran", "poly is 0");
		return true;
	}
	if (poly.isNumber()) {
		CARL_LOG_TRACE("carl.ran", "poly is constant but not zero");
		return false;
	}

	return real_roots(poly, varToRANMap).is_nullified();
}
} // namespace ran::interval

} // namespace carl

#endif