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

nom <- c("wb-cs", "Stanford", "in-2004", "wikipedia", "wb-edu");
num <- 1:5
newdata <- data[order(V8),]

#plot(nbV,ittpow,xlab="nombre de sommet",ylab= "nombre d'itération",col="red",ylim=c(0,200),pch=19);
#points(nbV,ittgs,col='blue',pch=19);
plot(1:5, ittpow, xlab="Graphes du web", ylab= "nombre d'itération", col="red",ylim=c(0,200),pch=19);
text(1:5, ittpow, labels=nom, cex=0.7, pos=1);
points(1:5, ittgs,col='blue',pch=19);
text(1:5, ittgs, labels=nom, cex=0.7, pos=1);
plot(1:5,tpow, xlab="Graphes du web", ylab= "temps d'exécution", col="red",pch=19,ylim=c(0,300));
text(1:5, tpow, labels=nom, cex=0.7, pos=3);
points(1:5,tgs,col='blue',pch=19);
text(1:5, tgs, labels=nom, cex=0.7, pos=1);
detach(data)
attach(newdata)

depth = V8 #nbN/nbV
acc = V9
nom <- c("wb-cs", "wb-edu", "Stanford", "wikipedia", "in-2004");
plot(depth,acc,xlab="Variance du nombre arêtes par sommet",ylab= "accélération",col="red",pch=19);
text(depth, acc, labels=nom, cex=0.7, pos=1);
# plot(nbV,tpow,xlab="nombre de sommet",ylab= "temps",col="red");
# lines(nbV,tgs,col='blue');
