/** 
 * @file:   numbers.h
 * @author: Sebastian Junges
 *
 * @since August 26, 2013
 */

#pragma once
#include <cln/cln.h>
#include <gmpxx.h>

namespace carl
{
	//
	// Type traits.
	// For type traits, we use the notation conventions of std, being lower cases with underscores.
	//

	/**
	 * Type trait  is_field. 
	 * Default is false, but certain types which encode algebraic fields should be set to true. 
	 * @see UnivariatePolynomial - CauchyBound for example.
	 */
	template<typename type>
	struct is_field
	{
		static const bool value = false;
	};
	
	template<>
	struct is_field<cln::cl_RA>
	{
		static const bool value = true;
	};
	
	template<>
	struct is_field<mpq_class>
	{
		static const bool value = true;
	};
	
	
	
	template<typename RationalType>
	struct IntegralT
	{
		typedef int type;
	};

	template<>
	struct IntegralT<cln::cl_RA>
	{
		typedef cln::cl_I type;
	};
	
	template<>
	struct IntegralT<mpq_class>
	{
		typedef mpz_class type;
	};
        
        

	inline cln::cl_I getNum(const cln::cl_RA& rat)
	{
		return cln::numerator(rat);
	}
	
	inline cln::cl_I getDenom(const cln::cl_RA& rat)
	{
		return cln::denominator(rat);
	}
	
	inline cln::cl_RA pow(const cln::cl_RA& base, unsigned exp)
	{
		return cln::expt(base, exp);
	}
        
        inline double getDouble( const cln::cl_RA& rational )
        {
            return cln::double_approx(rational);
        }
        
		template<typename t>
        inline t rationalize( double d )
        {
            return cln::rationalize( cln::cl_R(d) );
        }
        
		
        template<>
        inline cln::cl_RA rationalize<cln::cl_RA>( double d )
        {
            return cln::rationalize( cln::cl_R(d) );
        }
	
	
	inline const mpz_class& getNum(const mpq_class& rat)
	{
		return rat.get_num();
	}
	
	inline const mpz_class& getDenom(const mpq_class& rat)
	{
		return rat.get_den();
	}
        
        inline double getDouble( const mpq_class& rational )
        {
            return rational.get_d();
        }
        
        template<>
        inline mpq_class rationalize<mpq_class>( double d )
        {
            return mpq_class(d);
        }
	
        inline double getDouble( const int& rational )
        {
            return double(rational);
        }
        
	inline mpz_class gcd(const mpz_class& v1, const mpz_class& v2)
	{
		mpz_class res;
		mpz_gcd(res.get_mpz_t(), v1.get_mpz_t(), v2.get_mpz_t());
		return res;
	}
	
	inline mpz_class lcm(const mpz_class& v1, const mpz_class& v2)
	{
		mpz_class res;
		mpz_lcm(res.get_mpz_t(), v1.get_mpz_t(), v2.get_mpz_t());
		return res;
	}
	
}