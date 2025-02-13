
module simple(input wire m,input wire [3:0] s,input wire [7:0] a,b,output reg [7:0] t,output reg cf,zf);
  always @(m or s or a or b) begin
      {cf,zf}=0;
      t=00000000;
      if(m==1'b0) 
		begin
		if (s==4'b1010) t=b;
		else if (s==4'b1100||s==4'b0100) t=a;
		end
      else if(m==1'b1) 
		begin
          if(s[3:0]==4'b1001) 
		    begin
             {cf,t}=a+b;
             if(t==8'b00000000) zf=1;
             else zf=0; 
		    end
          else if(s[3:0]==4'b0110) 
            begin
             {cf,t}=b-a;
             if(t==8'b00000000) zf=1;
             else zf=0;         
            end
          else if(s[3:0]==4'b1011) t=a&b;
          else if(s[3:0]==4'b0101) t=~b;
      end
end
endmodule
/*
module simple(
		input m,
		input [3:0] s,
		input [7:0] a,b,
		output reg cf,zf,
		output reg [7:0] t);
reg [8:0] temp;
always @(m or s or a or b) begin
	zf=0;cf=0;
	if(m==1'b0 && s[3:0]==4'b1010) t=b;
	else if(m==1'b0 && (s[3:0]==4'b1100 || s[3:0]==4'b0100)) t=a;
	else if(m==1'b1) begin
        if(s[3:0]==4'b1001) begin
			t=a+b;
			if(t==8'b00000000) zf=1;
			else zf=0;
			temp=a+b;
			if(temp[8]==1'b0) cf=0;
			else cf=1;
		end
		else if(s[3:0]==4'b0110) begin
			t=b-a;
			if(t==8'b00000000) zf=1;
			else zf=0;
			temp=b-a;
			if(temp[8]==1'b0) cf=0;
			else cf=1;
		end
		else if(s[3:0]==4'b1011) t=a&b;
		else if(s[3:0]==4'b0101) t=~b;
		
	end
end
endmodule 
*/

