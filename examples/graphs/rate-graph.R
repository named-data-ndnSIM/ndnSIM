# Copyright (c) 2012  Alexander Afanasyev <alexander.afanasyev@ucla.edu>

# install.packages ('ggplot2')  
library (ggplot2)
# install.packages ('scales')  
library (scales)

# install.packages ('doBy')
library (doBy)

#########################
# Rate trace processing #
#########################
data = read.table ("rate-trace.txt", header=T)
data$Node = factor (data$Node)
data$FaceId <- factor(data$FaceId)
data$Kilobits <- data$Kilobytes * 8
data$Type = factor (data$Type)

# exlude irrelevant types
data = subset (data, Type %in% c("InInterests", "OutInterests", "InData", "OutData"))

# combine stats from all faces
data.combined = summaryBy (. ~ Time + Node + Type, data=data, FUN=sum)
  
data.root = subset (data.combined, Node == "root")
data.leaves = subset (data.combined, Node %in% c("leaf-1", "leaf-2", "leaf-3", "leaf-4"))

# graph rates on all nodes in Kilobits
g.all <- ggplot (data.combined) +
  geom_point (aes (x=Time, y=Kilobits.sum, color=Type), size=1) +
  ylab ("Rate [Kbits/s]") +
  facet_wrap (~ Node)

print (g.all)
  
# graph rates on the root nodes in Packets
g.root <- ggplot (data.root) +
  geom_point (aes (x=Time, y=Kilobits.sum, color=Type), size=2) +
  geom_line (aes (x=Time, y=Kilobits.sum, color=Type), size=0.5) +
  ylab ("Rate [Kbits/s]")

print (g.root)

png ("root-rates.png", width=500, height=250)
print (g.root)
dev.off ()

###############################
# Aggreagate trace processing #
###############################

data = read.table ("aggregate-trace.txt", header=T)
data$Node = factor (data$Node)
data$FaceId <- factor(data$FaceId)
data$Type = factor (data$Type)

# exlude irrelevant types
data = subset (data, Type %in% c("InInterests", "OutInterests", "InData", "OutData"))

# Aggregate packet stats in 5-second intervals
data$Time5Sec = 5 * ceiling (data$Time / 5)
data.combined = summaryBy (. ~ Time5Sec + Node + Type, data=data, FUN=sum)

data.root = subset (data.combined, Node == "root")
data.leaves = subset (data.combined, Node %in% c("leaf-1", "leaf-2", "leaf-3", "leaf-4"))

# graph rates on all nodes in Packets
g.all <- ggplot (data.combined) +
  geom_point (aes (x=Time5Sec, y=Packets.sum, color=Type), size=2) +
  geom_line (aes (x=Time5Sec, y=Packets.sum, color=Type), size=0.5) +
  ylab ("Number of transitted packets in 5 secon intervals") +
  facet_wrap (~ Node)

print (g.all)
  
# graph rates on the root nodes in Packets
g.root <- ggplot (data.root) +
  geom_point (aes (x=Time5Sec, y=Packets.sum, color=Type), size=2) +
  geom_line (aes (x=Time5Sec, y=Packets.sum, color=Type), size=0.5) +
  ylab ("Number of transitted packets in 5 secon intervals") +
  ylim (c(0,2000))

print (g.root)

png ("root-5sec-counts.png", width=500, height=250)
print (g.root)
dev.off ()
