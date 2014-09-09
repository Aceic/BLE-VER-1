class tx_sequence extends ovm_sequence;

   `ovm_object_utils(tx_sequence)


   extern function new(string name = "tx_sequence");
   extern task body;

endclass

function tx_sequence::new(string name = "tx_sequence");
   super.new(name);
endfunction


task tx_sequence::body;
   
   tx_seq_item req;

   begin

      req = tx_seq_item::type_id::create("req");
      start_item(req);
      assert(req.randomize());
      finish_item(req);

   end

endtask
