
module reg_group(clk,we,raa,rwba,i,s,d);
input clk,we,raa,rwba,i;
output s,d;
wire [7:0]i;
wire [1:0]raa;//01
wire [1:0]rwba;//11
reg [7:0]r1,r2,r3;
reg [7:0]s;
reg [7:0]d;

initial
begin
r1=8'b00000011;
r2=8'b00111100;
r3=8'b10000000;
end

always @(we,raa,rwba,i)//01 11
begin 
	if(raa==2'b00) s=r1;
	else if(raa==2'b01) s=r2;//
	else s=r3;
	
	if(rwba==2'b00) d=r1;
	else if(rwba==2'b01) d=r2;
	else d=r3;//
end

always @(negedge clk)
	if(we==0)begin
		if(rwba==2'b00)  r1<=i;
		else if(rwba==2'b01)  r2<=i;
		else       r3<=i;
	end
	else begin 
	r1<=r1;r2<=r2;r3<=r3;
	end

endmodule

