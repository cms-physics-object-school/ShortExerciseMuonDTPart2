# DT Short Exercise at CMS POS 2019 (Part 2 - Matching segments to muons and then accessing segment timing)

This repository contains the skeleton code for Part 2 of the DT Short Exercise at CMS POS 2019 : Matching segments to muons and then accessing segment timing.

## Introduction

The instructions for the exercise can be found at :
[https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCMSPhysicsObjectSchoolAACHEN2019DT](https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCMSPhysicsObjectSchoolAACHEN2019DT)

## Instructions

### 1. Create the Working Area

First you must create and set up your working area according to the instructions listed at the [exercise twiki](https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCMSPhysicsObjectSchoolAACHEN2019DT#Creating_the_Working_Area), skip this part in case you have already set up your working area. The instructions to set up your working area are repeated below.

In case you need to log in to the CMS POS machines at Aachen, otherwise skip the following 2 lines.
```
ssh -XY YOURLOGINNAME@portal.physik.rwth-aachen.de
ssh portal-cmspos
```

Now source CMSSW, setup your working area and create a GRID proxy.

```
source /cvmfs/cms.cern.ch/cmsset_default.sh
mkdir YOURWORKINGAREA_DT
cd YOURWORKINGAREA_DT
export SCRAM_ARCH=slc7_amd64_gcc630
cmsrel CMSSW_9_4_12
cd CMSSW_9_4_12/src
cmsenv
voms-proxy-init --voms cms
```
And then enter your GRID password to craete the proxy.

### 2. Clone this repository into your working area

Clone this repo in your working area in the CMSSWxxx/src area into a folder named 'UserCode'

```
git clone https://github.com/cms-physics-object-school/ShortExerciseMuonDTPart2.git UserCode
```

### 3. Complete the code 

Now you can complete the code based on the skeleton code in this repository.

The main C++ code is in the file 

```
CMSPOS/plugins/MatchedSegmentTiming.cc
```
and the corresponding header file

```
CMSPOS/plugins/MatchedSegmentTiming.h
```

The python config file can be found at

```
CMSPOS/test/run_matched.py
```

You can fill in the code appropriately.

**Note (if you are completely stuck)** : A fully working version of the code can be found in the branch named [*fullexercise*](https://github.com/cms-physics-object-school/ShortExerciseMuonDTPart2/tree/fullexercise) of this repo. 


### 4. Compile your code

Compile the code from the directory CMSSWxxx/src using the command :

```
scram b
```

To compile quicker using 4 cores, you can instead use the command : 

```
scram b -j 4
```

### 5. Run the CMSSW code

Use cmsRun to run the code from the directory CMSSWxxx/src using the command :

```
cmsRun UserCode/CMSPOS/test/run_segments.py
```

This should produce your root file : muon_timing.root

### 6. Make plots using the root tuple

Now you can make your plots from the root file segments.root using a plotting code based on the (under your UserCode directory) :

```
CMSPOS/test/plots.C
``` 

### 7. Complete the exercise

Complete the exercise following the instructions on the [DT exercise twiki](https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCMSPhysicsObjectSchoolAACHEN2019DT).

