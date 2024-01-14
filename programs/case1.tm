; the finite set of states
#Q = {0,check_a,check_b,accept,right_a,left_b,right,left,reject,illegal}

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {a,b,c,I,x,y,z,l,e,g,n,p,u,t,_}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {accept}

; the number of tapes
#N = 2

; the transition functions

; State 0: 
0 b_ _b rr reject
0 a_ _a rr check_a
0 __ __ ** illegal

check_a a_ _a rr check_a
check_a b_ _b rr check_b
check_a __ _b rr reject

check_b a_ _a rr reject
check_b b_ _b rr check_b
check_b __ __ ll left

reject a_ __ rr reject
reject b_ __ rr reject
reject __ __ r* illegal

illegal __ Il r* illegal
illegal _l lx r* illegal
illegal _x le r* illegal
illegal _e eg r* illegal
illegal _g ga r* illegal
illegal _a ay r* illegal
illegal _y lz r* illegal
illegal _z _I r* illegal
illegal _I In r* illegal
illegal _n np r* illegal
illegal _p pu r* illegal
illegal _u ut r* illegal
illegal _t tb ** illegal

left _b __ *l left_b
left _a __ ** accept
left __ __ ** accept

left_b _b _b *l left_b
left_b _a ca rl left_b
left_b __ __ *r right

right _a __ *r right_a
right _b __ ** accept
right __ __ ** accept

right_a _a _a *r right_a
right_a _b cb rr right_a
right_a __ __ *l left