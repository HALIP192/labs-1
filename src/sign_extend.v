`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08.10.2021 17:17:56
// Design Name: 
// Module Name: sign_extend
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module sign_extend(
    input [31:0] data,
    input        count,
    output [31:0] result
    );
    
    assign  result = count ? data[7] ? 'b11111111_11111111_11111111_00000000 + data[7:0] : 'd0 + data[7:0] : data;
endmodule
