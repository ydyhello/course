module sm(clk,sm_en,sm);
input clk,sm_en;
output sm;
reg sm;
always@(negedge clk)
	if(sm_en==1)
	sm<=~sm;
	else
	sm<=sm;
endmodule