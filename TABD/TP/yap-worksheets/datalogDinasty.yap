person(1,'Prince William - Duke of Cambridge',m,'1982-06-21','NULL').
person(2,'Catherine - Duchess of Cambridge',f,'1982-01-09','NULL').
person(3,'Prince George',m,'2013-07-22','NULL').
person(4,'Prince Harry',m,'1984-09-15','NULL').
person(5,'Prince Charles - Prince of Wales',m,'1948-11-14','NULL').
person(6,'Diana - Princess of Wales',f,'1961-07-01','1997-08-31').
person(7,'Camilla - Duchess of Cornwall',f,'1947-07-17','NULL').
person(8,'Princess Anne',f,'1950-08-15','NULL').
person(9,'Mark Phillips',m,'1948-09-22','NULL').
person(10,'Timothy Laurence',m,'1955-03-01','NULL').
person(11,'Peter Phillips',m,'1977-11-15','NULL').
person(12,'Autumn Phillips',f,'1978-05-03','NULL').
person(13,'Zara Phillips',f,'1981-05-15','NULL').
person(14,'Mike Tindall',m,'1978-10-18','NULL').
person(15,'Savannah',f,'2010-12-29','NULL').
person(16,'Isla',f,'2012-03-31','NULL').
person(17,'Mia',f,'2014-01-17','NULL').
marriage(1,1,2,'2011-04-29','NULL').
marriage(2,5,7,'2005-04-09','NULL').
marriage(3,5,6,'1981-07-01','1996-08-28').
marriage(4,8,9,'1973-11-14','1992-04-28').
marriage(5,8,10,'1992-12-12','NULL').
marriage(6,11,12,'2008-05-17','NULL').
marriage(7,13,14,'2011-07-30','NULL').
parent(1,5,1).
parent(2,5,4).
parent(3,6,1).
parent(4,6,4).
parent(5,1,3).
parent(6,2,3).
parent(7,13,17).
parent(8,14,17).
parent(9,11,15).
parent(10,11,16).
parent(11,12,15).
parent(12,12,16).
parent(13,8,11).
parent(14,8,13).
parent(15,9,11).
parent(16,9,13).

female(I,N) :- person(I,N,f,_,_).

mother(NM, NF) :- female(I,NM) , parent(_, I ,IF) , person(IF, NF,_ ,_ ,_).

grandSon(X,Y) := parent(_,X,I) , parent(_,I,Y) .

sibling (IP,IS) :- parent (_, I, IP), parent (_, I, IS), IP \== IS

uncle(X,Y) :- parent(_,X,I) , sibling(X,Y) .

father (X, Y) :- parent (_, X, I) , (X, _, m, _, _) .

mother (X, Y) :- parent (_, X, I) , (X, _, f, _, _) .

divorced (X,Y):- marriage (_,X,Y,_,D) , D \== 'NULL' .

widower (X) :- marriage(Y, Z , H) , person(Z, _,m,_,_), person (Y, _,_,_,D) , D \== 'NULL'. 





