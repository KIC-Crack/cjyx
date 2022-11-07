import cjyx
from cjyx import cjyxvolumerenderingtestdatapaths as datapath

filepath = datapath.input + '/VolumeRendering_CTAbdomen_AppleTutorial.xml'
testUtility = cjyx.app.testingUtility()
success = testUtility.playTests(filepath)
if not success:
    raise Exception('Failed to finished properly the play back !')
