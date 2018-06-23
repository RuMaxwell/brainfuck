multiply: Input 2 digits; output the result (in one digit)

Input 2 characters; subtract them with 48 (the ASCII of '0')
,>,>++++++++[<------<------>>-]<

The core algorithm of multiplication
	We use two cells next to the two operands to store temporary values;
	Take 3 * 2 as an example; We use $x to represent the xth cell in memory;
	At first; there should be: $0: 3; $1: 2; $2: 0; $3: 0;
		$0: The first operand;
		$1: The second operand;
		$2: The result;
		$3: Stores the copy of $0;
	(1) The program copies the first operand (namely $0) to $2 and $3; and then move $3 back to $0;
		Notice that in Brainfuck the only way to copy a value is by moving it to more than one cell;
		This keeps the first operand for further use;
	(2) Loop (1); Everytime it hits the end of (1); decrease the second operand (namely $1) by 1;
		When $1 is decreased to 0; the result shall be there in $2;
	You can enter debug mode to see the exact thing that happens on multiplication;
[<[>>+>+<<<-]>>>[<<<+>>>-]<<-]

Output the result by adding the character with 48
<[-]>++++++++[<++++++>-]>+[<<+>>-]<<-.
Output a linefeed
[-]++++++++++.
