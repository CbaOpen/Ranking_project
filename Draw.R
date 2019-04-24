modelname="graph.data"
data=read.table(modelname)
attach(data);

ittpow = V1
tpow = V2
ittgs = V3
tgs = V4

nom <- c("wb-cs", "Stanford", "Stanford_Berkeley", "in-2004", "wikipedia", "wb-edu");
num <- 1:6
newdata <- data[order(V5),]

plot(1:6, ittpow, xlab="Graphes du web", ylab= "nombre d'itération", col="red",ylim=c(0,200),pch=19);
text(1:6, ittpow, labels=nom, cex=0.7, pos=1);
points(1:6, ittgs,col='blue',pch=19);
text(1:6, ittgs, labels=nom, cex=0.7, pos=1);
plot(1:6,tpow, xlab="Graphes du web", ylab= "temps d'exécution", col="red",pch=19,ylim=c(0,300));
text(1:6, tpow, labels=nom, cex=0.7, pos=3);
points(1:6,tgs,col='blue',pch=19);
text(1:6, tgs, labels=nom, cex=0.7, pos=1);
detach(data)
attach(newdata)

depth = V5 #variance du nombre d'arêtes
acc = V6
nom <- c("wb-cs", "wb-edu", "Stanford","Berkeley", "wikipedia", "in-2004");
plot(depth,acc,xlab="Variance du nombre d'arêtes par sommet",ylab= "accélération",col="red",pch=19);
text(depth, acc, labels=nom, cex=0.7, pos=1);

