modelname="graph.data"
data=read.table(modelname)
attach(data);

nbV=V1
nbE = V2
nbN = V3
ittpow = V4
tpow = V5
ittgs = V6
tgs = V7

newdata <- data[order(V8),]

plot(nbV,ittpow,xlab="nombre de sommet",ylab= "nombre d'itération",col="red",ylim=c(0,200),pch=19);
points(nbV,ittgs,col='blue',pch=19);
plot(nbV,tpow,xlab="nombre de sommet",ylab= "temps d'exécution",col="red",pch=19,ylim=c(0,300));
points(nbV,tgs,col='blue',pch=19);
detach(data)
attach(newdata)

depth = V8 #nbN/nbV
acc = V9
plot(depth,acc,xlab="nbN/nbV",ylab= "acc",col="red",type="l");
# plot(nbV,tpow,xlab="nombre de sommet",ylab= "temps",col="red");
# lines(nbV,tgs,col='blue');
