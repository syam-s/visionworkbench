// __BEGIN_LICENSE__
//
// Copyright (C) 2006 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration
// (NASA).  All Rights Reserved.
// 
// This software is distributed under the NASA Open Source Agreement
// (NOSA), version 1.3.  The NOSA has been approved by the Open Source
// Initiative.  See the file COPYING at the top of the distribution
// directory tree for the complete NOSA document.
// 
// THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY
// KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT
// LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO
// SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
// A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT
// THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT
// DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE.
//
// __END_LICENSE__

// TestEdgeExtend.h
#include <cxxtest/TestSuite.h>

#include <vw/Image/ImageView.h>
#include <vw/Image/EdgeExtend.h>

using namespace vw;

class TestEdgeExtend : public CxxTest::TestSuite
{
public:

  template <template<class> class TraitT, class T>
  static bool bool_trait( T const& arg ) {
    return TraitT<T>::value;
  }

  void testZeroEdgeExtend()
  {
    ImageView<double> im(2,3); im(0,0)=1; im(1,0)=2; im(0,1)=3; im(1,1)=4; im(0,2)=5; im(1,2)=6;
    EdgeExtendView<ImageView<double>, ZeroEdgeExtend> im2 = edge_extend(im, ZeroEdgeExtend() );
    TS_ASSERT_EQUALS( im2.cols(), 2 );
    TS_ASSERT_EQUALS( im2.rows(), 3 );
    TS_ASSERT_EQUALS( im2(-1,0), 0 );
    TS_ASSERT_EQUALS( im2(0,-1), 0 );
    TS_ASSERT_EQUALS( im2(2,0), 0 );
    TS_ASSERT_EQUALS( im2(0,3), 0 );
    TS_ASSERT_EQUALS( im2(-1,-1), 0 );
    TS_ASSERT_EQUALS( im2(2,3), 0 );

    // Teste accessor
    TS_ASSERT_EQUALS( *(im2.origin().advance(-1,-1)), 0 );
    TS_ASSERT_EQUALS( *(im2.origin().advance(1,1)), 4 );

    // Test the traits
    TS_ASSERT( !bool_trait<IsReferenceable>( edge_extend(im, ZeroEdgeExtend() ) ) );
    TS_ASSERT( !bool_trait<IsMultiplyAccessible>( edge_extend(im, ZeroEdgeExtend() ) ) );
    TS_ASSERT( !bool_trait<IsReferenceable>( edge_extend(im, ZeroEdgeExtend()).origin() ) );
  }

  void testConstantEdgeExtend()
  {
    ImageView<double> im(2,3); im(0,0)=1; im(1,0)=2; im(0,1)=3; im(1,1)=4; im(0,2)=5; im(1,2)=6;
    EdgeExtendView<ImageView<double>, ConstantEdgeExtend> im2 = edge_extend(im, ConstantEdgeExtend() );
    TS_ASSERT_EQUALS( im2.cols(), 2 );
    TS_ASSERT_EQUALS( im2.rows(), 3 );
    TS_ASSERT_EQUALS( im2(-1,0), 1 );
    TS_ASSERT_EQUALS( im2(1,-1), 2 );
    TS_ASSERT_EQUALS( im2(2,0), 2 );
    TS_ASSERT_EQUALS( im2(0,3), 5 );
    TS_ASSERT_EQUALS( im2(-1,-1), 1 );
    TS_ASSERT_EQUALS( im2(2,3), 6 );

    // Teste accessor
    TS_ASSERT_EQUALS( *(im2.origin().advance(-1,-1)), 1 );
    TS_ASSERT_EQUALS( *(im2.origin().advance(1,1)), 4 );

    // Test the traits
    TS_ASSERT( !bool_trait<IsReferenceable>( edge_extend(im, ZeroEdgeExtend() ) ) );
    TS_ASSERT( !bool_trait<IsMultiplyAccessible>( edge_extend(im, ZeroEdgeExtend() ) ) );
    TS_ASSERT( !bool_trait<IsReferenceable>( edge_extend(im, ZeroEdgeExtend()).origin() ) );
  }

  void testReflectEdgeExtend()
  {
    ImageView<double> im(2,3); im(0,0)=1; im(1,0)=2; im(0,1)=3; im(1,1)=4; im(0,2)=5; im(1,2)=6;
    EdgeExtendView<ImageView<double>, ReflectEdgeExtend> im2 = edge_extend(im, ReflectEdgeExtend() );
    TS_ASSERT_EQUALS( im2.cols(), 2 );
    TS_ASSERT_EQUALS( im2.rows(), 3 );
    TS_ASSERT_EQUALS( im2(-1,0), 2 );
    TS_ASSERT_EQUALS( im2(1,-2), 6 );
    TS_ASSERT_EQUALS( im2(3,0), 2 );
    TS_ASSERT_EQUALS( im2(0,4), 5 );
    TS_ASSERT_EQUALS( im2(-1,-1), 4 );
    TS_ASSERT_EQUALS( im2(2,3), 6 );

    // Teste accessor
    TS_ASSERT_EQUALS( *(im2.origin().advance(-1,-1)), 4 );
    TS_ASSERT_EQUALS( *(im2.origin().advance(1,1)), 4 );

    // Test the traits
    TS_ASSERT( !bool_trait<IsReferenceable>( edge_extend(im, ZeroEdgeExtend() ) ) );
    TS_ASSERT( !bool_trait<IsMultiplyAccessible>( edge_extend(im, ZeroEdgeExtend() ) ) );
    TS_ASSERT( !bool_trait<IsReferenceable>( edge_extend(im, ZeroEdgeExtend()).origin() ) );
  }

};
