

class intr_driver extends ovm_driver #(intr_seq_item);

  `ovm_component_utils(intr_driver)                                   // Factory registration of interrupt driver

  intr_config int_cfg;                                                 // Instance of environment configuration class 

  virtual intr_if vif_intr;                                           // Virtual instance of Interrupt interface

  extern function new(string name = "intr_driver", ovm_component parent = null);
  extern function void build();
  extern function void connect();
  extern task run();
  
  extern task drive(intr_seq_item trans);

endclass: intr_driver


  function intr_driver::new(string name = "intr_driver", ovm_component parent = null);
   super.new(name, parent);
  endfunction 


  function void intr_driver:: build();
   ovm_object temp1;

   `ovm_info(get_name(),"BUILD PHASE",OVM_LOW)

   if(!get_config_object("int_cfg",temp1,0))                       // Get the pointer of env configuration object            
    `ovm_fatal("INTR_MONITOR","Cannot get config handle..")
 
   if(!$cast(int_cfg,temp1))
    `ovm_fatal("INTR_MONITOR","Casting failed")

  endfunction: build

  function void intr_driver:: connect();
  
  `ovm_info(get_name(),"CONNECT PHASE",OVM_LOW)
   vif_intr = int_cfg.int_if;
  
  endfunction: connect

  task intr_driver:: run();

   `ovm_info(get_name(),"RUN PHASE",OVM_LOW)
 
   forever
    begin
     seq_item_port.get_next_item(req);
     drive(req);                                                               // Task to drive the interrupt sequence item
      $display ("Before item_done");
     seq_item_port.item_done();
      $display ("After item_done");
    end

  endtask

  task intr_driver::drive(intr_seq_item trans);

  `ovm_info("INTR_DRIVER",$sformatf("\n Interrupt sequence item %s",trans.sprint),OVM_LOW)
  
   vif_intr.int_o <= trans.int_o;                                            // Driving interrupt signal int_o on interrupt interface,
     
  endtask
