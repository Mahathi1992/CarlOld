#pragma once
#include <memory>
#include <list>
#include "MultivariatePolynomial.h"
#include "logging.h"

namespace carl
{



template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>::MultivariatePolynomial(Variable::Arg v) :
mTerms(1,std::make_shared<const Term<Coeff>>(v))
{
    
}
template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>::MultivariatePolynomial(const Monomial& m) :
mTerms(1,std::make_shared<const Term<Coeff>>(m))
{
    
}
template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>::MultivariatePolynomial(const Term<Coeff>& t) :
mTerms(1,std::make_shared<const Term<Coeff>>(t))
{
    
}
template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>::MultivariatePolynomial(std::shared_ptr<const Monomial> m)
{
    LOG_NOTIMPLEMENTED();
}
template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>::MultivariatePolynomial(std::shared_ptr<const Term<Coeff>> t)
{
    LOG_NOTIMPLEMENTED();
}
template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>::MultivariatePolynomial(const UnivariatePolynomial<MultivariatePolynomial<Coeff, Policy>>& pol)
{
    LOG_NOTIMPLEMENTED();
}
template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>::MultivariatePolynomial(const UnivariatePolynomial<Coeff>& pol)
{
    LOG_NOTIMPLEMENTED();
}

template<typename Coeff, typename Policy>
template<typename InputIterator>
MultivariatePolynomial<Coeff,Policy>::MultivariatePolynomial(InputIterator begin, InputIterator end)
{
    mTerms.assign(begin, end);
    sortTerms();
}
    
template<typename Coeff, typename Policy>
std::shared_ptr<const Monomial> MultivariatePolynomial<Coeff,Policy>::lmon() const
{
    return lterm()->monomial();
}
template<typename Coeff, typename Policy>
std::shared_ptr<const Term<Coeff>> MultivariatePolynomial<Coeff,Policy>::lterm() const
{
    LOG_ASSERT(!isZero(), "Leading term undefined on zero polynomials.");
	return mTerms.back();
    
}
template<typename Coeff, typename Policy>
Coeff MultivariatePolynomial<Coeff,Policy>::lcoeff() const
{
    return lterm()->coeff();
}

template<typename Coeff, typename Policy>
exponent MultivariatePolynomial<Coeff,Policy>::highestDegree() const
{
    if(mTerms.size == 0) return 0;
    if(Policy::Ordering::degreeOrder)
    {
        return mTerms.back()->tdeg();
    }
    else 
    {
        LOG_NOTIMPLEMENTED();
    }
}

template<typename Coeff, typename Policy>
bool MultivariatePolynomial<Coeff,Policy>::isZero() const
{
    return mTerms.empty();
}
template<typename Coeff, typename Policy>
bool MultivariatePolynomial<Coeff,Policy>::isConstant() const
{
    return (mTerms.size == 0) || (mTerms.size == 1 && mTerms.front()->isConstant());
}
template<typename Coeff, typename Policy>
bool MultivariatePolynomial<Coeff,Policy>::isLinear() const
{
    if(mTerms.size == 0) return true;
    if(Policy::Ordering::degreeOrder)
    {
        return mTerms.back()->isLinear();
    }
    else 
    {
        LOG_NOTIMPLEMENTED();
    }
}

template<typename Coeff, typename Policy>
std::shared_ptr<const Term<Coeff>> MultivariatePolynomial<Coeff,Policy>::constantPart() const
{
    if(mTerms.size == 0) return true;
    if(Policy::Ordering::degreeOrder)
    {
        if(mTerms.front()->isConstant()) return mTerms.back();
    }
    else 
    {
        LOG_NOTIMPLEMENTED();
    }
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy> MultivariatePolynomial<Coeff,Policy>::tail() const
{
	assert(mTerms.size() != 0);
	if (mTerms.size() == 1) return MultivariatePolynomial();
    MultivariatePolynomial tail;
	tail.mTerms.reserve(mTerms.size()-1);
    tail.mTerms.insert(tail.mTerms.begin(), mTerms.begin(), --mTerms.end());
    return tail;
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>& MultivariatePolynomial<Coeff,Policy>::stripLT()
{
    assert(!isZero());
    mTerms.pop_back();
    return *this;
}

template<typename Coeff, typename Policy>
bool MultivariatePolynomial<Coeff,Policy>::isTsos() const
{
    LOG_NOTIMPLEMENTED();
    return false;
}

template<typename C, typename P>
bool operator==( const MultivariatePolynomial<C,P>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    if(lhs.mTerms.size() != rhs.mTerms.size()) return false;
    // Compare vector entries. We cannot use std::vector== as we not only want to compare the pointers.
    return std::equal(lhs.mTerms.begin(), lhs.mTerms.end(), rhs.mTerms.begin(),
                    [](const std::shared_ptr<const Term<C>>& lterm, const std::shared_ptr<const Term<C>>& rterm) 
                    -> bool 
                    {
                        return lterm == rterm || *lterm == *rterm;
                    });
}
template<typename C, typename P>
bool operator==(const UnivariatePolynomial<C>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    LOG_NOTIMPLEMENTED();
    return false;
}
template<typename C, typename P>
bool operator==(const MultivariatePolynomial<C,P>& lhs, const UnivariatePolynomial<C>& rhs)
{
    return rhs == lhs;
}
template<typename C, typename P>
bool operator==(const UnivariatePolynomial<MultivariatePolynomial<C>>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    LOG_NOTIMPLEMENTED();
    return false;
}
template<typename C, typename P>
bool operator==(const MultivariatePolynomial<C,P>& lhs, const UnivariatePolynomial<MultivariatePolynomial<C>>& rhs)
{
    return rhs == lhs;
}
template<typename C, typename P>
bool operator==(const MultivariatePolynomial<C,P>& lhs, const Term<C>& rhs)
{
    if(lhs.mTerms.empty() && rhs.coeff() == 0) return true;
    if(lhs.mTerms.size() > 1) return false;
    return *(lhs.mTerms.front()) == rhs;
}
template<typename C, typename P>
bool operator==(const Term<C>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return rhs == lhs;
}
template<typename C, typename P>
bool operator==(const MultivariatePolynomial<C,P>& lhs, const Monomial& rhs)
{
    if(lhs.mTerms.size() != 1) return false;
    return *(lhs.mTerms.front()->monomial) == rhs;
}
template<typename C, typename P>
bool operator==(const Monomial& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return rhs == lhs;
}
template<typename C, typename P>
bool operator==(const MultivariatePolynomial<C,P>& lhs, const C& rhs)
{
    if(lhs.mTerms.empty() && rhs == 0) return true;
    if(lhs.mTerms.size() > 1) return false;
    return *(lhs.mTerms.front()->coeff()) == rhs;
}
template<typename C, typename P>
bool operator==(const C& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return rhs == lhs;
}
template<typename C, typename P>
bool operator==(const MultivariatePolynomial<C,P>& lhs, Variable::Arg rhs)
{
    if(lhs.mTerms.size() != 1) return false;
    return *(lhs.mTerms.front()->monomial()) == rhs;
}
template<typename C, typename P>
bool operator==(Variable::Arg lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return rhs == lhs;
}

template<typename C, typename P>
bool operator!=( const MultivariatePolynomial<C,P>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const UnivariatePolynomial<C>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const MultivariatePolynomial<C,P>& lhs, const UnivariatePolynomial<C>& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const UnivariatePolynomial<MultivariatePolynomial<C>>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const MultivariatePolynomial<C,P>& lhs, const UnivariatePolynomial<MultivariatePolynomial<C>>& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const MultivariatePolynomial<C,P>& lhs, const Term<C>& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const Term<C>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const MultivariatePolynomial<C,P>& lhs, const Monomial& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const Monomial& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const MultivariatePolynomial<C,P>& lhs, const C& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const C& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(const MultivariatePolynomial<C,P>& lhs, Variable::Arg rhs)
{
    return lhs != rhs;
}
template<typename C, typename P>
bool operator!=(Variable::Arg lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return lhs != rhs;
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff, Policy>& MultivariatePolynomial<Coeff, Policy>::operator+=(const MultivariatePolynomial& rhs)
{
    if(mTerms.size() == 0) mTerms = rhs.mTerms;
    if(rhs.mTerms.size() == 0) return *this;
    
    TermsType newTerms;
    newTerms.reserve(mTerms.size() + rhs.mTerms.size());
    typename TermsType::const_iterator lhsIt(mTerms.begin());
    typename TermsType::const_iterator rhsIt(rhs.mTerms.begin());
    while(true)
    {
        CompareResult cmpres(Policy::Ordering::compare(**lhsIt, **rhsIt));
        if(cmpres == CompareResult::LESS)
        {
            newTerms.push_back(*lhsIt);
            if(++lhsIt != mTerms.end()) break;
        }
        else if(cmpres == CompareResult::GREATER)
        {
            newTerms.push_back(*rhsIt);
            if(++rhsIt != rhs.mTerms.end()) break;
        }
        else 
        {
            if( (**lhsIt).coeff() != -(**rhsIt).coeff() )
            {
                newTerms.push_back(std::make_shared<const Term<Coeff>>( (**lhsIt).coeff() + (**rhsIt).coeff(), (**lhsIt).monomial() ));
            }
            ++lhsIt;
            ++rhsIt;
            if(lhsIt == mTerms.end() || rhsIt == rhs.mTerms.end() ) break;
        }
    }
    newTerms.insert(newTerms.end(), lhsIt, mTerms.cend());
    newTerms.insert(newTerms.end(), rhsIt, rhs.mTerms.cend());
    
    mTerms = std::move(newTerms);
    return *this;
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff, Policy>& MultivariatePolynomial<Coeff, Policy>::operator+=(const Term<Coeff>& rhs)
{
    if(rhs.coeff() == 0) return *this;
    if(Policy::searchLinear) 
    {
        typename TermsType::iterator it(mTerms.begin());
        while(it != mTerms.end())
        {
            // TODO consider comparing the shared pointers.
            if( (**it).isConstant() ) break;
            CompareResult cmpres(Policy::Ordering::compare((**it), rhs));
            if( cmpres == CompareResult::GREATER ) break;
            if( cmpres == CompareResult::EQUAL )
            {
                // new coefficient would be zero, simply removing is enough.
                if((**it).coeff() == -rhs.coeff())
                {
                    mTerms.erase(it);
                }
                // we have to create a new term object. 
                else
                {
                    *it = std::make_shared<const Term<Coeff>>((**it).coeff()+rhs.coeff(), (**it).monomial());
                }
                return *this;
            }
            ++it;    
        }
        // no equal monomial does occur. We can simply insert.
        mTerms.insert(it,std::make_shared<const Term<Coeff>>(rhs));
        return *this;
    }
    else 
    {
        LOG_NOTIMPLEMENTED();
    }
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff, Policy>& MultivariatePolynomial<Coeff, Policy>::operator+=(const Monomial& rhs)
{
    if(rhs.tdeg() == 0) return *this;
    if(Policy::searchLinear) 
    {
        typename TermsType::iterator it(mTerms.begin());
        while(it != mTerms.end())
        {
            if( (**it).isConstant() ) break;
            CompareResult cmpres(Policy::Ordering::compare(*(**it).monomial(), rhs));
            if( cmpres == CompareResult::GREATER ) break;
            if( cmpres == CompareResult::EQUAL )
            {
                // new coefficient would be zero, simply removing is enough.
                if((**it).coeff() == -1)
                {
                    mTerms.erase(it);
                }
                // we have to create a new term object. 
                else
                {
                    *it = std::make_shared<const Term<Coeff>>((**it).coeff()+1, (**it).monomial());
                }
                return *this;
            }
            ++it;    
        }
        // no eq ual monomial does occur. We can simply insert.
        mTerms.insert(it,std::make_shared<const Term<Coeff>>(rhs));
        return *this;
    }
    else 
    {
        LOG_NOTIMPLEMENTED();
    }
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff, Policy>& MultivariatePolynomial<Coeff, Policy>::operator+=(const Variable::Arg rhs)
{
    if(Policy::searchLinear) 
    {
        typename TermsType::iterator it(mTerms.begin());
        while(it != mTerms.end())
        {
            CompareResult cmpres(Policy::Ordering::compare(*(**it).monomial(), rhs));
            if( cmpres == CompareResult::GREATER ) break;
            if( cmpres == CompareResult::EQUAL )
            {
                // new coefficient would be zero, simply removing is enough.
                if((**it).coeff() == -1)
                {
                    mTerms.erase(it);
                }
                // we have to create a new term object. 
                else
                {
                    *it = std::make_shared<const Term<Coeff>>((**it).coeff()+1, (**it).monomial());
                }
                return *this;
            }
            ++it;    
        }
        // no eq ual monomial does occur. We can simply insert.
        mTerms.insert(it,std::make_shared<const Term<Coeff>>(rhs));
    }
    else 
    {
        LOG_NOTIMPLEMENTED();
        
    }
    return *this;
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff, Policy>& MultivariatePolynomial<Coeff, Policy>::operator+=(const Coeff& c)
{
    if(c == 0) return *this;
    if(Policy::Ordering::degreeOrder)
    {
        if(mTerms.size() > 0 && mTerms.front()->isConstant())
        {
            Coeff newConstantPart(mTerms.front()->coeff() + c);
            
            if(newConstantPart != 0)
            {
                mTerms.front() = (std::make_shared<Term<Coeff>>(newConstantPart));
            }
            else
            {
                mTerms.erase(mTerms.begin());
            }
        }
        else
        {
            mTerms.emplace(mTerms.begin(),std::make_shared<Term<Coeff>>(c));
        }
    }
    else
    {
        LOG_NOTIMPLEMENTED();
        
    }
    return *this;
}

template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+( const MultivariatePolynomial<C,P>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    LOG_NOTIMPLEMENTED();
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const UnivariatePolynomial<C>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    LOG_NOTIMPLEMENTED();
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const MultivariatePolynomial<C,P>& lhs, const UnivariatePolynomial<C>& rhs)
{
    LOG_NOTIMPLEMENTED();
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const UnivariatePolynomial<MultivariatePolynomial<C>>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    LOG_NOTIMPLEMENTED();
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const MultivariatePolynomial<C,P>& lhs, const UnivariatePolynomial<MultivariatePolynomial<C>>& rhs)
{
	LOG_NOTIMPLEMENTED();
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const MultivariatePolynomial<C,P>& lhs, const Term<C>& rhs)
{
    LOG_NOTIMPLEMENTED();
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const Term<C>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return rhs + lhs;
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const MultivariatePolynomial<C,P>& lhs, const Monomial& rhs)
{
    LOG_NOTIMPLEMENTED();
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const Monomial& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return rhs + lhs;
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const MultivariatePolynomial<C,P>& lhs, const C& rhs)
{
    LOG_NOTIMPLEMENTED();
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const C& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return rhs + lhs;
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(const MultivariatePolynomial<C,P>& lhs, Variable::Arg rhs)
{
    LOG_NOTIMPLEMENTED();
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator+(Variable::Arg lhs, const MultivariatePolynomial<C,P>& rhs)
{
    return rhs + lhs;
}


template<typename Coeff, typename Policy>
const MultivariatePolynomial<Coeff, Policy> MultivariatePolynomial<Coeff,Policy>::operator -() const
{
    MultivariatePolynomial<Coeff, Policy> negation;
    negation.mTerms.reserve(mTerms.size());
    for(auto term : mTerms)
    {
        negation.mTerms.push_back(std::make_shared<const Term<Coeff>>(-term));
    }
    return negation;
}


template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff, Policy>& MultivariatePolynomial<Coeff, Policy>::operator-=(const MultivariatePolynomial& rhs)
{
    if(mTerms.size() == 0) mTerms = rhs.mTerms;
    if(rhs.mTerms.size() == 0) return *this;
    
    TermsType newTerms;
    newTerms.reserve(mTerms.size() + rhs.mTerms.size());
    typename TermsType::const_iterator lhsIt(mTerms.begin());
    typename TermsType::const_iterator rhsIt(rhs.mTerms.begin());
    while(true)
    {
        CompareResult cmpres(Policy::Ordering::compare(**lhsIt, **rhsIt));
        if(cmpres == CompareResult::LESS)
        {
            newTerms.push_back(std::make_shared<const Term<Coeff>>(-(**lhsIt)));
            if(++lhsIt != mTerms.end()) break;
        }
        else if(cmpres == CompareResult::GREATER)
        {
            newTerms.push_back(std::make_shared<const Term<Coeff>>(-(**rhsIt)));
            if(++rhsIt != rhs.mTerms.end()) break;
        }
        else 
        {
            if( (**lhsIt).coeff() != (**rhsIt).coeff() )
            {
                newTerms.push_back(std::make_shared<const Term<Coeff>>( (**lhsIt).coeff() - (**rhsIt).coeff(), (**lhsIt).monomial() ));
            }
            ++lhsIt;
            ++rhsIt;
            if(lhsIt == mTerms.end() || rhsIt == rhs.mTerms.end() ) break;
        }
    }
    while(lhsIt != mTerms.end())
    {
        newTerms.push_back(std::make_shared<const Term<Coeff>>(-(**lhsIt)));
        ++lhsIt;
    }
    
    while(rhsIt != rhs.mTerms.end())
    {
        newTerms.push_back(std::make_shared<const Term<Coeff>>(-(**rhsIt)));
        ++rhsIt;
    }
    
    mTerms = std::move(newTerms);
    return *this;
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff, Policy>& MultivariatePolynomial<Coeff, Policy>::operator-=(const Term<Coeff>& rhs)
{
    if(rhs.coeff() == 0) return *this;
    if(Policy::searchLinear) 
    {
        typename TermsType::iterator it(mTerms.begin());
        while(it != mTerms.end())
        {
            // TODO consider comparing the shared pointers.
            if( (**it).isConstant() ) break;
            CompareResult cmpres(Policy::Ordering::compare((**it), rhs));
            if( cmpres == CompareResult::GREATER ) break;
            if( cmpres == CompareResult::EQUAL )
            {
                // new coefficient would be zero, simply removing is enough.
                if((**it).coeff() == rhs.coeff())
                {
                    mTerms.erase(it);
                }
                // we have to create a new term object. 
                else
                {
                    *it = std::make_shared<const Term<Coeff>>((**it).coeff()-rhs.coeff(), (**it).monomial());
                }
                return *this;
            }
            ++it;    
        }
        // no equal monomial does occur. We can simply insert.
        mTerms.insert(it,std::make_shared<const Term<Coeff>>(-rhs));
        return *this;
    }
    else 
    {
        LOG_NOTIMPLEMENTED();
    }
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff, Policy>& MultivariatePolynomial<Coeff, Policy>::operator-=(const Monomial& rhs)
{
    if(rhs.tdeg() == 0) return *this;
    if(Policy::searchLinear) 
    {
        typename TermsType::iterator it(mTerms.begin());
        while(it != mTerms.end())
        {
            if( (**it).isConstant() ) break;
            CompareResult cmpres(Policy::Ordering::compare(*(**it).monomial(), rhs));
            if( cmpres == CompareResult::GREATER ) break;
            if( cmpres == CompareResult::EQUAL )
            {
                // new coefficient would be zero, simply removing is enough.
                if((**it).coeff() == 1)
                {
                    mTerms.erase(it);
                }
                // we have to create a new term object. 
                else
                {
                    *it = std::make_shared<const Term<Coeff>>((**it).coeff()-1, (**it).monomial());
                }
                return *this;
            }
            ++it;    
        }
        // no eq ual monomial does occur. We can simply insert.
        mTerms.insert(it,std::make_shared<const Term<Coeff>>(-1, rhs));
        return *this;
    }
    else 
    {
        LOG_NOTIMPLEMENTED();
    }
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff, Policy>& MultivariatePolynomial<Coeff, Policy>::operator-=(const Variable::Arg rhs)
{
    if(Policy::searchLinear) 
    {
        typename TermsType::iterator it(mTerms.begin());
        while(it != mTerms.end())
        {
            CompareResult cmpres(Policy::Ordering::compare(*(**it).monomial(), rhs));
            if( cmpres == CompareResult::GREATER ) break;
            if( cmpres == CompareResult::EQUAL )
            {
                // new coefficient would be zero, simply removing is enough.
                if((**it).coeff() == 1)
                {
                    mTerms.erase(it);
                }
                // we have to create a new term object. 
                else
                {
                    *it = std::make_shared<const Term<Coeff>>((**it).coeff()-1, (**it).monomial());
                }
                return *this;
            }
            ++it;    
        }
        // no eq ual monomial does occur. We can simply insert.
        mTerms.insert(it,std::make_shared<const Term<Coeff>>(-1, rhs));
    }
    else 
    {
        LOG_NOTIMPLEMENTED();
        
    }
    return *this;
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff, Policy>& MultivariatePolynomial<Coeff, Policy>::operator-=(const Coeff& c)
{
    if(c == 0) return *this;
    if(Policy::Ordering::degreeOrder)
    {
        if(mTerms.size() > 0 && mTerms.front()->isConstant())
        {
            Coeff newConstantPart(mTerms.front()->coeff() - c);
            
            if(newConstantPart != 0)
            {
                mTerms.front() = (std::make_shared<Term<Coeff>>(newConstantPart));
            }
            else
            {
                mTerms.erase(mTerms.begin());
            }
        }
        else
        {
            mTerms.emplace(mTerms.begin(),std::make_shared<Term<Coeff>>(-c));
        }
    }
    else
    {
        LOG_NOTIMPLEMENTED();
        
    }
    return *this;
}

template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-( const MultivariatePolynomial<C,P>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const UnivariatePolynomial<C>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const MultivariatePolynomial<C,P>& lhs, const UnivariatePolynomial<C>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const UnivariatePolynomial<MultivariatePolynomial<C>>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const MultivariatePolynomial<C,P>& lhs, const UnivariatePolynomial<MultivariatePolynomial<C>>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const MultivariatePolynomial<C,P>& lhs, const Term<C>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const Term<C>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const MultivariatePolynomial<C,P>& lhs, const Monomial& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const Monomial& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const MultivariatePolynomial<C,P>& lhs, const C& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const C& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(const MultivariatePolynomial<C,P>& lhs, Variable::Arg rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator-(Variable::Arg lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}

template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>& MultivariatePolynomial<Coeff,Policy>::operator*=(const MultivariatePolynomial<Coeff,Policy>& rhs)
{
    TermsType newTerms;
    newTerms.reserve(mTerms.size() * rhs.mTerms.size());
    for(auto termLhs : mTerms)
    {
        for(auto termRhs : rhs.mTerms)
        {
            newTerms.push_back(std::make_shared<Term<Coeff>>(*termLhs * *termRhs));
        }
    }
    mTerms.clear();
    // Sort the entries from newterms.
    // As automatic template deduction will not work (Ordering::less is overloaded), we give an explicit function pointer cast.
    std::sort(newTerms.begin(), newTerms.end(), (bool (&)(std::shared_ptr<const Term<Coeff>> const&, std::shared_ptr<const Term<Coeff>> const&))Ordering::less);
    // remove duplicates by adding their coefficients.
    // list.unique() fails because it does not handle coefficient updates.
    std::shared_ptr<const Term<Coeff>> frontTerm = newTerms.front();
    Coeff frontCoeff(frontTerm->coeff());
    
    for(auto it = ++newTerms.begin(); it != newTerms.end(); ++it)
    {
        if(Policy::Ordering::compare(*frontTerm, **it) == CompareResult::EQUAL)
        {
            // Do not add yet, but simply add the coefficient.
            frontCoeff += (*it)->coeff();
        }
        else
        {
            if(frontCoeff == frontTerm->coeff())
            {
                mTerms.push_back(frontTerm);
            }
            else
            {
                mTerms.emplace_back(std::make_shared<const Term<Coeff>>(frontCoeff, frontTerm->monomial()));
            }
            frontTerm = *it;
            frontCoeff = (*it)->coeff();
        }
    }
    
    if(frontCoeff == frontTerm->coeff())
    {
        mTerms.push_back(frontTerm);
    }
    else
    {
        mTerms.emplace_back(std::make_shared<const Term<Coeff>>(frontCoeff, frontTerm->monomial()));
    }
    
    return *this;
    
}
template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>& MultivariatePolynomial<Coeff,Policy>::operator*=(const Term<Coeff>& rhs)
{
    TermsType newTerms;
    newTerms.reserve(mTerms.size());
    for(auto term : mTerms)
    {
        newTerms.push_back(std::make_shared<Term<Coeff>>(*term * rhs));
    }
    mTerms = std::move(newTerms);
    return *this;
}
template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>& MultivariatePolynomial<Coeff,Policy>::operator*=(const Monomial& rhs)
{
    TermsType newTerms;
    newTerms.reserve(mTerms.size());
    for(auto term : mTerms)
    {
        newTerms.push_back(std::make_shared<Term<Coeff>>(*term * rhs));
    }
    mTerms = std::move(newTerms);
    return *this;
}
template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>& MultivariatePolynomial<Coeff,Policy>::operator*=(const Variable::Arg rhs)
{
    TermsType newTerms;
    newTerms.reserve(mTerms.size());
    for(auto term : mTerms)
    {
        newTerms.push_back(std::make_shared<Term<Coeff>>(*term * rhs));
    }
    mTerms = std::move(newTerms);
    return *this;
}
template<typename Coeff, typename Policy>
MultivariatePolynomial<Coeff,Policy>& MultivariatePolynomial<Coeff,Policy>::operator*=(const Coeff& c)
{
    if(c == 1) return *this;
    if(c == 0) 
    {
        mTerms.clear();
        return *this;
    }
    TermsType newTerms;
    newTerms.reserve(mTerms.size());
    for(auto term : mTerms)
    {
        newTerms.push_back(std::make_shared<Term<Coeff>>(*term * c));
    }
    mTerms = std::move(newTerms);
    return *this;
}

template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*( const MultivariatePolynomial<C,P>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const UnivariatePolynomial<C>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const MultivariatePolynomial<C,P>& lhs, const UnivariatePolynomial<C>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const UnivariatePolynomial<MultivariatePolynomial<C>>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const MultivariatePolynomial<C,P>& lhs, const UnivariatePolynomial<MultivariatePolynomial<C>>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const MultivariatePolynomial<C,P>& lhs, const Term<C>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const Term<C>& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const MultivariatePolynomial<C,P>& lhs, const Monomial& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const Monomial& lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const MultivariatePolynomial<C,P>& lhs, const C& rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const C& lhs, const MultivariatePolynomial<C,P>& rhs)
{

}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(const MultivariatePolynomial<C,P>& lhs, Variable::Arg rhs)
{
    
}
template<typename C, typename P>
const MultivariatePolynomial<C,P> operator*(Variable::Arg lhs, const MultivariatePolynomial<C,P>& rhs)
{
    
}

template<typename C, typename P>
std::ostream& operator <<( std::ostream& os, const MultivariatePolynomial<C,P>& rhs )
{
    const typename MultivariatePolynomial<C,P>::TermsType& terms(rhs.mTerms);
    typename MultivariatePolynomial<C,P>::TermsType::const_reverse_iterator term = terms.rbegin();
    if(terms.size() == 0) return os;
    if(terms.size() == 1) return os << **term;
    os << **term;
    
    for(++term; term != terms.rend(); ++term)
    {
        os << " + " << **term;
    }
    return os;
}

template<typename Coeff, typename Policy>
void MultivariatePolynomial<Coeff, Policy>::sortTerms()
{
    std::sort(mTerms.begin(), mTerms.end(), (bool (*)(const std::shared_ptr<const Term<Coeff>>&, const std::shared_ptr<const Term<Coeff>>& ))Ordering::less);
}

}