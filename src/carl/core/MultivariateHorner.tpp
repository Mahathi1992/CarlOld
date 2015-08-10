 /**
 * @file	MultivariateHorner.tpp
 * @author	Lukas Netz
 *
 */

#pragma once
#include "carl/core/Term.h"

 #ifndef HORNER_Minimize_arithmetic_operations
 #define HORNER_Minimize_arithmetic_operations 1
 #endif

namespace carl
{
	template< typename PolynomialType > 
	MultivariateHorner< PolynomialType >::MultivariateHorner (const PolynomialType& inPut) {

		if (HORNER_Minimize_arithmetic_operations)
		{		
			std::set<Variable>::iterator variableIt;
			std::set<Variable>::iterator selectedVariable;
			std::set<Variable> allVariablesinPolynome;
			inPut.gatherVariables(allVariablesinPolynome);

			unsigned int monomials_containingChoosenVar = 0;

			if (allVariablesinPolynome.size() != 0)
			{
				//Detecting amounts of Variables in Monomials
				for (variableIt = allVariablesinPolynome.begin(); variableIt != allVariablesinPolynome.end(); variableIt++)
				{	
					unsigned int monomialCounter = 0;
					typename PolynomialType::TermsType::const_iterator polynomialIt;
					for (polynomialIt = inPut.begin(); polynomialIt != inPut.end(); polynomialIt++)
					{
						if (polynomialIt->has(*variableIt))
						{
							monomialCounter++;
						}
					}
					
					//saving most promising Variable for later 
					if ( monomialCounter >= monomials_containingChoosenVar ) {
						monomials_containingChoosenVar = monomialCounter;
						selectedVariable = variableIt;					
					}
				}

				//Setting the choosen Variable for the current Hornerscheme iterartion
				this->setVariable(*selectedVariable); 

				#ifdef DEBUG_HORNER
				std::cout << "Polynome: " << inPut << std::endl;
				std::cout << "  Choosen Var: " << *selectedVariable << std::endl;
 				#endif
				
				typename PolynomialType::TermsType::const_iterator polynomialIt;
				typename PolynomialType::TermType tempTerm;

				typename PolynomialType::PolyType h_independentPart;
				typename PolynomialType::PolyType h_dependentPart;

				//Choose Terms from Polynome denpending on dependency on choosen Variable
				for (polynomialIt = inPut.begin(); polynomialIt != inPut.end(); polynomialIt++)
				{
					if (polynomialIt->has(*selectedVariable))
					{
						//divide dependent terms by choosen Variable
						polynomialIt->divide(*selectedVariable, tempTerm);
						h_dependentPart.addTerm( tempTerm );
					}
					else 
					{
						h_independentPart.addTerm( *polynomialIt );
					}
				}

				//If Dependent Polynome contains Variables - continue with recursive Horner
				if ( !h_dependentPart.isNumber() )
				{
					mH_dependent = std::move(MultivariateHorner< PolynomialType >(h_dependentPart));
					mConst_dependent = constant_zero<CoeffType>::get();	
				}
				
				//Dependent Polynome is a Constant (Number) - save to memberVar
				else 
				{
					removeDependent();
					mConst_dependent = (PolynomialType) h_dependentPart.constantPart();
				}			

				//If independent Polynome contains Variables - continue with recursive Horner
				if ( !h_independentPart.isNumber() )
				{
					mH_independent = std::move(MultivariateHorner< PolynomialType >(h_independentPart));
					mConst_independent = constant_zero<CoeffType>::get();
				}
				//Independent Polynome is a Constant (Number) - save to memberVar
				else
				{
					removeIndepenent();
					mConst_independent = (PolynomialType) h_independentPart.constantPart();
				}					
			} 
						
			//If there are no Variables in the polynome
			else 
			{		
				removeIndepenent();
				removeDependent();
				mConst_independent = (PolynomialType) inPut.constantPart();
				this->setVariable( Variable::NO_VARIABLE );
			}
			

		}//minimize arithmatic operations
	};


/*
	template< typename PolynomialType > 
	MultivariateHorner< PolynomialType >::~MultivariateHorner()
	{
		if (mH_dependent != NULL) 
		{
			delete mH_dependent;
		}
		if (mH_independent != NULL)
		{
			delete mH_independent;
		}
	}
*/

/**
	 * Streaming operator for multivariate HornerSchemes.
	 * @param os Output stream.
	 * @param rhs HornerScheme.
	 * @return `os`.
	 */
template< typename PolynomialType > 
std::ostream& operator<<(std::ostream& os,const MultivariateHorner<PolynomialType>& mvH)
{

	if (mvH.getDependent() != NULL && mvH.getIndependent() != NULL)
	{
		if (mvH.getExponent() != 1)
		{
			return (os << mvH.getVariable() <<"^"<< mvH.getExponent() << " * (" << *mvH.getDependent() << ") + " << *mvH.getIndependent());		
		}
		else
		{
			return (os << mvH.getVariable() << " * (" << *mvH.getDependent() << ") + " << *mvH.getIndependent());			
		}
		
	}
	else if (mvH.getDependent() != NULL && mvH.getIndependent() == NULL)
	{
		if (mvH.getIndepConstant() == 0)
		{
			if (mvH.getExponent() != 1)
			{
				return (os << mvH.getVariable() <<"^"<< mvH.getExponent() << " * (" << *mvH.getDependent() << ")" );			
			}
			else
			{
				return (os << mvH.getVariable() << " * (" << *mvH.getDependent() << ")" );				
			}
		}
		else
		{
			if (mvH.getExponent() != 1)
			{
				return (os << mvH.getVariable() <<"^"<< mvH.getExponent() << " * (" << *mvH.getDependent() << ") + " << mvH.getIndepConstant());	
			}
			else
			{
				return (os << mvH.getVariable() << " * (" << *mvH.getDependent() << ") + " << mvH.getIndepConstant());		
			}
		}	
	}
	else if (mvH.getDependent() == NULL && mvH.getIndependent() != NULL)
	{
		if (mvH.getDepConstant() == 1)
		{	
			if (mvH.getExponent() != 1)
			{
				return (os << mvH.getVariable() <<"^"<< mvH.getExponent() << " + " << *mvH.getIndependent() );			
			}
			else
			{
				return (os << mvH.getVariable() << " + " << *mvH.getIndependent() );		
			}			
		}
		else
		{	
			if (mvH.getExponent() != 1)
			{
				return (os << mvH.getDepConstant() << mvH.getVariable() <<"^"<< mvH.getExponent() << " + " << *mvH.getIndependent() );		
			}
			else
			{
				return (os << mvH.getDepConstant() << mvH.getVariable() <<" + " << *mvH.getIndependent() );			
			}
		}
	}	
	else //if (mvH.getDependent() == NULL && mvH.getIndependent() == NULL)
	{
		if (mvH.getIndepConstant() == 0)
		{
			if (mvH.getDepConstant() == 1)
			{
				if (mvH.getExponent() != 1)
				{
					return (os << mvH.getVariable() <<"^"<< mvH.getExponent() );	
				}
				else
				{
					return (os << mvH.getVariable());	
				}
				
			}
			else
			{
				if (mvH.getExponent() != 1)
				{
					return (os << mvH.getDepConstant() << mvH.getVariable() <<"^"<< mvH.getExponent());	
				}
				else
				{
					return (os << mvH.getDepConstant() << mvH.getVariable());	
				}
			}
		}
		else
		{
			if (mvH.getExponent() != 1)
			{
				return (os << mvH.getDepConstant() << mvH.getVariable() <<"^"<< mvH.getExponent() << " + " << mvH.getIndepConstant());	
			}
			else
			{
				return (os << mvH.getDepConstant() << mvH.getVariable() <<" + " << mvH.getIndepConstant());	
			}

		}
	}
}

template<typename PolynomialType>
MultivariateHorner<PolynomialType> simplify( MultivariateHorner<PolynomialType> mvH)
{
	#ifdef DEBUG_HORNER
	std::cout << mvH << std::endl;	
	#endif
	
	if (mvH->getDependent() != NULL && (mvH->getDependent()->getDependent() != NULL || mvH->getDependent()->getDepConstant() != 0) && mvH->getDependent()->getIndependent() == NULL && mvH->getDependent()->getIndepConstant() == 0 )
	{

		if (mvH->getVariable() == mvH->getDependent()->getVariable())
		{
			mvH->setExponent (mvH->getExponent() + mvH->getDependent()->getExponent()) ;

			if (mvH->getDependent()->getDependent() != NULL)
			{
				mvH->setDependent(simplify(mvH->getDependent()->getDependent()) );	
			} 
			else if (mvH->getDependent()->getDepConstant() != 0)
			{
				mvH->setDepConstant(mvH->getDependent()->getDepConstant() );
				mvH->removeDependent();
			}

			if (mvH->getIndependent() != NULL)
			{	
				mvH->setIndependent(simplify(mvH->getIndependent() ));
			}
	
			return ( simplify(mvH) );	
		}
	}

	else if (mvH->getDependent() == NULL && mvH->getIndependent() != NULL)
	{
		mvH->setIndependent(simplify (mvH->getIndependent()));
		return mvH;
	}

	else if (mvH->getDependent() != NULL && mvH->getIndependent() == NULL)
	{
		mvH->setDependent(simplify (mvH->getDependent()));
		return mvH;
	}
	
	else if (mvH->getDependent() != NULL && mvH->getIndependent() != NULL)
	{
		mvH->setDependent(simplify(mvH->getDependent()));
		mvH->setIndependent(simplify (mvH->getIndependent()));
		return mvH;
	}
	
	return(mvH);
}

//template<typename PolynomialType>
//typedef typename MultivariatePolynomial<PolynomialType>::CoeffType CoeffType;
template<typename PolynomialType, typename Number>
static Interval<Number> evaluate(const MultivariateHorner<PolynomialType> mvH, std::map<Variable, Interval<Number>>& map)
{
	Interval<Number> result(1);

	assert (map.find(mvH.getVariable()) != map.end() );
	
	//Case 1: no further Horner schemes in mvH
	if (mvH.getDependent() == NULL && mvH.getIndependent() == NULL)
	{
		result = map.find(mvH.getVariable())->second.pow(mvH.getExponent()) * mvH.getDepConstant().constantPart() + mvH.getIndepConstant().constantPart();
		return result;
	}
	//Case 2: dependent part contains a Horner Scheme
	else if (mvH.getDependent() != NULL && mvH.getIndependent() == NULL)
	{
		result = map.find(mvH.getVariable())->second.pow(mvH.getExponent()) * evaluate(*mvH.getDependent(), map) + mvH.getIndepConstant().constantPart();
		return result;
	}
	//Case 3: independent part contains a Horner Scheme
	else if (mvH.getDependent() == NULL && mvH.getIndependent() != NULL)
	{
		result = map.find(mvH.getVariable())->second.pow(mvH.getExponent()) * mvH.getDepConstant().constantPart() +  evaluate(*mvH.getIndependent(), map);
		return result;
	}
	//Case 4: both independent part and dependent part 
	else if (mvH.getDependent() != NULL && mvH.getIndependent() != NULL)
	{
		result = map.find(mvH.getVariable())->second.pow(mvH.getExponent()) * evaluate(*mvH.getDependent(), map) + evaluate(*mvH.getIndependent(), map);
		return result;
	}

	return result;
}


}//namespace carl