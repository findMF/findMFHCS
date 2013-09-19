file <-("/home/witold/testdata/ludovic/lgillet_L121127_001_p_msconvert.a.c.mzXML")
aa <- openMSfile(file)
runInfo(aa)
pl <- peaks(aa,10)
dim(pl)

fileprofile <-("/home/witold/testdata/ludovic/lgillet_L121127_001_p_msconvert.mzXML")
ap <- openMSfile(fileprofile)
pl1 <- peaks(ap,10)
dim(pl1)
head(pl1)

plot(pl1[,1],pl1[,2],type="l",xlim=c(395,400),ylim=c(0,2000))
lines(pl[,1],pl[,2],type="h",col=2)

