// Copyright (c) 2013, Jan Winkler <winkler@cs.uni-bremen.de>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Universität Bremen nor the names of its
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __C_CRAZYFLIE_H__
#define __C_CRAZYFLIE_H__


#include <iostream>
#include <sstream>
#include <math.h>

#include "CCrazyRadio.h"
#include "CController.h"
#include "CPController.h"

using namespace std;


enum Controller {
  CTRL_NONE = 0,
  CTRL_P = 1
};


class CCrazyflie {
 private:
  CCrazyRadio *m_crRadio;
  int m_nLastRequestedVariableIndex;
  int m_nThrust;
  float m_fRoll;
  float m_fPitch;
  float m_fYaw;
  CController *m_ctrlController;
  struct DSPose m_dspCurrentPose;
  struct DSPose m_dspDesiredPose;
  enum Controller m_enumCtrl;
  double m_dSecondsLast;
  
  // Control related parameters
  float m_fMaxAbsRoll;
  float m_fMaxAbsPitch;
  float m_fMaxAbsYaw;
  int m_nMaxThrust;
  int m_nMinThrust;

 public:
  CCrazyflie(CCrazyRadio *crRadio);
  ~CCrazyflie();
  
  bool sendSetpoint(float fRoll, float fPitch, float fYaw, short sThrust);
  void updateTOC();
  void populateTOCElement(int nIndex);
  void populateNextTOCElement();

  void populateLOGElement(int nIndex);
  void populateNextLOGElement();
  
  void updateLogTOC();
  
  void setThrust(int nThrust);
  int thrust();
  
  void setRoll(float fRoll);
  float roll();

  void setPitch(float fPitch);
  float pitch();

  void setYaw(float fYaw);
  float yaw();
  
  void cycle();
  
  void disableController();
  void setPController(float fPGain);
  
  void setDesiredPose(struct DSPose dspDesired);
  void applyControllerResult();
};


#endif /* __C_CRAZYFLIE_H__ */