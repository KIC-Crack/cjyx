import cjyx


def testMRMLCreateNodeByClassWithSetReferenceCountMinusOne():
    n = cjyx.mrmlScene.CreateNodeByClass('vtkMRMLViewNode')
    n.UnRegister(None)  # the node object is now owned by n Python variable therefore we can release the reference that CreateNodeByClass added
    cjyx.mrmlScene.AddNode(n)


if __name__ == '__main__':
    testMRMLCreateNodeByClassWithSetReferenceCountMinusOne()
