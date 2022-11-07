import os
import sys

import cjyx


# try get the path of the ruler scene file from the arguments
numArgs = len(sys.argv)
if numArgs > 1:
    scenePath = sys.argv[1]
else:
    # set the url as best guess from CJYX_HOME
    scenePath = os.path.join(os.environ['CJYX_HOME'], "../../Cjyx4/Modules/Loadable/Annotations/Testing/Data/Input/ruler.mrml")

scenePath = os.path.normpath(scenePath)
print("Trying to load ruler mrml file", scenePath)
cjyx.mrmlScene.SetURL(scenePath)


# and load it
cjyx.mrmlScene.Connect()
