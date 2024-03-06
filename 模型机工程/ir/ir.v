module ir(clk,ir_ld,d,ir);
input [7:0]d;
input clk,ir_ld;
output [7:0]ir;
reg [7:0]ir;
always@(negedge clk)
	if(ir_ld==1)
	ir<=d;
	else
	ir<=ir;
endmodule