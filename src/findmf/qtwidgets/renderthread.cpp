// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <QtGui>

#include <math.h>
#include <iostream>
#include "renderthread.h"


//! [0]
RenderThread::RenderThread(QObject *parent)
  : QThread(parent)
{
  restart = false;
  abort = false;

  for (int i = 0; i < ColormapSize; ++i){
    colormap[i] = rgbFromWaveLength(380.0 + (i * 400.0 / ColormapSize));
    //colormap[i] = i;
    }
}

void RenderThread::setMap(ralab::MapVis * map){
  map_ = map;
}

RenderThread::~RenderThread()
{
  mutex.lock();
  abort = true;
  condition.wakeOne();
  mutex.unlock();
  wait();
}

void RenderThread::render(double centerX, double centerY, double scaleFactor,
                          QSize resultSize)
{
  QMutexLocker locker(&mutex);
  this->centerX = centerX;
  this->centerY = centerY;
  this->scaleFactor = scaleFactor;
  this->resultSize = resultSize;

  if (!isRunning()) {
      start(LowPriority);
    } else {
      restart = true;
      condition.wakeOne();
    }
}


void RenderThread::run()
{
  forever {
    mutex.lock();
    QSize resultSize = this->resultSize;
    double scaleFactor = this->scaleFactor;
    double centerX = this->centerX;
    double centerY = this->centerY;

    mutex.unlock();
    int halfWidth = resultSize.width() / 2;
    int halfHeight = resultSize.height() / 2;
    QImage image( resultSize , QImage::Format_RGB32 );
    for (int y = -halfHeight; y < halfHeight; ++y) {
        if (restart)
          break;
        if (abort)
          return;
        uint *scanLine =
            reinterpret_cast<uint *>(image.scanLine(y + halfHeight));
        for (int x = -halfWidth; x < halfWidth; ++x,++scanLine) {
            uint32_t px = x + halfWidth + centerX;
            uint32_t py = y + halfHeight + centerY;
            if(px < map_->getMZsize() && py < map_->getRTsize())
              {
                //double x =  sqrt(map_->get( px,  py )+1.)/sqrt(map_->getMaxelem()+1.);
                double pixelval = map_->get( px,  py );
                double maxelem = map_->getMaxelem();
                double minelem = map_->getMinelem();

                //double x =  log(pixelval + 1.) / log(maxelem + 1.);
                double x = (pixelval-minelem)/(maxelem-minelem);
                int col = static_cast<int>( x * (ColormapSize - 1));
                if(col < ColormapSize && col >=0){
                    *scanLine = colormap[col];
                  }else{
                    *scanLine = qRgb(0, 0, 0);
                  }

              } else {
                *scanLine = qRgb(0, 0, 0);
              }
          }
      }
    if (!restart)
      emit renderedImage(image, scaleFactor);
    mutex.lock();
    if (!restart)
      condition.wait(&mutex);
    restart = false;
    mutex.unlock();
  }
}

uint RenderThread::rgbFromWaveLength(double wave)
{
  double r = 0.0;
  double g = 0.0;
  double b = 0.0;

  if (wave >= 380.0 && wave <= 440.0) {
      r = -1.0 * (wave - 440.0) / (440.0 - 380.0);
      b = 1.0;
    } else if (wave >= 440.0 && wave <= 490.0) {
      g = (wave - 440.0) / (490.0 - 440.0);
      b = 1.0;
    } else if (wave >= 490.0 && wave <= 510.0) {
      g = 1.0;
      b = -1.0 * (wave - 510.0) / (510.0 - 490.0);
    } else if (wave >= 510.0 && wave <= 580.0) {
      r = (wave - 510.0) / (580.0 - 510.0);
      g = 1.0;
    } else if (wave >= 580.0 && wave <= 645.0) {
      r = 1.0;
      g = -1.0 * (wave - 645.0) / (645.0 - 580.0);
    } else if (wave >= 645.0 && wave <= 780.0) {
      r = 1.0;
    } else {
	  r =0.0;
	}

  double s = 1.0;
  if (wave > 700.0)
    s = 0.3 + 0.7 * (780.0 - wave) / (780.0 - 700.0);
  else if (wave <  420.0)
    s = 0.3 + 0.7 * (wave - 380.0) / (420.0 - 380.0);
  else
	  s = 1.0;

  r = pow(r * s, 0.8);
  g = pow(g * s, 0.8);
  b = pow(b * s, 0.8);
  return qRgb(int(r * 255), int(g * 255), int(b * 255));
}


/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/


