/*
module con_signal (mova,movb,movc,add,sub,andl,notl,rsr,rsl,jmp,jz,z,jc,c,inl,outl,nop,halt,ir,sm,
pc_ld,pc_inc,
madd,
ram_xl,ram_dl,
ir_ld,
reg_ra,reg_wa,reg_we,
alu_m,alu_s,
shi_fbus,shi_flbus,shi_frbus,
cf_en,zf_en,sm_en,in_en,out_en);

input mova,movb,movc,add,sub,andl,notl,rsr,rsl,jmp,jz,z,jc,c,inl,outl,nop,halt,sm;
input [7:0] ir;
output reg [1:0] reg_ra,reg_wa,madd;
output reg [3:0] alu_s;
output reg pc_ld,pc_inc,
reg_we,ram_xl,ram_dl,alu_m,
shi_fbus,shi_flbus,shi_frbus,
ir_ld,cf_en,zf_en,sm_en,in_en,out_en;
always@(mova,movb,movc,add,sub,andl,notl,rsr,rsl,jmp,jz,jc,inl,outl,nop,halt,ir,sm)
	begin 
	reg_ra=ir[1:0];
	reg_wa=ir[3:2];
	alu_s=ir[7:4];
	pc_ld=jmp|(jc&c)|(jz&z);
	pc_inc=(~sm)|(jc&(~c))|(jz&(~z));
	reg_we=~(sm&(mova|movc|add|sub|andl|notl|rsr|rsl|inl));
	ram_xl=movb;
	ram_dl=(~sm)|movc|jmp|(jz&z)|(jc&c);
	alu_m=andl|notl|add|sub|rsr|rsl|outl;
	shi_fbus=mova|movb|add|sub|andl|notl|outl;
	shi_flbus=rsl;
	shi_frbus=rsr;
	ir_ld=~sm;
	cf_en=add|sub|rsr|rsl;
	zf_en=add|sub;
	sm_en=~halt;
	in_en=inl;
	out_en=outl;
	if(sm==1&&movc==1) madd=2'b01;
	else if(sm==1&&movb==1) madd=2'b10;
	else madd=2'b00;
	end
endmodule
*/
module con_signal(mova,movb,movc,add,sub,and1,not1,rsr,rsl,jmp,jz,jc,in1,out1,nop,halt,ir,sm,z,c,
reg_ra,reg_wa,
madd,alu_s,
pc_ld,pc_inc,
alu_m,ir_ld,
cf_en,zf_en,sm_en,
reg_we,ram_xl,ram_dl,
shi_fbus,shi_flbus,shi_frbus,
in_en,out_en);
input mova,movb,movc,add,sub,and1,not1,rsr,rsl,jmp,jz,z,jc,c,in1,out1,nop,halt,sm;
input [7:0] ir;
output reg[1:0] reg_ra,reg_wa,madd;
output reg[3:0] alu_s;
output reg pc_ld,pc_inc,reg_we,ram_xl,ram_dl,
alu_m,shi_fbus,shi_flbus,shi_frbus,
ir_ld,cf_en,zf_en,sm_en,in_en,out_en;
 always @(*) begin
    reg_ra=ir[1:0];
    reg_wa=ir[3:2];
    alu_s=ir[7:4];
    pc_ld=jmp|(jc&c)|(jz&z);
    pc_inc=(~sm)|(jc&(~c))|(jz&(~z));
    reg_we=~(sm&(mova|movc|add|sub|and1|not1|rsr|rsl|in1));
    ram_xl=movb;
	ram_dl=(~sm)|movc|jmp|(jz&z)|(jc&c);
	alu_m=and1|not1|add|sub;
	shi_fbus=mova|movb|add|sub|and1|not1|out1;
	shi_flbus=rsl;
	shi_frbus=rsr;
    ir_ld=~sm;
    cf_en=add|sub|rsr|rsl;
    zf_en=add|sub;
    sm_en=~halt;
    in_en=in1;
    out_en=out1;
    if(sm==1&&movc==1) madd=2'b01;
    else if(sm==1&&movb==1) madd=2'b10;
    else madd=2'b00;
    end
endmodule

