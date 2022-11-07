import cjyx
import os


def testMRMLSceneImportAndExport():
    tempDir = cjyx.app.temporaryPath
    scenePath = tempDir + '/temp_scene.mrml'
    cjyx.mrmlScene.SetURL(scenePath)
    if not cjyx.mrmlScene.Commit(scenePath):
        raise Exception('Saving a MRML scene failed !')

    success = cjyx.mrmlScene.Import()
    os.remove(scenePath)
    if not success:
        raise Exception('Importing back a MRML scene failed !')


if __name__ == '__main__':
    testMRMLSceneImportAndExport()
