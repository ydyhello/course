module shift(fbus,flbus,frbus,a,w,cf);
    input fbus,flbus,frbus;
    input [7:0] a;
    output reg [7:0] w;
    output reg cf;
    always@(fbus or flbus or frbus or a)
    begin 
      cf = 0;
      //case({fbus,flbus,frbus})
      //3'b100:
      if (fbus == 1&& flbus == 0 && frbus == 0)
      begin
		w = a;
	  end
      //3'b010:
      else if(fbus == 0 && flbus == 1 && frbus == 0) 
      begin
      w[7:1] = a[6:0]; w[0] = a[7]; cf = a[7]; 
      end
      else if(fbus == 0 && flbus == 0 && frbus == 1)
      //begin
      //3'b001:
      begin
      w[7] = a[0]; w[6:0] = a[7:1]; cf = a[0]; 
      end
      //default: 
      else begin w = 8'hZZ;end
      //endcase
    end
endmodule