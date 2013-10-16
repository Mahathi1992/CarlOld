/**
 * @file VariablePool.h 
 * @author Sebastian Junges
 */

#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <map>

#include "Variable.h"


namespace carl 
{

/**
 * Class which saves names of variables and can generate fresh variables etc.
 * 
 * We decided it to be a singleton:
 * 1) We need global access to the variable pool. Everywhere where we want to 
 * interpret some polynomial, we should have access to the pool. 
 * 2) We only want !exactly! one instance of the pool, as otherwise we would get potential clashes
 *  if variables are not available in some method.
 *  Local variable pools make no sense, as variables cannot distinguish from which pool they stem.
 * 
 * 
 */
class VariablePool
{
private:
    
    /// Counter for the number of used variables.
    unsigned mNextVarId;
    
    /**
     * The (only) instance of VariablePool.
     */
    static std::shared_ptr< VariablePool >  instance;
    /**
     * Making sure that creating a new instance is called only once.
     * Note: According to the new C++11 standard this is not strictly necessary.
     * However, this is currently only supported in gcc 4.8.1 and newer. Therefore,
     * we decided to make explicit use of this flag, thereby supporting gcc 4.7 
     * and some other compilers.
     */
    static std::once_flag                   only_one;
    
    std::map<Variable, std::string> mFriendlyNames;
    /* 
     * Constructors are all private, as we have a singleton.
     */
    VariablePool();
    VariablePool(const VariablePool& rs) = delete;
    VariablePool& operator = (const VariablePool& rs);
public:
    static VariablePool& getInstance(  );
    
    /**
     * Get a variable which was not used before.
     * @param type
     * @return A variable of type type.
     */
    Variable getFreshVariable(VariableType type = VT_REAL);
    /**
     * Get the name recorded for this variable. If no special name was set,
     * we use "x_id".
     * @param v
     * @return 
     */
    const std::string getVariableName(Variable::Arg v, bool friendlyVarName = true) const;
    /**
     * Add a name for a given variable.
     * @param v
     * @param name
     */
    void setVariableName(Variable::Arg v, const std::string& name);
   
    /**
     * The number of variables initialized by the pool.
     * @return 
     */
    unsigned nrVariables() const
    {
        return mNextVarId;
    }
    
    static unsigned NrVariables() 
    {
        return getInstance().nrVariables();
    }
    
    static std::string getName(Variable::Arg v, bool friendlyVarName = true)
    {
        if(!instance) 
        {
            return getInstance().getVariableName(v, friendlyVarName);
        }
        else
        {
            return instance->getVariableName(v, friendlyVarName);
        }
        
    }
    /**
     * Remove all information.
     */
    void clear();
};

}