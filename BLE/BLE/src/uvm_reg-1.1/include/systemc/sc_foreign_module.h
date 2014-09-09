/*****************************************************************************

  sc_foreign_module.h -- Systemc wrapper for all HDL modules.

 *****************************************************************************/

/* $Id: //dvt/mti/rel/10.0b/src/systemc/sc_foreign_module.h#1 $ */

#ifndef SC_FOREIGN_MODULE_H
#define SC_FOREIGN_MODULE_H

#include "sc_module.h"

namespace sc_core {

class sc_foreign_module : public sc_module
{
public:

	// constructors
	sc_foreign_module(
		const sc_module_name& nm,
		const char* hdl_name,
        int num_generics = 0,
        const char** generics_list = NULL);

	sc_foreign_module(
		const sc_module_name& nm,
		int num_generics = 0,
		const char** generics_list = NULL);

	// destructor
	virtual ~sc_foreign_module() { } 

	void add_parameter(const char* param_str);

	void add_parameter(const char* param_name, int param_val);

	void elaborate_foreign_module(
		const char* hdl_name,
        int num_generics = 0,
        const char** generics_list = NULL);

private:

	void clear_parameter_list();

	::std::vector <char*> m_parameter_list;
};

} // namespace sc_core

#endif
