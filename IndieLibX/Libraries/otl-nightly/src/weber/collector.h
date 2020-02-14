/*
  Copyright (c) 2004 Pascal Kirchdorfer, Seth Yastrov

  This software is provided 'as-is', without any express or implied warranty. In
  no event will the authors be held liable for any damages arising from the use
  of this software.

  Permission is granted to anyone to use this software for any purpose, including
  commercial applications, and to alter it and redistribute it freely, subject to
  the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim
       that you wrote the original software. If you use this software in a product,
       an acknowledgment in the product documentation would be appreciated but is
       not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _WEBER_COLLECTOR_H_
#define _WEBER_COLLECTOR_H_

#include "opentree/opentree.h"
#include "tree.h"
#include "stem.h"
#include "segment.h"
#include "leaf.h"

#include "opentree/ottree.h"
#include "opentree/otstem.h"
#include "opentree/otleaf.h"

#include "opentree/utils/otpolylinespline.h"
#include "opentree/utils/otpolylinespline3.h"

namespace opentree
{

  /// \addtogroup Weber_Algorithm
  /// @{

  ///Parses the Webertree and creates an otTree
  class Collector
  {
  private:
    otWeberTree* tree;
    otTree* ottree;

    void collectTree()
    {
      ottree = new otTree(tree->stemrelations.getCount(), tree->getLeafCount());
      for (unsigned int i = 0; i < tree->stemrelations.getCount(); i++)
      {
        collectStem(tree->stemrelations.get(i), 0);
      }
    }

    void collectStem(otWeberStemRelations* stem, otStem* parent)
    {
      if (stem->segments.getCount() == 0) return;

      otStem* new_stem = new otStem(stem->level, stem->leaves.getCount(), 
        stem->clones.getCount()+stem->substems.getCount(), stem->offset);

      otPolyLineSpline* rad_spline = new otPolyLineSpline(stem->segments.getCount());
      otPolyLineSpline3* pos_spline = new otPolyLineSpline3(stem->segments.getCount());

      new_stem->setRadSpline(rad_spline);
      new_stem->setPosSpline(pos_spline);

      if (parent) 
        parent->addChild(new_stem);
      else 
        ottree->addTrunk(new_stem);

      unsigned int i;

      for (i = 0; i < stem->segments.getCount(); i++)
      {
        collectSegment(stem->segments.get(i), new_stem, i, stem->segments.getCount()-1);
      }
      for (i = 0; i < stem->substems.getCount(); i++)
      {
        collectStem(stem->substems.get(i), new_stem);
      }
      for (i = 0; i < stem->clones.getCount(); i++)
      {
        collectStem(stem->clones.get(i), new_stem);
      }
      for (i = 0; i < stem->leaves.getCount(); i++)
      {
        collectLeaf(&stem->leaves.get(i), new_stem);
      }
    }

    void collectSegment(otWeberSegment* segment, otStem* parent, unsigned int idx, unsigned int total)
    {
      ((otPolyLineSpline3*)parent->getPosSpline())->setPoint((float)idx/total, segment->getTranslation());
      ((otPolyLineSpline*)parent->getRadSpline())->setPoint((float)idx/total, segment->getRad());
    }

    void collectLeaf(otWeberLeaf* leaf, otStem* parent)
    {
      otLeaf otleaf(leaf->trf, (float)leaf->length, (float)leaf->width);
      parent->addLeaf(otleaf);
    }

  public:
    Collector(otWeberTree* tree) : tree(tree), ottree(0) {}
    virtual ~Collector() { }

    void collect()
    {
      collectTree();
    }

    otTree* returnTree()
    {
      return ottree;
    }
  };

  /// }@

};

#endif //_WEBERARBARO_COLLECTOR_H_
