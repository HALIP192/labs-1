`timescale 1ns / 1ps

module top(
    input   [10:0] sw_i,
    output  [7:0] disp_seg_o
    );
    
    reg [7:0]  HEX;
    
    assign  disp_seg_o = HEX;
    
    always @(*) 
    begin
        case (sw_i[9:0])
            'b00000_00001:    HEX = 'b1100_0000;
            'b00000_00010:    HEX = 'b1111_1001;
            'b00000_00100:    HEX = 'b1010_0100;
            'b00000_01000:    HEX = 'b1011_0000;
            'b00000_10000:    HEX = 'b1001_1001;
            'b00001_00000:    HEX = 'b1001_0010;
            'b00010_00000:    HEX = 'b1000_0010;
            'b00100_00000:    HEX = 'b1111_1000;
            'b01000_00000:    HEX = 'b1000_0000;
            'b10000_00000:    HEX = 'b1001_0000;
            default:
                    HEX = 'b1111_1111;         
        endcase
        HEX <= sw_i[10] ? HEX - 'b1000_0000 : HEX; 
    end
    
endmodule
