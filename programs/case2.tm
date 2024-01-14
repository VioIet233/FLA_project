; the finite set of states
#Q = {0,s1,s2,accept,reject,judge,true,false}

; the finite set of input symbols
#S = {a,b,c}

; the complete set of tape symbols
#G = {a,b,c,_,t,r,u,e,f,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {accept}

; the number of tapes
#N = 3

; the transition functions

; State 0: 
0 a__ a__ *** s1
0 b__ b__ *** s1
0 c__ ___ r** s2

s1 a__ _a_ rr* s1
s1 b__ _b_ rr* s1
s1 c__ ___ r** s2
s1 ___ _f_ *** false

s2 a__ __a r*r s2
s2 b__ __b r*r s2
s2 c__ ___ r** reject
s2 ___ ___ *ll judge;

judge _aa ___ *ll judge
judge _bb ___ *ll judge
judge ___ _t_ *** true
judge _ab _f_ *** false
judge _ba _f_ *** false
judge __a _f_ *** false
judge _a_ _f_ *** false
judge __b _f_ *** false
judge _b_ _f_ *** false

reject *__ ___ r** reject
reject ___ _f_ *** false

false _f_ fa_ r** false
false _a_ al_ r** false
false _l_ ls_ r** false
false _s_ se_ r** false
false _e_ e_a *** false

true _t_ tr_ r** true
true _r_ ru_ r** true
true _u_ ue_ r** true
true _e_ et_ *** true