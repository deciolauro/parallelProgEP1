## @file parser.py
#  @brief Simple parser script to extract all the relevant features
#
#  This Python parser extracts all the relevant features from all .log files
#  produced by the execution of run_measurements.sh script and dumps the
#  information in a prickle file named finaldata.p to be used by the plotter
#  script
#
#  Usage:
#   python parser.py
#
#  Notes:
#   - This file and all the .log files should be placed in the same directory
#
#  @author		Candy Tenorio Gonzales (cvtenoriog@gmail.com)
#  @author		Decio Lauro Soares (deciolauro@gmail.com)
#  @author		Fatemeh Mosaiyebzadeh (mahshid.msy179@gmail.com)
#  @date		24 Apr 2017
#  @bug		    No known bugs
#  @warning	    The parser and the .log files should be in the same directory
#  @copyright	GNU Public License v3
import os
import numpy
import pickle

maxImageSize = 8192
finalToPlot = []

##
#  - Index  Labels explanation
#  - 0     TC       -> task-clock (msec)
#  - 1     StdTC    -> standard-dev 10 measurements of TC
#  - 2     CSW      -> Context-switches (number of)
#  - 3     StdCSW   -> std 10 measuments of CSW
#  - 4     CPUmg    -> CPU-migrations (number of)
#  - 5     StdCPUmg -> std 10 measuments of CPUmg
#  - 6     PF       -> page-faults (number of)
#  - 7     StdPF    -> std 10 meas. of PF
#  - 8     ET       -> Elapsed time (sec)
#  - 9     StdET    -> std 10 meas. of ET
extractedLabels = ("TC", "StdTC", "CSW", "StdCSW", "CPUmg", "StdCPUmg", "PF",
                   "StdPF", "ET", "StdET")

validInfo = ("task-clock", "context-switches", "page-faults", "cpu-migrations")
fullInfo = validInfo + ("Starting", "seconds")

for filename in [f for f in os.listdir(os.getcwd()) if f.endswith('.log')]:
    try:
        currentFile = open(filename)
    except IOError:
        print "Unable to open file %s" % str(filename)
    testType = (filename.split("."))[0]
    toPlot = []
    dataPlot = []
    for line in currentFile:
        # Avoid processing blank/clear lines
        aux = line.split()
        if(len(aux) > 0):
            # check to see if there is the main info line and extract
            if(aux[0] == "Starting"):
                threadImgSize = (
                    int(((line.split()[3].replace(',', '')).split('='))[1]),
                    int(((line.split()[5].replace(',', '')).split('='))[1]))
                toPlot.append(testType)
                toPlot.append(threadImgSize)
            else:
                if(aux[1] in fullInfo):
                    try:
                        hasPercentage = False
                        aux0 = aux[0].replace(',', '')
                        auxNoP = filter(lambda x: '%' in x, aux)
                        if(len(auxNoP) == 1):
                            hasPercentage = True
                        if(aux[1] in validInfo):
                            dataPlot.append(float(aux0))
                            if(hasPercentage):
                                dataPlot.append(float(
                                 (auxNoP[0].replace('+-', ' ')).split('%')[0]))
                            else:
                                dataPlot.append(0)
                        elif(aux[1] == "seconds"):
                            dataPlot.append(float(aux[0]))
                            dataPlot.append(float(aux[6].split('%')[0]))
                            toPlot.append(tuple(dataPlot))
                            finalToPlot.append(tuple(toPlot))
                            toPlot = []
                            dataPlot = []
                    except ValueError as e:
                        print "####################"
                        print "An error happened during the parsing in line: "
                        print e
                        print "####################"

# filter bigger cases (greater than maxImageSize)
finalToPlotFiltered = [x for x in finalToPlot if x[1][1] <= maxImageSize]

pickle.dump(finalToPlotFiltered, open("finaldata.p", "wb"))
