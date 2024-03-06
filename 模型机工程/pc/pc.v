module pc(clk,pc_ld,pc_inc,a,add);
input clk,pc_ld,pc_inc,a;
output add;
wire [7:0]a;
reg [7:0]add;
always @(negedge clk)
begin
if(pc_inc==1 && pc_ld==0)
add<=add+1;
else if(pc_ld==1 && pc_inc==0)
add<=a;
else add<=add;
end
endmodule
