/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */
#ifndef nsPageFrame_h___
#define nsPageFrame_h___

#include "nsHTMLContainerFrame.h"

// Page frame class used by the simple page sequence frame
class nsPageFrame : public nsContainerFrame {
public:
  friend nsresult NS_NewPageFrame(nsIFrame*& aResult);

  NS_IMETHOD  Reflow(nsIPresContext&      aPresContext,
                     nsHTMLReflowMetrics& aDesiredSize,
                     const nsHTMLReflowState& aMaxSize,
                     nsReflowStatus&      aStatus);

  NS_IMETHOD  CreateContinuingFrame(nsIPresContext&  aCX,
                                    nsIFrame*        aParent,
                                    nsIStyleContext* aStyleContext,
                                    nsIFrame*&       aContinuingFrame);

  /**
   * Get the "type" of the frame
   *
   * @see nsLayoutAtoms::pageFrame
   */
  NS_IMETHOD GetFrameType(nsIAtom** aType) const;
  
  // Debugging
  NS_IMETHOD  GetFrameName(nsString& aResult) const;

protected:
  nsPageFrame();
  ~nsPageFrame();
};

#endif /* nsPageFrame_h___ */

