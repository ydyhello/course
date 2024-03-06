module psw(clk,cf_en,zf_en,cf,zf,c,z);
input clk,cf_en,zf_en,cf,zf;
output c,z;
reg c,z;
always@(negedge clk)
begin
if(cf_en==1)
c<=cf;
else
c<=c;
if(zf_en==1)
z<=zf;
else
z=z;
end
endmodule