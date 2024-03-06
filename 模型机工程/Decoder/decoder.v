module decoder(
	input wire en,
	input wire [7:0] ir,
	output reg mova,movb,movc,add,sub,and1,not1,rsr,rsl,jmp,jz,jc,in1,out1,nop,halt);
always @(ir) begin //sensitive list   begin--end
{mova,movb,movc,add,sub,and1,not1,rsr,rsl,jmp,jz,jc,in1,out1,nop,halt}=0;//initialize
	if(en) begin
		if(ir[7:4]==4'b1100) begin//12
			if(ir[3:2]==2'b11) 	begin mova=0;movb=1;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end
			else if(ir[1:0]==2'b11) begin mova=0;movb=0;movc=1;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end
			else                    begin mova=1;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end
		end
		else if(ir[7:4]==4'b1001)   begin mova=0;movb=0;movc=0;add=1;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end
		else if(ir[7:4]==4'b0110)   begin mova=0;movb=0;movc=0;add=0;sub=1;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end
		else if(ir[7:4]==4'b1011)   begin mova=0;movb=0;movc=0;add=0;sub=0;and1=1;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end
		else if(ir[7:4]==4'b0101)   begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=1;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end
		else if(ir[7:4]==4'b1010) begin//10
		if(ir[1:0]==2'b00)          begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=1;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end
		else if(ir[1:0]==2'b11)     begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=1;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end//3
			end
		else if(ir[7:2]==6'b001100) begin //12
			if(ir[1:0]==2'b00)      begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=1;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end//0
			else if(ir[1:0]==2'b01) begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=1;jc=0;in1=0;out1=0;nop=0;halt=0;end//1
			else if(ir[1:0]==2'b10) begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=1;in1=0;out1=0;nop=0;halt=0;end//2
			end		
		else if(ir[7:4]==4'b0010)   begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=1;out1=0;nop=0;halt=0;end//2
		else if(ir[7:4]==4'b0100)   begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=1;nop=0;halt=0;end//4
		else if(ir[7:4]==4'b0111)   begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=1;halt=0;end//7
		else if(ir[7:4]==4'b1000)   begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=1;end//8
		else                        begin mova=0;movb=0;movc=0;add=0;sub=0;and1=0;not1=0;rsr=0;rsl=0;jmp=0;jz=0;jc=0;in1=0;out1=0;nop=0;halt=0;end
	end
end	
endmodule