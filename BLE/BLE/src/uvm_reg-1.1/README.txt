-----------------------------------------------------------------------------

           UVM Register Package 1.1 for OVM - General Overview

-----------------------------------------------------------------------------


   This package, hereafter called the UVM Reg package, is a near verbatim
   copy of the register layer portion of the Accellera Universal Verification
   Methodology (UVM) reference library. Minor additions and modifications
   were made to enable its use as a standalone package and be compatible
   with OVM 2.1.2. 
   
   The purpose of providing this standalone UVM register package is to provide
   a standard, robust, and vendor-independent register solution to OVM users
   who are not yet able to migrate to the UVM. Then, when circumstances finally
   allow migration to the UVM, the process would not entail architectural
   rewrites or use model changes. Your investment in a register library
   would be preserved and forward compatible with any new improvements to
   the UVM library as a whole.

   The UVM, and this derivative work, are licensed under the Apache 2.0 license.
   The full text of the license is provided in this kit in the file LICENSE.txt.


   What's Different?
   -----------------
  
   Minor modifications were made to allow extraction of the UVM register code
   for use as a standalone package. A complete 'diff' of the UVM's 'reg'
   directory versus the 'src' directory in UVM Reg Pkg is provided in 

      docs/diff_uvm1.1.txt

   Other portions of the UVM base library are included in the UVM Reg package
   to allow its use with OVM. They 
   
      uvm_callbacks        : for optional uvm_reg and uvm_field callback
                             registration. The UVM Reg package requires UVM,
                             not OVM, callbacks.

      DPI shared library   : for optional HDL backdoor access. If you do not use
                             backdoor access, you can disable it by adding
                             +define+UVM_NO_DPI when compiling the UVM Reg package.
                             Precompiled libraries for linux are provided in
                             the lib directory.

      tlm2_generic_payload : to enable uvm_reg_sequence to compile


   Simulator Requirements
   ----------------------

   The UVM Reg package has the same simulator version requirements as the
   UVM reference implementation from Accellera. Consult your vendor for
   simulator-specific requirements. For Mentor's Questa, the minimum
   version needed is 6.6d.


   Documentation
   -------------

   To reduce package size and reduce maintenance issues, documentation for the
   UVM Reg Pkg is not included. Documentation can be found in the relevant
   sections in UVM Standard document and User Guide, both available for
   free download at

      http://www.accellera.org/activities/vip

   Keep in mind the text and code samples in the documentation may refer to 
   UVM base classes and features other than those ported register classes
   (e.g. uvm_object). In most cases, these references can be replaced with
   their OVM equivalent, i.e. change the u's to o's. In any case, these
   references should not affect your understanding register model usage
   in the OVM.


   Examples
   --------

   Examples are found in the <install dir>/examples. These are the same examples
   distributed with the UVM standard, converted to run on OVM per the conversion
   script described above.

   To run an example, 'cd' to one of the examples dirs and invoke the Makefile
   there:

      cd <install dir>/examples/primer
      make -f Makefile.questa OVM_HOME=<path to OVM> all

   To run all examples, 'cd' to the top-level examples dir and invoke make as follows:

      cd <install_dir>/examples
      make OVM_HOME=<path> all_examples
   
   By default, the makefile is set up to run on a linux 32-bit platform. Set the
   PLATFORM argument to linux_x86_64 or win32 to run on those platforms. (This
   option governs which precompiled DPI library is loaded. DPI libs are platform-
   dependent.)
   
      make OVM_HOME=<path> PLATFORM=linux_x86_64 all_examples

   If you copy the examples out of the install tree, you will need to also provide
   the path to the UVM Reg Package on the Makefile command line:

     cd <example dir>
     make -f Makefile.questa UVM_REG_HOME=<path> OVM_HOME=<path> all

   Instead of supplying arguments to the Makefile command line, you may
   alternatively set an environment variable of the same name.
   
   After invoking, the Makefile output will show you the commands being executed and
   their arguments. You may choose to extract these commands into a shell script
   to simplify the run procedure.

   To get more information on all the options available in the Makefile, type

      make -f Makefile.questa help


   Usage
   -----
   
   The UVM Reg package is separate from OVM. Thus, code using the register package
   will need to import the uvm_reg_pkg and, likely, include the uvm_reg_macros.svh
   files. This typically happens at the same locations you include the OVM macros
   and import the OVM package:

      `include "ovm_macros.svh"
      `include "uvm_reg_macros.svh"

      package my_reg_model;
        import ovm_pkg::*;
        import uvm_reg_pkg::*;
        ...
      endpackage

   The `include of macros should be located outside package, module, interface,
   and program block scopes. Macros exist at compilation unit scope. Including
   them inside any other scope incorrectly implies the macro definitions'
   visibility is confined to that scope.

   Generating the Register Model:
   
   UVM register models are typically generated with UVM code generators, which
   are not supplied with the UVM or this kit. (Mentor's register generator
   is available in the Certe Register Assitant.)

   A generated register model should compile without modification except for
   the `include and import statements described above.

   If after adding the necessary include and import statements the generator
   output still does not compile, you need to convert/adapt the offending code to use
   an OVM equivalent. In general, this involves simple U's to O's conversion.
   The uvm2ovm.pl Perl script included in the bin directory helps make most
   conversions for you. Type uvm2ovm.pl -h to get help and usage information.

   Integrating the Register Model:

   The UVM Reg model employs limited use of a UVM feature called the resource
   database to set and retrieve integral attributes (for indicating coverage
   and built-in test support). The uvm_reg_pkg provides a uvm_resource_db
   class definition that implements uvm_resource_db #(T)::set() and
   read_by_name() to use the OVM equivalent, set_ and get_config_int.

   The following examples' register model definitions use the uvm_resource_db
   class to set attributes that exclude certain registers from involvement
   in the built-in register test sequences.

     .../examples/sequence_api/reg_B.sv
     .../examples/models/user-defined/regmodel.sv
     .../examples/vertical_reuse/reg_B.sv
     .../examples/models/broadcast/regmodel.sv

   If your testbench does not run the built-in tests, you may get OVM warnings
   about the attributes never being referenced:
   
     # OVM_WARNING @ 520: reporter [CFGNTS] No get_config_int() call ever
        matched the following set_config_int() call from the GLOBAL scope:
          Integral: component=*, field=REG::regmodel.user_acp.NO_REG_ACCESS_TEST, value=1

   You can safely ignore these.


   Support
   -------

   Community-based support and resources for OVM and UVM
   can be found at

     http://www.ovmworld.org
     http://www.uvmworld.org
     
   A deep library of cookbook examples and methodology best practices
   can be found at

     www.verificationacademy.com
   
  
   Accellera uses the Mantis database for bug and enhancement tracking of
   the UVM reference implementation.

   It can be accessed on the web at
   
     http://www.eda.org/svdb
     
   From there, you would select the VIP project from the drop-down menu on
   the top right of the page. Then, you may choose the Register category to
   to filter out all but the register-related issues.



