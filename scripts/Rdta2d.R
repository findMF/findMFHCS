library("RSQLite")
library("DBI")

#what this file does is to concert the featues in the feature table (sqlite file) into a dta2d file.
#run it using Rscript Rdta2d.R /dir/to/sqlfile

args <- commandArgs(TRUE)
direct<-args[1]

drv <-dbDriver("SQLite")


massfile<-grep("*.mass",dir(direct),value=T)
mass<-readBin(file.path(direct,massfile),"numeric",size=8,n=100000)

rtfile <- grep("*.rt",dir(direct),value=T)
rt<-readBin(file.path(direct,rtfile),"numeric",size=8,n=100000)



dbstring <- grep("*.sqlite",dir(direct),value=T)
con <- dbConnect(drv,file.path(direct,dbstring))
res <- dbSendQuery(con, "select MZ, RT, minRTIndex, rtExtend, minMZIndex, mzExtend,mzProjection ,rtProjection from features where rtExtend>2 and mzExtend>2 and Volume>400 order by MZ")


cc = 0
while(!dbHasCompleted(res)){
  
  coord <- fetch(res,n=100000)
  
  print(dim(coord))

  #read the coordinate projections
  mzp <-vector("list",length(coord$rtProjection))
  rtp <-vector("list",length(coord$rtProjection))
  for(i in 1:length(coord$rtProjection)){
    mzp[[i]]<-readBin(coord$mzProjection[[i]],"numeric",size=4,n=1000)
    rtp[[i]]<-readBin(coord$rtProjection[[i]],"numeric",size=4,n=1000)
  }
  
  
  print("attempt to allocate")
  rs<-matrix(0,nrow=dim(coord)[1],ncol=length(rt))
  print("succeeded")
  for(i in 1:dim(coord)[1])
  {
    ext<-coord$rtExtend[i]
    minidx<-coord$minRTIndex[i]
    minidx <- minidx+1
    rs[i,minidx:(minidx+ext-1)]<-rtp[[i]]
  }
  
  resMat <- matrix(0,nrow=sum(rs>0),ncol=3)
  
  count = 1
  for(i in 1:length(rt)){
    idx <- which(rs[,i]>0)
    
    if(length(idx > 0)){
      countn = count + length(idx);
      
      tmp = cbind(rep(rt[i],length(idx)),coord$MZ[idx],rs[,i][idx])
      #print(dim(tmp))
      #print(count)
      #print(countn)
      resMat[count:(countn-1),] = tmp
      count = countn
    }
  }
  cc = cc +1 #add rownames only 1 times
  if(cc == 1){
    colnames(resMat)<-c("SEC","MZ","INT")
  }
  
  x<-sub("(.+)[.][^.]+$", "\\1",dbstring )
  x<-sub("(.+)[.][^.]+$", "\\1",x)
  x<-paste(x,"_RR.dta2d",sep="")
  
  write.table(resMat, file=x,row.names=FALSE, append=TRUE)
  print(file.path(x))
  
}

dbDisconnect(con)

