/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVotingBinaryHoleFillingImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVotingBinaryHoleFillingImageFilter_h
#define __itkVotingBinaryHoleFillingImageFilter_h

#include "itkVotingBinaryImageFilter.h"

namespace itk
{
/** \class VotingBinaryHoleFillingImageFilter
 * \brief Fills in holes and cavities by iteratively applying a voting operation.
 *
 *
 * \sa Image
 * \sa VotingBinaryImageFilter
 * \sa Neighborhood
 * \sa NeighborhoodOperator
 * \sa NeighborhoodIterator
 * 
 * \ingroup IntensityImageFilters
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT VotingBinaryHoleFillingImageFilter :
    public VotingBinaryImageFilter< TInputImage, TOutputImage >
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  /** Standard class typedefs. */
  typedef VotingBinaryHoleFillingImageFilter Self;
  typedef VotingBinaryImageFilter< InputImageType, OutputImageType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VotingBinaryHoleFillingImageFilter, VotingBinaryImageFilter);
  
  /** Image typedef support. */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef typename InputImageType::SizeType InputSizeType;


   /** Maximum number of iterations. This filter is executed iteratively as
    * long as at least one pixel has changed in a previous iteration, or until
    * the maximum number of iterations has been reached. */
  itkGetConstReferenceMacro(MaximumNumberOfIterations, unsigned int);
  itkSetMacro(MaximumNumberOfIterations, unsigned int);

  /** Majority threshold. It is the number of pixels over 50% that will decide
   * whether an OFF pixel will become ON or not. For example, if the
   * neighborhood of a pixel has 124 pixels (excluding itself), the 50% will be
   * 62, and if you set upd a Majority threshold of 5, that means that the
   * filter will require 67 or more neighbor pixels to be ON in order to switch
   * the current OFF pixel to ON. The default value is 1. */ 
  itkGetConstReferenceMacro( MajorityThreshold, unsigned int );
  itkSetMacro( MajorityThreshold, unsigned int );


  /** Returns the number of pixels that changed when the filter was executed. */
  itkGetConstReferenceMacro( NumberOfPixelsChanged, unsigned int );

protected:
  VotingBinaryHoleFillingImageFilter();
  virtual ~VotingBinaryHoleFillingImageFilter() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

   /** Make protected the methods SetBirthThreshold() and
    * SetSurvivalThreshold() so users of this filter do not have access to
    * them. */
  void SetBirthThreshold( InputPixelType value ) 
            { this->Superclass::SetBirthThreshold( value );  }
  void SetSurvivalThreshold( InputPixelType value ) 
            { this->Superclass::SetSurvivalThreshold( value );  }


  /** VotingBinaryHoleFillingImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  VotingBinaryHoleFillingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_MaximumNumberOfIterations;
 
  unsigned int m_MajorityThreshold;

  unsigned int m_NumberOfPixelsChanged;
};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVotingBinaryHoleFillingImageFilter.txx"
#endif

#endif
