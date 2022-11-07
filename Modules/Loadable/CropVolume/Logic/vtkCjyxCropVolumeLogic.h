/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Cjyx
  Module:    $RCSfile: vtkCjyxCropVolumeLogic.h,v $
  Date:      $Date: 2006/01/08 04:48:05 $
  Version:   $Revision: 1.45 $

=========================================================================auto=*/

// .NAME vtkCjyxCropVolumeLogic - cjyx logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkCjyxCropVolumeLogic_h
#define __vtkCjyxCropVolumeLogic_h

// Cjyx includes
#include "vtkCjyxModuleLogic.h"
class vtkCjyxCLIModuleLogic;
class vtkCjyxVolumesLogic;
class vtkMRMLVolumeNode;
class vtkMRMLDisplayableNode;
// vtk includes
class vtkMatrix4x4;
// CropVolumes includes
#include "vtkCjyxCropVolumeModuleLogicExport.h"
class vtkMRMLCropVolumeParametersNode;


/// \class vtkCjyxCropVolumeLogic
/// \brief Crop a volume to the specified region of interest.
///
/// This class implements cropping and resampling of a volume.
/// Two main use cases:
///
/// 1. Reduce size (both extent and resolution) of a large volume.
/// Size reduction is useful, as it reduces memory need and makes
/// visualization and processing faster.
///
/// 2. Increase resolution of a specific region.
/// Increasing resolution (decreasing voxel size) is useful for
/// segmentation and visualization of fine details.
///
/// If interpolation is disabled then only the extent of the volume
/// is decreased. Cropping without resampling is very fast and needs
/// almost no extra memory.
///
/// If interpolation is enabled, then both the size and resolution
/// of the volume can be changed.
///
/// Limitations:
/// * Region of interes (ROI) node cannot be under non-linear transform
/// * Cropped output volume node cannot be under non-linear transform
/// * If interpolation is disabled: input volume node cannot be under non-linear transform
///   and ROI node must be aligned with the input volume
///
/// \ingroup Cjyx_QtModules_CropVolume
class VTK_CJYX_CROPVOLUME_MODULE_LOGIC_EXPORT vtkCjyxCropVolumeLogic
  : public vtkCjyxModuleLogic
{
public:

  static vtkCjyxCropVolumeLogic *New();
  vtkTypeMacro(vtkCjyxCropVolumeLogic,vtkCjyxModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Crop input volume using the specified ROI node.
  int Apply(vtkMRMLCropVolumeParametersNode*);

  /// Perform non-interpolated (voxel-based) cropping.
  /// If limitToInputExtent is set to true (default) then the extent can only be smaller than the input volume.
  static int CropVoxelBased(vtkMRMLDisplayableNode* roi, vtkMRMLVolumeNode* inputVolume,
    vtkMRMLVolumeNode* outputNode, bool limitToInputExtent=true, double fillValue=0.0);

  /// Compute non-interpolated (voxel-based) cropping output volume geometry (without actually cropping the image).
  /// If limitToInputExtent is set to true (default) then the extent can only be smaller than the input volume.
  static bool GetVoxelBasedCropOutputExtent(vtkMRMLDisplayableNode* roi, vtkMRMLVolumeNode* inputVolume,
    int outputExtent[6], bool limitToInputExtent=false);

  /// Perform interpolated cropping.
  int CropInterpolated(vtkMRMLDisplayableNode* roi, vtkMRMLVolumeNode* inputVolume, vtkMRMLVolumeNode* outputNode,
    bool isotropicResampling, double spacingScale, int interpolationMode, double fillValue);

  /// Computes output volume geometry for interpolated cropping (without actually cropping the image).
  static bool GetInterpolatedCropOutputGeometry(vtkMRMLDisplayableNode* roi, vtkMRMLVolumeNode* inputVolume,
    bool isotropicResampling, double spacingScale, int outputExtent[6], double outputSpacing[3]);

  /// Sets ROI to fit to input volume.
  /// If ROI is under a non-linear transform then the ROI transform will be reset to RAS.
  static bool FitROIToInputVolume(vtkMRMLCropVolumeParametersNode* parametersNode);

  static void SnapROIToVoxelGrid(vtkMRMLCropVolumeParametersNode* parametersNode);

  static bool IsROIAlignedWithInputVolume(vtkMRMLCropVolumeParametersNode* parametersNode);

  void RegisterNodes() override;

protected:
  vtkCjyxCropVolumeLogic();
  ~vtkCjyxCropVolumeLogic() override;

private:
  vtkCjyxCropVolumeLogic(const vtkCjyxCropVolumeLogic&) = delete;
  void operator=(const vtkCjyxCropVolumeLogic&) = delete;

  class vtkInternal;
  vtkInternal* Internal;
};

#endif

