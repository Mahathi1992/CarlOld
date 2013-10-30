#include "gtest/gtest.h"
#include "../../carl/core/MultivariateHensel.h"
#include <gmpxx.h>

using namespace carl;

TEST(Diophantine, Constructor)
{
	VariablePool& vpool = VariablePool::getInstance();
    Variable x = vpool.getFreshVariable();
    vpool.setVariableName(x, "x");
    UnivariatePolynomial<mpz_class> A1(x, {(mpz_class)0, (mpz_class)2});
	const GaloisField<mpz_class>* gf5 = new GaloisField<mpz_class>(5);
    UnivariatePolynomial<GFNumber<mpz_class>> a1 = A1.toFiniteDomain(gf5);
	UnivariatePolynomial<mpz_class> A2(x, 1, 5);
	UnivariatePolynomial<GFNumber<mpz_class>> a2 = A2.toFiniteDomain(gf5);
    DiophantineEquations<mpz_class> d(5,1);
	
	std::vector<UnivariatePolynomial<GFNumber<mpz_class>>> result = d.univariateDiophantine({a1,a2}, x, 4);
	EXPECT_EQ((unsigned)2,result.size());
	std::cout << result.front() << std::endl;
	std::cout << result.back() << std::endl;
}
