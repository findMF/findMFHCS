setwd("/home/witold/prog/qttest/mandelbrot/findmfout")
f<-dir(pattern="*.bin")

x<-6*0
ormz<-readBin(f[2],what=numeric(),n=100000)

L<-length(ormz)
bb<-diff(ormz)
bb2<-bb
bb2[bb>0.01]<-0
plot(ormz[-L],bb2^2,pch=".",xlim=c(0,500))

orint<-readBin(f[1],what=numeric(),n=100000)
parea<-readBin(f[3+x],what=numeric(),n=100000)
pint<-readBin(f[4+x],what=numeric(),n=100000)
pmass<-readBin(f[5+x],what=numeric(),n=100000)
rint<-readBin(f[6+x],what=numeric(),n=100000)
rmass<-readBin(f[7+x],what=numeric(),n=100000)
sint <-readBin(f[8+x],what=numeric(),n=100000)

plot(rmass,rint,type="l",xlim=c(390.5,391.1),ylim=c(0,200))
lines(rmass,rint,type="h")
lines(rmass,sint,type="l",col=2,lwd=3)
lines(pmass,pint,type="h",col=4,lwd=3)
lines(pmass,parea,type="h",col=5,lwd=2,lty=2)
lines(ormz,orint,type="l",col=3,lwd=3)
lines(ormz,orint,type="h",col=3,lwd=4)

ormz[1:100]
orint[1:100]

ormz<-readBin(f[2],what=numeric(),n=100000)
L<-length(ormz)
bb<-diff(ormz)
bb2<-bb
bb2[bb>0.01]<-0
length(bb2-1)
r<-bb2[-length(bb2)]/bb2[-1]
sum(bb2==0)
r[1:10]
plot(ormz[-L],bb2^2,pch=".",xlim=c(0,800),ylim=c(0,2e-05),cex=4)
plot(ormz[-c(L-1,L)],r,pch=".",xlim=c(0,500),ylim=c(0,0.5))

abline(c(0,1))