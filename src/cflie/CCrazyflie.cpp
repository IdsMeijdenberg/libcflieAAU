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

#include "cflie/CCrazyflie.h"


CCrazyflie::CCrazyflie(CCrazyRadio *crRadio) {
  m_crRadio = crRadio;
  m_nLastRequestedVariableIndex = -1;
  m_nThrust = 0;
  
  //this->updateTOC();
  this->updateLogTOC();
}

CCrazyflie::~CCrazyflie() {
}

bool CCrazyflie::sendSetpoint(float fRoll, float fPitch, float fYaw, short sThrust) {
  fPitch = -fPitch;
  
  int nSize = 3 * sizeof(float) + sizeof(short);
  char cBuffer[nSize];
  
  memcpy(&cBuffer[0 * sizeof(float)], &fRoll, sizeof(float));
  memcpy(&cBuffer[1 * sizeof(float)], &fPitch, sizeof(float));
  memcpy(&cBuffer[2 * sizeof(float)], &fYaw, sizeof(float));
  memcpy(&cBuffer[3 * sizeof(float)], &sThrust, sizeof(short));
  
  CCRTPPacket *crtpPacket = new CCRTPPacket(cBuffer, nSize, 3);
  CCRTPPacket *crtpReceived = m_crRadio->sendPacket(crtpPacket);
  
  if(crtpPacket != NULL) {
    delete crtpPacket;
    return true;
  } else {
    return false;
  }
}

void CCrazyflie::updateTOC() {
  char cOne = 1;
  m_crRadio->setUpdatesParameterCount(true);
  
  CCRTPPacket *crtpPacket = new CCRTPPacket(&cOne, 1, 2);
  CCRTPPacket *crtpReceived = m_crRadio->sendPacket(crtpPacket);
  
  delete crtpPacket;
  if(crtpReceived) {
    delete crtpReceived;
  }
}

void CCrazyflie::populateTOCElement(int nIndex) {
  char cRequest[2];
  cRequest[0] = 0;
  cRequest[1] = nIndex;
  cout << (int)cRequest[1] << endl;
  CCRTPPacket *crtpPacket = new CCRTPPacket(cRequest, 2, 2);
  CCRTPPacket *crtpReceived = m_crRadio->sendPacket(crtpPacket);
  
  delete crtpPacket;
  if(crtpReceived) {
    delete crtpReceived;
  }
}

void CCrazyflie::populateNextTOCElement() {
  if(m_crRadio->parameterCount() > 0) {
    m_nLastRequestedVariableIndex++;
    
    if(m_nLastRequestedVariableIndex < m_crRadio->parameterCount()) {
      this->populateTOCElement(m_nLastRequestedVariableIndex);
    }
  }
}

void CCrazyflie::updateLogTOC() {
  char cOne = 1;
  m_crRadio->setUpdatesLogParameterCount(true);
  m_crRadio->setLogElementPopulated(true);
  
  CCRTPPacket *crtpPacket = new CCRTPPacket(&cOne, 1, 5);
  crtpPacket->setChannel(0);
  
  CCRTPPacket *crtpReceived = m_crRadio->sendPacket(crtpPacket);
  
  delete crtpPacket;
  if(crtpReceived) {
    delete crtpReceived;
  }
}

void CCrazyflie::populateLOGElement(int nIndex) {
  char cRequest[2];
  cRequest[0] = 0;
  cRequest[1] = nIndex;
  cout << nIndex << ", " << (m_crRadio->countLOGElements() == 0 ? 1 : 2) << endl;
  CCRTPPacket *crtpPacket = new CCRTPPacket(cRequest, (m_crRadio->countLOGElements() == 0 ? 1 : 2), 2);
  crtpPacket->setChannel(2);
  CCRTPPacket *crtpReceived = m_crRadio->sendPacket(crtpPacket);
  
  delete crtpPacket;
  if(crtpReceived) {
    delete crtpReceived;
  }
}

void CCrazyflie::populateNextLOGElement() {
  if(m_crRadio->populatesLOGCache() && m_crRadio->logElementPopulated()) {
    int nNextLogVarID = m_crRadio->nextLogVarID();
    cout << nNextLogVarID << endl;
    if(nNextLogVarID != 0xff) {
      m_crRadio->setLogElementPopulated(false);
      this->populateLOGElement(nNextLogVarID);
    } else {
      m_crRadio->setPopulatesLOGCache(false);
    }
  }
}

void CCrazyflie::setThrust(int nThrust) {
  m_nThrust = nThrust;
}

int CCrazyflie::thrust() {
  return m_nThrust;
}

void CCrazyflie::cycle() {
  this->sendSetpoint(m_fRoll, m_fPitch, m_fYaw, m_nThrust);
  
  if(m_crRadio->populatesTOCCache()) {
    //this->populateNextTOCElement();
  }
  
  if(m_crRadio->populatesLOGCache()) {
    this->populateNextLOGElement();
  }
}

void CCrazyflie::setRoll(float fRoll) {
  m_fRoll = fRoll;
}

float CCrazyflie::roll() {
  return m_fRoll;
}

void CCrazyflie::setPitch(float fPitch) {
  m_fPitch = fPitch;
}

float CCrazyflie::pitch() {
  return m_fPitch;
}

void CCrazyflie::setYaw(float fYaw) {
  m_fYaw = fYaw;
}

float CCrazyflie::yaw() {
  return m_fYaw;
}
