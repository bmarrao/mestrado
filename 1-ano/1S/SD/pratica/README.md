# Compilar
Executar : 

javac ds/assign/poisson/*.java

javac ds/assign/ring/*.java

javac ds/assign/entropy/*.java

javac ds/assign/chat/*.java





# Chat 

java ds.assign.chat.Peer Ip1 port1 Ip2 port2 ... IpN portN (j - 1)

Em que j é o peer que está rodar no terminal 

Exemplo : 


java ds.assign.chat.Peer localhost 3000 localhost 3001 localhost 30002 localhost 30003 localhost 3004 localhost 3005 0

java ds.assign.chat.Peer localhost 3000 localhost 3001 localhost 30002 localhost 30003 localhost 3004 localhost 3005 1

java ds.assign.chat.Peer localhost 3000 localhost 3001 localhost 30002 localhost 30003 localhost 3004 localhost 3005 2 

java ds.assign.chat.Peer localhost 3000 localhost 3001 localhost 30002 localhost 30003 localhost 3004 localhost 3005 3

java ds.assign.chat.Peer localhost 3000 localhost 3001 localhost 30002 localhost 30003 localhost 3004 localhost 3005 4

java ds.assign.chat.Peer localhost 3000 localhost 3001 localhost 30002 localhost 30003 localhost 3004 localhost 3005 5

# Entropy : 

java ds.assign.entropy.Peer port1 Ip2 port2 ... IpN portN no qual Ip1 e port1 n

No qual port1 é o peer que vai rodar no terminal e os outros são os Peers conectados

Exemplo : 

java ds.assign.entropy.Peer 3001 localhost 3002 

java ds.assign.entropy.Peer 3002 localhost 3001 localhost 3003 localhost 3004  

java ds.assign.entropy.Peer 3003 localhost 3002 

java ds.assign.entropy.Peer 3004 localhost 3002 localhost 3005 localhost 3006

java ds.assign.entropy.Peer 3005 localhost 3004 

java ds.assign.entropy.Peer 3006 localhost 3004 

# Ring : 


Initiate the server :

java ds.assign.ring.Server ip port  


Initiate peers :

java ds.assign.ring.Peer yourPort nextIP nextPort serverIP serverPort 

E para iniciar o token
java ds.assign.ring.Peer yourPort nextIP nextPort serverIP serverPort 1


Exemplo : 

java ds.assign.ring.Server localhost 3006 

java ds.assign.ring.Peer 3000 localhost 3001 localhost 3006  

java ds.assign.ring.Peer 3001 localhost 3002 localhost 3006 

java ds.assign.ring.Peer 3002 localhost 3003 localhost 3006  

java ds.assign.ring.Peer 3003 localhost 3004 localhost 3006  

java ds.assign.ring.Peer 3004 localhost 3005 localhost 3006  

java ds.assign.ring.Peer 3005 localhost 3000 localhost 3006 1





















