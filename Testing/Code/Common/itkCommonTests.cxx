// this file defines the itkBasicFiltersTest for the test driver
// and all it expects is that you have a function called RegisterTests
#include <iostream>
#include "itkTestMain.h" 


void RegisterTests()
{
REGISTER_TEST(itkModifiedTimeTest );
REGISTER_TEST(itkAdaptorComparisonTest );
REGISTER_TEST(itkAffineTransformTest );
REGISTER_TEST(itkArrayTest );
REGISTER_TEST(itkArray2Test );
REGISTER_TEST(itkAutoPointerTest );
REGISTER_TEST(itkAzimuthElevationToCartesianTransformTest );
REGISTER_TEST(itkBinaryThresholdImageFunctionTest );
REGISTER_TEST(itkBoundingBoxTest );
REGISTER_TEST(itkBSplineInterpolateImageFunctionTest );
REGISTER_TEST(itkByteSwapTest );
REGISTER_TEST(itkConstNeighborhoodIteratorTest );
REGISTER_TEST(itkConstSmartNeighborhoodIteratorTest );
REGISTER_TEST(itkConstRandomAccessNeighborhoodIteratorTest );
REGISTER_TEST(itkCovariantVectorGeometryTest );
REGISTER_TEST(itkDataTypeTest );
REGISTER_TEST(itkDynamicMeshTest );
REGISTER_TEST(itkEuler2DTransformTest );
REGISTER_TEST(itkExceptionObjectTest );
REGISTER_TEST(itkHashTableTest );
REGISTER_TEST(itkImageAdaptorTest );
REGISTER_TEST(itkImageIteratorTest );
REGISTER_TEST(itkImageIteratorsForwardBackwardTest );
REGISTER_TEST(itkImageLinearIteratorTest );
REGISTER_TEST(itkImageRandomIteratorTest );
REGISTER_TEST(itkImageRegionTest );
REGISTER_TEST(itkImageRegionExclusionIteratorWithIndexTest );
REGISTER_TEST(itkImageSliceIteratorTest );
REGISTER_TEST(itkIteratorTests );
REGISTER_TEST(itkLightObjectTest );
REGISTER_TEST(itkLevelSetFunctionTest );
REGISTER_TEST(itkMatrixTest );
REGISTER_TEST(itkMapContainerTest );
REGISTER_TEST(itkMemoryLeakTest );
REGISTER_TEST(itkMeshTest );
REGISTER_TEST(itkMeshFstreamTest );
REGISTER_TEST(itkNeighborhoodTest );
REGISTER_TEST(itkNeighborhoodIteratorTest );
REGISTER_TEST(itkNeighborhoodOperatorTest );
REGISTER_TEST(itkObjectStoreTest );
REGISTER_TEST(itkPixelAccessTest );
REGISTER_TEST(itkPointGeometryTest );
REGISTER_TEST(itkPointSetTest );
REGISTER_TEST(itkRandomAccessNeighborhoodIteratorTest );
REGISTER_TEST(itkRGBPixelTest );
REGISTER_TEST(itkRigid2DTransformTest );
REGISTER_TEST(itkRigid3DTransformTest );
REGISTER_TEST(itkRigid3DPerspectiveTransformTest );
REGISTER_TEST(itkSliceIteratorTest );
REGISTER_TEST(itkSpatialFunctionTest );
REGISTER_TEST(itkSmartPointerTest );
REGISTER_TEST(itkTimeProbesTest );
REGISTER_TEST(itkTransformTest );
REGISTER_TEST(itkThreadDefsTest );
REGISTER_TEST(itkTranslationTransformTest );
REGISTER_TEST(itkVectorGeometryTest );
REGISTER_TEST(itkVersorTest );
REGISTER_TEST(itkVersorRigid3DTransformTest );
REGISTER_TEST(itkVectorTest );
REGISTER_TEST(itkVectorInterpolateImageFunctionTest );
REGISTER_TEST(itkScaleTransformTest );
REGISTER_TEST(itkSplineKernelTransformTest );
REGISTER_TEST(itkEllipsoidInteriorExteriorSpatialFunctionTest );
REGISTER_TEST(itkSymmetricEllipsoidInteriorExteriorSpatialFunctionTest );
}
